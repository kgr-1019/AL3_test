#pragma once
#include"Model.h"
#include"Vector3.h"
#include "WorldTransform.h"
class Enemy 
{
public:// メンバ関数

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:// メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 速度
	Vector3 velocity_;

};
