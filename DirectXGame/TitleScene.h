#pragma once
#include"Input.h"
#include"Sprite.h"
#include"DirectXCommon.h"

class TitleScene {

public:
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:

	bool finished_ = false;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	Sprite* sprite_ = nullptr;
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

};
