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
Matrix4x4 MakeRotateYMatrix(const Vector3& rotate) {
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
Matrix4x4 MakeRotateZMatrix(const Vector3& rotate) {
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
Matrix4x4 MakeAffineMatrix(const Vector3& S, const Vector3& R, const Vector3& T) 
{
	Matrix4x4 result{};

	result.m[0][0] = S.x * R.m[0][0];
	result.m[0][1] = S.x * R.m[0][1];
	result.m[0][2] = S.x * R.m[0][2];
	result.m[1][0] = S.y * R.m[1][0];
	result.m[1][1] = S.y * R.m[1][1];
	result.m[1][2] = S.y * R.m[1][2];
	result.m[2][0] = S.z * R.m[2][0];
	result.m[2][1] = S.z * R.m[2][1];
	result.m[2][2] = S.z * R.m[2][2];
	result.m[3][0] = T.x;
	result.m[3][1] = T.y;
	result.m[3][2] = T.z;
	result.m[3][3] = 1;

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

		move.y += kCharacterSpeed;

	} else if (input_->PushKey(DIK_DOWN)) {

		move.y -= kCharacterSpeed;
	}

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 平行移動行列
	Matrix4x4 translateMatrix = MakeTranslateMatrix(worldTransform_.translation_);
	// スケーリング行列
	Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);
	// 回転行列
	Matrix4x4 rotationXMatrix = MakeRotateXMatrix(worldTransform_.rotation_);
	Matrix4x4 rotationYMatrix = MakeRotateYMatrix(worldTransform_.rotation_);
	Matrix4x4 rotationZMatrix = MakeRotateZMatrix(worldTransform_.rotation_);
	Matrix4x4 rotationXYZMatrix = Multiply(rotationXMatrix, Multiply(rotationYMatrix, rotationZMatrix));
	// 平行、スケーリング、回転行列を合成
	Matrix4x4 matWorldMatrix = Multiply(scaleMatrix, Multiply(rotationXYZMatrix, translateMatrix));

	// 行列更新
	worldTransform_.matWorld_ = matWorldMatrix;

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("PlayerPos");
	ImGui::Text("Player %.02f,%.02f,%.02f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();

	// キャラクター旋回処理
	Rotate();

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	if (bullet_)
	{
		bullet_->Update();
	}
}


void Player::Rotate()
{
	// 回転の速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f; 

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A))
	{
		worldTransform_.rotation_.y -= kRotSpeed;

	} else if (input_->PushKey(DIK_D)) {

		worldTransform_.rotation_.y += kRotSpeed;
	}
}


void Player::Attack() 
{
	if (input_->PushKey(DIK_SPACE)) 
	{
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		// 弾を登録する
		bullet_ = newBullet;
	}
}


void Player::Draw(ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	if (bullet_)
	{
		bullet_->Draw(viewProjection);
	}

}