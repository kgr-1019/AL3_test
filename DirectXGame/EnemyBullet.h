#pragma once
#include"Model.h"
#include"Vector3.h"
#include "WorldTransform.h"
class EnemyBullet 
{
public:// メンバ関数
	
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Fire();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:// メンバ変数

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
