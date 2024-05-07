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
	textureHandle_ = TextureManager::Load("tex1.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ関数に移し替える
	velocity_ = velocity;
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;
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
		if (worldTransform_.translation_.z < 0.0f)
		{
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		// 移動(ベクトルを加算)
		worldTransform_.translation_ = Add(worldTransform_.translation_, leaveVelocity_);
		break;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
