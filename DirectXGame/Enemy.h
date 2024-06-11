#pragma once
#include"Model.h"
#include"Vector3.h"
#include "WorldTransform.h"
#include"EnemyBullet.h"
#include<list>
#include"Input.h"

// ゲームシーンクラスの前方宣言
class GameScene;

// 自機クラスの前方宣言
/*
敵に自キャラを貸し出す
 
弾を発射するのは敵だが、自機狙いのベクトルを計算するには
自キャラの座標が情報として必要。

しかし Enemy.h で Player.h をインクルードすると
Enemy が Player に依存してしまう。
(Player がないと Enemy が成立しない状況)

C++ではこういう場合の苦肉の策として、インクルードの代わりに
クラスの前方宣言を行う。

クラスの前方宣言を行うことで、インクルードせずに
そのクラスのポインタを持つことができる。
*/
class Player;

class Enemy 
{
public:// メンバ関数

	~Enemy();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& appvelocity, const Vector3& leavelocity);

	void Update();

	void Fire();

	void Draw(const ViewProjection& viewProjection);

	// 接近フェーズ初期化
	void Approach();

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 行動フェーズ
	/*
	状態遷移の実装にもいろいろな方法があるが共通して行うこととして
	enum の定義がある。

	いくつかある行動フェーズのうち、今どれなのかを1つの変数で持つと
	一気に状態がわかり安くなる。

	そのために都合の良い型として enum を新たに定義する。
	*/
	enum class Phase
	{
		Approach,// 接近する
		Leave,// 離脱する
	};

	// 発射間隔
	static const int kFireInterval = 60;

	
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

private:// メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 速度
	Vector3 velocity_;
	Vector3 approachVelocity_;// 接近フェーズ
	Vector3 leaveVelocity_;// 離脱フェーズ

	// フェーズ
	/*
	enum 型の変数を持たせる。

	メンバ変数として持たせることで、Enemy のインスタンスが生きている間
	そのインスタンス(つまり敵一体)についての全ての関数から、フェーズを
	読み書きできる。

	またフェーズの初期値(初期フェーズ)も設定しておく。
	単に Approach と指定するのではなく、どの列挙型に属するのかを指定する。
	*/
	Phase phase_ = Phase::Approach;

	
	// 発射タイマー
	int32_t ShotTimer = 0;

	// 自キャラ
	Player* player_ = nullptr;

	// デスフラグ
	bool isDead_ = false;
};
