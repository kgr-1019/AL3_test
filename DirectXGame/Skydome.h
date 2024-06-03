#pragma once
#include"Model.h"
#include"Vector3.h"
#include "WorldTransform.h"

class Skydome 
{
public:
	void Initialize(Model* modelSkydome, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0;
};
