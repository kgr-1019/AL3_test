#pragma once
#include"Model.h"
#include"Vector3.h"
#include "WorldTransform.h"


class PlayerBullet 
{
public:

	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	/*
	メンバ変数である isDead_ は privateなので、外部から参照するために
	public な getter を作る。

	getter のように非常に短い関数はインライン関数としてヘッダーファイルに
	処理を直接記述することが多い。

	関数名の後ろの const は、「このメンバ関数はメンバ変数を書き換えませんよ」
	という指定。
	*/
	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 速度
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;

	// デスフラグ
	bool isDead_ = false;
};
