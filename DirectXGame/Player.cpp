#include "Player.h"
#include"cassert"
#include"ImGuiManager.h"


// 関数の定義

// 座標移動（ベクトルの加算）
Vector3 Add(const Vector3& translation, const Vector3& move) {

	Vector3 result{};
	result.x = translation.x + move.x; 
	result.y = translation.y + move.y; 
	result.z = translation.z + move.z; 

	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 subtract{};
	subtract.x = v1.x - v2.x;
	subtract.y = v1.y - v2.y;
	subtract.z = v1.z - v2.z;
	return subtract;
}

// 乗算
Vector3 Multiply(float k, const Vector3& v1) {
	Vector3 multiply{};
	multiply.x = k * v1.x;
	multiply.y = k * v1.y;
	multiply.z = k * v1.z;
	return multiply;
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
Matrix4x4 MakeRotateXMatrix(float rotate) {
	Matrix4x4 result{};

	result.m[1][1] = std::cos(rotate);
	result.m[1][2] = std::sin(rotate);
	result.m[2][1] = -std::sin(rotate);
	result.m[2][2] = std::cos(rotate);
	result.m[0][0] = 1;
	result.m[3][3] = 1;

	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float rotate) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(rotate);
	result.m[0][2] = -std::sin(rotate);
	result.m[1][1] = 1;
	result.m[2][0] = std::sin(rotate);
	result.m[2][2] = std::cos(rotate);
	result.m[3][3] = 1;

	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float rotate) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(rotate);
	result.m[0][1] = std::sin(rotate);
	result.m[1][0] = -std::sin(rotate);
	result.m[1][1] = std::cos(rotate);
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

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result{};

	result.m[0][0] = width / 2;
	result.m[1][1] = -height / 2;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;

	return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m1) {
	Matrix4x4 result{};
	// int A = {};//絶対値はint型にする
	float abs; // 絶対値はint型にする

	// |A|
	abs = (m1.m[0][0] * m1.m[1][1] * m1.m[2][2] * m1.m[3][3]) + (m1.m[0][0] * m1.m[1][2] * m1.m[2][3] * m1.m[3][1]) + (m1.m[0][0] * m1.m[1][3] * m1.m[2][1] * m1.m[3][2]) -
	      (m1.m[0][0] * m1.m[1][3] * m1.m[2][2] * m1.m[3][1]) - (m1.m[0][0] * m1.m[1][2] * m1.m[2][1] * m1.m[3][3]) - (m1.m[0][0] * m1.m[1][1] * m1.m[2][3] * m1.m[3][2]) -
	      (m1.m[0][1] * m1.m[1][0] * m1.m[2][2] * m1.m[3][3]) - (m1.m[0][2] * m1.m[1][0] * m1.m[2][3] * m1.m[3][1]) - (m1.m[0][3] * m1.m[1][0] * m1.m[2][1] * m1.m[3][2]) +
	      (m1.m[0][3] * m1.m[1][0] * m1.m[2][2] * m1.m[3][1]) + (m1.m[0][2] * m1.m[1][0] * m1.m[2][1] * m1.m[3][3]) + (m1.m[0][1] * m1.m[1][0] * m1.m[2][3] * m1.m[3][2]) +
	      (m1.m[0][1] * m1.m[1][2] * m1.m[2][0] * m1.m[3][3]) + (m1.m[0][2] * m1.m[1][3] * m1.m[2][0] * m1.m[3][1]) + (m1.m[0][3] * m1.m[1][1] * m1.m[2][0] * m1.m[3][2]) -
	      (m1.m[0][3] * m1.m[1][2] * m1.m[2][0] * m1.m[3][1]) - (m1.m[0][2] * m1.m[1][1] * m1.m[2][0] * m1.m[3][3]) - (m1.m[0][1] * m1.m[1][3] * m1.m[2][0] * m1.m[3][2]) -
	      (m1.m[0][1] * m1.m[1][2] * m1.m[2][3] * m1.m[3][0]) - (m1.m[0][2] * m1.m[1][3] * m1.m[2][1] * m1.m[3][0]) - (m1.m[0][3] * m1.m[1][1] * m1.m[2][2] * m1.m[3][0]) +
	      (m1.m[0][3] * m1.m[1][2] * m1.m[2][1] * m1.m[3][0]) + (m1.m[0][2] * m1.m[1][1] * m1.m[2][3] * m1.m[3][0]) + (m1.m[0][1] * m1.m[1][3] * m1.m[2][2] * m1.m[3][0]);

	// 1/A
	result.m[0][0] = 1.0f / abs *
	                 ((m1.m[1][1] * m1.m[2][2] * m1.m[3][3]) + (m1.m[1][2] * m1.m[2][3] * m1.m[3][1]) + (m1.m[1][3] * m1.m[2][1] * m1.m[3][2]) - (m1.m[1][3] * m1.m[2][2] * m1.m[3][1]) -
	                  (m1.m[1][2] * m1.m[2][1] * m1.m[3][3]) - (m1.m[1][1] * m1.m[2][3] * m1.m[3][2]));
	result.m[0][1] = 1.0f / abs *
	                 (-(m1.m[0][1] * m1.m[2][2] * m1.m[3][3]) - (m1.m[0][2] * m1.m[2][3] * m1.m[3][1]) - (m1.m[0][3] * m1.m[2][1] * m1.m[3][2]) + m1.m[0][3] * m1.m[2][2] * m1.m[3][1] +
	                  m1.m[0][2] * m1.m[2][1] * m1.m[3][3] + m1.m[0][1] * m1.m[2][3] * m1.m[3][2]);
	result.m[0][2] = 1.0f / abs *
	                 ((m1.m[0][1] * m1.m[1][2] * m1.m[3][3]) + (m1.m[0][2] * m1.m[1][3] * m1.m[3][1]) + (m1.m[0][3] * m1.m[1][1] * m1.m[3][2]) - (m1.m[0][3] * m1.m[1][2] * m1.m[3][1]) -
	                  (m1.m[0][2] * m1.m[1][1] * m1.m[3][3]) - (m1.m[0][1] * m1.m[1][3] * m1.m[3][2]));
	result.m[0][3] = 1.0f / abs *
	                 (-(m1.m[0][1] * m1.m[1][2] * m1.m[2][3]) - (m1.m[0][2] * m1.m[1][3] * m1.m[2][1]) - (m1.m[0][3] * m1.m[1][1] * m1.m[2][2]) + (m1.m[0][3] * m1.m[1][2] * m1.m[2][1]) +
	                  (m1.m[0][2] * m1.m[1][1] * m1.m[2][3]) + (m1.m[0][1] * m1.m[1][3] * m1.m[2][2]));

	result.m[1][0] = 1.0f / abs *
	                 (-(m1.m[1][0] * m1.m[2][2] * m1.m[3][3]) - (m1.m[1][2] * m1.m[2][3] * m1.m[3][0]) - (m1.m[1][3] * m1.m[2][0] * m1.m[3][2]) + (m1.m[1][3] * m1.m[2][2] * m1.m[3][0]) +
	                  (m1.m[1][2] * m1.m[2][0] * m1.m[3][3]) + (m1.m[1][0] * m1.m[2][3] * m1.m[3][2]));
	result.m[1][1] = 1.0f / abs *
	                 ((m1.m[0][0] * m1.m[2][2] * m1.m[3][3]) + (m1.m[0][2] * m1.m[2][3] * m1.m[3][0]) + (m1.m[0][3] * m1.m[2][0] * m1.m[3][2]) - (m1.m[0][3] * m1.m[2][2] * m1.m[3][0]) -
	                  (m1.m[0][2] * m1.m[2][0] * m1.m[3][3]) - (m1.m[0][0] * m1.m[2][3] * m1.m[3][2]));
	result.m[1][2] = 1.0f / abs *
	                 (-(m1.m[0][0] * m1.m[1][2] * m1.m[3][3]) - (m1.m[0][2] * m1.m[1][3] * m1.m[3][0]) - (m1.m[0][3] * m1.m[1][0] * m1.m[3][2]) + (m1.m[0][3] * m1.m[1][2] * m1.m[3][0]) +
	                  (m1.m[0][2] * m1.m[1][0] * m1.m[3][3]) + (m1.m[0][0] * m1.m[1][3] * m1.m[3][2]));
	result.m[1][3] = 1.0f / abs *
	                 ((m1.m[0][0] * m1.m[1][2] * m1.m[2][3]) + (m1.m[0][2] * m1.m[1][3] * m1.m[2][0]) + (m1.m[0][3] * m1.m[1][0] * m1.m[2][2]) - (m1.m[0][3] * m1.m[1][2] * m1.m[2][0]) -
	                  (m1.m[0][2] * m1.m[1][0] * m1.m[2][3]) - (m1.m[0][0] * m1.m[1][3] * m1.m[2][2]));

	result.m[2][0] = 1.0f / abs *
	                 ((m1.m[1][0] * m1.m[2][1] * m1.m[3][3]) + (m1.m[1][1] * m1.m[2][3] * m1.m[3][0]) + (m1.m[1][3] * m1.m[2][0] * m1.m[3][1]) - (m1.m[1][3] * m1.m[2][1] * m1.m[3][0]) -
	                  (m1.m[1][1] * m1.m[2][0] * m1.m[3][3]) - (m1.m[1][0] * m1.m[2][3] * m1.m[3][1]));
	result.m[2][1] = 1.0f / abs *
	                 (-(m1.m[0][0] * m1.m[2][1] * m1.m[3][3]) - (m1.m[0][1] * m1.m[2][3] * m1.m[3][0]) - (m1.m[0][3] * m1.m[2][0] * m1.m[3][1]) + (m1.m[0][3] * m1.m[2][1] * m1.m[3][0]) +
	                  (m1.m[0][1] * m1.m[2][0] * m1.m[3][3]) + (m1.m[0][0] * m1.m[2][3] * m1.m[3][1]));
	result.m[2][2] = 1.0f / abs *
	                 ((m1.m[0][0] * m1.m[1][1] * m1.m[3][3]) + (m1.m[0][1] * m1.m[1][3] * m1.m[3][0]) + (m1.m[0][3] * m1.m[1][0] * m1.m[3][1]) - (m1.m[0][3] * m1.m[1][1] * m1.m[3][0]) -
	                  (m1.m[0][1] * m1.m[1][0] * m1.m[3][3]) - (m1.m[0][0] * m1.m[1][3] * m1.m[3][1]));
	result.m[2][3] = 1.0f / abs *
	                 (-(m1.m[0][0] * m1.m[1][1] * m1.m[2][3]) - (m1.m[0][1] * m1.m[1][3] * m1.m[2][0]) - (m1.m[0][3] * m1.m[1][0] * m1.m[2][1]) + (m1.m[0][3] * m1.m[1][1] * m1.m[2][0]) +
	                  (m1.m[0][1] * m1.m[1][0] * m1.m[2][3]) + (m1.m[0][0] * m1.m[1][3] * m1.m[2][1]));

	result.m[3][0] = 1.0f / abs *
	                 (-(m1.m[1][0] * m1.m[2][1] * m1.m[3][2]) - (m1.m[1][1] * m1.m[2][2] * m1.m[3][0]) - (m1.m[1][2] * m1.m[2][0] * m1.m[3][1]) + (m1.m[1][2] * m1.m[2][1] * m1.m[3][0]) +
	                  (m1.m[1][1] * m1.m[2][0] * m1.m[3][2]) + (m1.m[1][0] * m1.m[2][2] * m1.m[3][1]));
	result.m[3][1] = 1.0f / abs *
	                 ((m1.m[0][0] * m1.m[2][1] * m1.m[3][2]) + (m1.m[0][1] * m1.m[2][2] * m1.m[3][0]) + (m1.m[0][2] * m1.m[2][0] * m1.m[3][1]) - (m1.m[0][2] * m1.m[2][1] * m1.m[3][0]) -
	                  (m1.m[0][1] * m1.m[2][0] * m1.m[3][2]) - (m1.m[0][0] * m1.m[2][2] * m1.m[3][1]));
	result.m[3][2] = 1.0f / abs *
	                 (-(m1.m[0][0] * m1.m[1][1] * m1.m[3][2]) - (m1.m[0][1] * m1.m[1][2] * m1.m[3][0]) - (m1.m[0][2] * m1.m[1][0] * m1.m[3][1]) + (m1.m[0][2] * m1.m[1][1] * m1.m[3][0]) +
	                  (m1.m[0][1] * m1.m[1][0] * m1.m[3][2]) + (m1.m[0][0] * m1.m[1][2] * m1.m[3][1]));
	result.m[3][3] = 1.0f / abs *
	                 ((m1.m[0][0] * m1.m[1][1] * m1.m[2][2]) + (m1.m[0][1] * m1.m[1][2] * m1.m[2][0]) + (m1.m[0][2] * m1.m[1][0] * m1.m[2][1]) - (m1.m[0][2] * m1.m[1][1] * m1.m[2][0]) -
	                  (m1.m[0][1] * m1.m[1][0] * m1.m[2][2]) - (m1.m[0][0] * m1.m[1][2] * m1.m[2][1]));

	// abs_A = abs(A);//絶対値にする

	return result;
}

// 正規化
Vector3 Normalize(const Vector3& v2) {
	Vector3 normalize{};
	normalize.x = v2.x / sqrtf(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z), normalize.y = v2.y / sqrtf(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z),
	normalize.z = v2.z / sqrtf(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
	return normalize;
}

// 座標変換
Vector3 Transform(const Vector3& point, const Matrix4x4& transformMatrix) {
	Vector3 result{};

	result.x = point.x * transformMatrix.m[0][0] + point.y * transformMatrix.m[1][0] + point.z * transformMatrix.m[2][0] + 1.0f * transformMatrix.m[3][0];
	result.y = point.x * transformMatrix.m[0][1] + point.y * transformMatrix.m[1][1] + point.z * transformMatrix.m[2][1] + 1.0f * transformMatrix.m[3][1];
	result.z = point.x * transformMatrix.m[0][2] + point.y * transformMatrix.m[1][2] + point.z * transformMatrix.m[2][2] + 1.0f * transformMatrix.m[3][2];
	float w = point.x * transformMatrix.m[0][3] + point.y * transformMatrix.m[1][3] + point.z * transformMatrix.m[2][3] + 1.0f * transformMatrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& S, const Vector3& R, const Vector3& T) 
{
	Matrix4x4 result{};

	// 回転行列
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(R.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(R.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(R.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	result.m[0][0] = S.x * rotateXYZMatrix.m[0][0];
	result.m[0][1] = S.x * rotateXYZMatrix.m[0][1];
	result.m[0][2] = S.x * rotateXYZMatrix.m[0][2];
	result.m[1][0] = S.y * rotateXYZMatrix.m[1][0];
	result.m[1][1] = S.y * rotateXYZMatrix.m[1][1];
	result.m[1][2] = S.y * rotateXYZMatrix.m[1][2];
	result.m[2][0] = S.z * rotateXYZMatrix.m[2][0];
	result.m[2][1] = S.z * rotateXYZMatrix.m[2][1];
	result.m[2][2] = S.z * rotateXYZMatrix.m[2][2];
	result.m[3][0] = T.x;
	result.m[3][1] = T.y;
	result.m[3][2] = T.z;
	result.m[3][3] = 1;

	return result;
}

// ベクトル変換
/*
3x3行列とベクトルの掛け算をすることで、平行移動を無視して
スケーリングと回転のみを適用する。
*/
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m)
{
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}


Player::~Player() 
{ 
	// 弾のポインタが１つの時は１回deleteすればよかったが、
	// 複数になったのでfor文で回して全てのポインタを開放する必要がある。
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}


void Player::Initialize(Model* model, uint32_t textureHandle,const Vector3& playerPosition) {

	// NULLポインタチェック
	assert(model);

	// レティクル用テクスチャ取得
	textureReticle_ = TextureManager::Load("Reticle.png");
	// レティクルスプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle_, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f});

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = playerPosition;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	// シングルインスタンスを取得する
	input_ = Input::GetInstance();
}



void Player::Update(const ViewProjection& viewProjection) 
{
	//=====================レティクル========================//
	
	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
	worldTransform3DReticle_.UpdateMatrix();
	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	//positionReticle = GetWorld3DReticlePosition();
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// ワールド➝スクリーン座標変換（ここで3Dから2Dになる）
	//positionReticle = Transform(positionReticle, matViewProjectionViewport);
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));


	//===============マウス座標にレティクル表示=================//

	POINT mousePosition;
	// マウス座標（スクリーン座標）を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	// マウス座標（スクリーン座標）を取得する
	sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);
 
	// スクリーン座標
	Vector3 posNear = Vector3((float)mousePosition.x, (float)mousePosition.y, 0);
	Vector3 posFar = Vector3((float)mousePosition.x, (float)mousePosition.y, 1);

	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 70.0f;
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply(kDistanceTestObject, mouseDirection));

	// デバッグ文字表示
	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)", positionReticle.x, positionReticle.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2)f", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();



	//===============プレイヤー移動================//

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;


	//=====ゲームパッド=====//

	// ゲームパッドの状態を得る変数（XINPUT）
	//XINPUT_STATE joyState;

	//// ゲームパッド状態取得
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) 
	//{
	//	move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
	//	move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	//}

	// スプライトの現在座標を取得
	//Vector2 spritePosition = sprite2DReticle_->GetPosition();


	// ジョイスティック状態取得
	//if (Input::GetInstance()->GetJoystickState(0, joyState))
	//{
	//	positionReticle.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
	//	positionReticle.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

	//	// スプライトの座標変更を反映
	//	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	//}



	//=====キーボード=====//

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
	Matrix4x4 rotationXMatrix = MakeRotateXMatrix(worldTransform_.rotation_.x);
	Matrix4x4 rotationYMatrix = MakeRotateYMatrix(worldTransform_.rotation_.y);
	Matrix4x4 rotationZMatrix = MakeRotateZMatrix(worldTransform_.rotation_.z);
	Matrix4x4 rotationXYZMatrix = Multiply(rotationXMatrix, Multiply(rotationYMatrix, rotationZMatrix));
	// 平行、スケーリング、回転行列を合成
	Matrix4x4 matWorldMatrix = Multiply(scaleMatrix, Multiply(rotationXYZMatrix, translateMatrix));

	// 行列更新
	worldTransform_.UpdateMatrix();



	// キャラクターの座標を画面表示する処理
	ImGui::Begin("PlayerPos");
	ImGui::Text("Player %.02f,%.02f,%.02f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();

	// キャラクター旋回処理
	Rotate();

	// キャラクター攻撃処理
	Attack();


	// 弾更新
	// for文でリスト内のすべてのPlayerBulletについて1個1個処理していく。
	for (PlayerBullet* bullet : bullets_)// 範囲for文。コンテナや配列を簡潔に扱うためのfor文の別表現。
	{
		// PlayerBulletのポインタのリストからPlayerBulletのポインタを1個づつ
		// 取り出しながら処理を回していく。
		// PlayerBullet* がリスト内の要素1個分の型。
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	/*
	死んだ弾を削除する処理。
	std::list のメンバ関数である remove_if() は、条件に当てはまる要素を
	リストから排除する関数。
	条件判定用の関数を渡す必要があるのだが、このように true か false を返す
	ラムダ式を指定すると便利。

	true を返した弾は list から取り除かれる。
	削除条件を満たした場合に delete した上で true を返すことで、インスタンスの
	解放と list からの除外を両立させる。
	*/
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}



// 旋回する
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
	//XINPUT_STATE joyState;

	//// ゲームパッド未接続なら何もせず抜ける
	//if (!Input::GetInstance()->GetJoystickState(0, joyState))
	//{
	//	return;
	//}

	//// Rトリガーを押していたら
	//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	//{
	//	// 弾の速度
	//	/*
	//	弾の速度ベクトル(1frmの移動量)を設定する。
	//	この場合は1frmにつきZ方向に1.0f進む設定。
	//	*/
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	// 自機から照準オブジェクトへのベクトル
	//	velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
	//	velocity = Multiply(kBulletSpeed, Normalize(velocity));

	//	// 弾を生成し、初期化
	//	PlayerBullet* newBullet = new PlayerBullet();
	//	newBullet->Initialize(model_, GetWorldPosition(), velocity);

	//	// 弾を登録する
	//	bullets_.push_back(newBullet);
	//}


	if (input_->TriggerKey(DIK_SPACE)) 
	{
		// 弾の速度
		/*
		弾の速度ベクトル(1frmの移動量)を設定する。
		この場合は1frmにつきZ方向に1.0f進む設定。
		*/
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));


		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

// ワールド変換
Vector3 Player::GetWorldPosition() {
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// ワールド変換
Vector3 Player::GetWorld3DReticlePosition() {
	Vector3 worldPos{};

	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}


// 衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision() { 
	isDead_ = true; 
	isFinished_ = true;
};

// 親子関係を結ぶ
void Player::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}


// レティクル2D描画
void Player::DrawUI() { sprite2DReticle_->Draw(); }


void Player::Draw(ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 3Dレティクルを描画
	//model_->Draw(worldTransform3DReticle_, viewProjection, textureReticle_);

	// 弾描画
	// 全部の弾の描画を呼び出す。
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

}