#pragma once
#include"Model.h"
#include"Vector3.h"
#include "WorldTransform.h"


class PlayerBullet 
{
public:

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
};
