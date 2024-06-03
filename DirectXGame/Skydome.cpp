#include "Skydome.h"
#include"cassert"
void Skydome::Initialize(Model* modelSkydome, uint32_t textureHandle)
{
	// NULLポインタチェック
	assert(modelSkydome);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = modelSkydome;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update()
{

}

void Skydome::Draw(ViewProjection& viewProjection)
{ model_->Draw(worldTransform_,viewProjection); }