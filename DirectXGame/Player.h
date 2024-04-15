#pragma once
#include"Model.h"
#include"WorldTransform.h"
class Player {

public:// メンバ関数

	// ワールド変換データ
	WorldTransform worldTransform_;



	/// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	

	/// 更新
	void Update();

	/// 描画
	void Draw(ViewProjection& viewProjection);

private:// メンバ変数

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
