#include "Player.h"
#include"cassert"
#include"ImGuiManager.h"


// 関数のプロトタイプ宣言
Vector3 Add(const Vector3& translation, const Vector3& move);
	

// 関数の定義

// 座標移動（ベクトルの加算）
Vector3 Add(const Vector3& translation, const Vector3& move) {

	Vector3 result{};
	result.x = translation.x + move.x; 
	result.y = translation.y + move.y; 
	result.z = translation.z + move.z; 

	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result{};

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(const Vector3& rotate) {
	Matrix4x4 result{};

	result.m[1][1] = std::cos(rotate.x);
	result.m[1][2] = std::sin(rotate.x);
	result.m[2][1] = -std::sin(rotate.x);
	result.m[2][2] = std::cos(rotate.x);
	result.m[0][0] = 1;
	result.m[3][3] = 1;

	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(Vector3 rotate) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(rotate.y);
	result.m[0][2] = -std::sin(rotate.y);
	result.m[1][1] = 1;
	result.m[2][0] = std::sin(rotate.y);
	result.m[2][2] = std::cos(rotate.y);
	result.m[3][3] = 1;

	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(Vector3 rotate) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(rotate.z);
	result.m[0][1] = std::sin(rotate.z);
	result.m[1][0] = -std::sin(rotate.z);
	result.m[1][1] = std::cos(rotate.z);
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	return result;
}

// XYZ合成
Matrix4x4 Multiply(const Matrix4x4& rotateX, const Matrix4x4& rotateYZ) {
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += rotateX.m[i][k] * rotateYZ.m[k][j];
			}
		}
	}

	return result;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) {
		result.m[i][i] = 1;
	}
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Matrix4x4& S, const Matrix4x4& R, const Matrix4x4& T) {
	Matrix4x4 SR{};
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				SR.m[i][j] += S.m[i][k] * R.m[k][j];
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += SR.m[i][k] * T.m[k][j];
			}
		}
	}

	return result;
}


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


	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	/*worldTransform_.translation_ = {0, 0, 0};
	worldTransform_.rotation_ = {0, 0, 0};
	worldTransform_.scale_ = {1, 1, 1};*/

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

	// 移動限界座標
	const float kMoveLimitX = 100;
	const float kMoveLimitY = 100;

	// 範囲を超えない処理
	move.x = max(move.x, -kMoveLimitX);
	move.x = min(move.x, +kMoveLimitX);
	move.y = max(move.y, -kMoveLimitY);
	move.y = min(move.y, +kMoveLimitY);


	// キャラクターの座標を画面表示する処理
	ImGui::Begin("");
	ImGui::Text("Player %d,%d,%d", move.x, move.y, move.z);
	ImGui::End();



	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	Matrix4x4 rotationX = MakeRotateXMatrix(worldTransform_.rotation_.x);
	Matrix4x4 rotationY = MakeRotateYMatrix(worldTransform_.rotation_.y);
	Matrix4x4 rotationZ = MakeRotateZMatrix(worldTransform_.rotation_.z);
	Matrix4x4 worldTransform_.rotation_ = Multiply(rotationX, Multiply(rotationY, rotationZ));
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}


void Player::Draw(ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}