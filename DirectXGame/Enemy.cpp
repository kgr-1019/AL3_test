#include "Enemy.h"
#include "TextureManager.h"
#include"Player.h"
#include"cassert"
void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("tex1.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ関数に移し替える
	velocity_ = velocity;
}

void Enemy::Update()
{
	//移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 　行列を更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
