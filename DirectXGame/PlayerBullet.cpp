#include "PlayerBullet.h"
#include"cassert"
#include "TextureManager.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) 
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
};

void PlayerBullet::Update() 
{ 
	worldTransform_->UpdateMatrix(); 
};

void PlayerBullet::Draw(const ViewProjection& viewProjection){

};
