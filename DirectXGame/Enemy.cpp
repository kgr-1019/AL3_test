#include "Enemy.h"
#include "TextureManager.h"
#include"Player.h"
#include"cassert"
void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& approachVelocity, const Vector3& leaveVelocity) {
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("wan.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ関数に移し替える
	velocity_ = velocity;
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;

	// 発射処理の呼び出し
	enemyBullet_->Fire();
}

void Enemy::Update()
{
	//移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 行列を更新
	worldTransform_.UpdateMatrix();

	// 行動フェーズ
	/*
	状態遷移を実装するためのもっとも基礎的な方法が switch 文。

	それぞれの状態ごとの処理として
	①その状態固有の更新処理(移動とか)
	②別の状態への移行処理
	を含む。

	なお、各フェーズの速度はゲームバランスに強く影響するので
	プランナーが調整しやすいように別ファイルに分けたい。
	(.txt とか .csv 、.json などにしてロード)
	*/
	switch (phase_)
	{ 
	case Phase::Approach:
	default:
		// 移動(ベクトルを加算)
		worldTransform_.translation_ = Add(worldTransform_.translation_, approachVelocity_);
		
		// 既定の位置に到達したら離脱
		/*
		接近フェーズ中に条件を満たしたら離脱フェーズに移行する
		*/
		if (worldTransform_.translation_.z < -20.0f)
		{
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		// 移動(ベクトルを加算)
		worldTransform_.translation_ = Add(worldTransform_.translation_, leaveVelocity_);
		break;
	}


	// 弾更新
	// for文でリスト内のすべてのPlayerBulletについて1個1個処理していく。
	for (EnemyBullet* enemyBullet : enemyBullets_) // 範囲for文。コンテナや配列を簡潔に扱うためのfor文の別表現。
	{
		// PlayerBulletのポインタのリストからPlayerBulletのポインタを1個づつ
		// 取り出しながら処理を回していく。
		// PlayerBullet* がリスト内の要素1個分の型。
		enemyBullet->Update();
	}

	// デスフラグの立った弾を削除
	/*
	死んだ弾を削除する処理。
	std::list のメンバ関数である remove_if() は、条件に当てはまる要素を
	リストから排除する関数。
	条件判定用の関数を渡す必要があるのだが、このように true か false を返す
	ラムダ式を指定すると便利。

	true を返した弾は list から取り除かれる。
	削除条件を満たした場合に delete した上で true を返すことで、インスタンスの
	解放と list からの除外を両立させる。
	*/
	enemyBullets_.remove_if([](EnemyBullet* enemyBullet) {
		if (enemyBullet->IsDead()) {
			delete enemyBullet;
			return true;
		}
		return false;
	});
}


void EnemyBullet::Fire()
{
	// 弾の速度
	/*
	弾の速度ベクトル(1frmの移動量)を設定する。
	この場合は1frmにつきZ方向に1.0f進む設定。
	*/
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	/*
	開店前の、Z方向向きのベクトルに自キャラの向きと同じ回転をかけることで、
	自キャラと同じ向きのベクトルになる。
	この計算を発射する瞬間の一度だけ行うことで、発射後は最初の向きを保ったまま
	同じ角度で進む。
	*/
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾があれば開放する
	/*if (bullet_)
	{
	    delete bullet_;
	    bullet_ = nullptr;
	}*/
	// ↑ポインタが複数持てて、前の弾を指すポインタを残しておけるから、
	// 　メモリリークの心配がなくなるので解放する必要がなくなる。

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	//enemyBullets_.push_back(newBullet);
};


void Enemy::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	// 全部の弾の描画を呼び出す。
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection);
	}
}
