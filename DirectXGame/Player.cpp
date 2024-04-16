#include "Player.h"
#include"cassert"



void Player::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// シングルインスタンスを取得する
	input_ = Input::GetInstance();
}


void Player::Update() {

	// スケーリング行列の作成

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {

		move.x -= kCharacterSpeed;

	} else if (input_->PushKey(DIK_RIGHT)) {

		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP)) {

		move.y -= kCharacterSpeed;

	} else if (input_->PushKey(DIK_DOWN)) {

		move.y += kCharacterSpeed;
	}

	// 座標移動（ベクトルの加算）
	Vector3 Add(const WorldTransform& worldTransform_, const Vector3& move) {
	
		worldTransform_.translation_ += move;
	}
}


void Player::Draw(ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}