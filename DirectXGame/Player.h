#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Vector3.h"
class Player {

public:// メンバ関数


	/// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	
	/// 更新
	void Update();

	/// 描画
	void Draw(ViewProjection& viewProjection);

private:// メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;


};
