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

// スカラー倍
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



void Player::Update() 
{
	
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
	Matrix4x4 rotationXMatrix = MakeRotateXMatrix(worldTransform_.rotation_.x);
	Matrix4x4 rotationYMatrix = MakeRotateYMatrix(worldTransform_.rotation_.y);
	Matrix4x4 rotationZMatrix = MakeRotateZMatrix(worldTransform_.rotation_.z);
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
	if (input_->PushKey(DIK_SPACE)) 
	{
		// 弾の速度
		/*
		弾の速度ベクトル(1frmの移動量)を設定する。
		この場合は1frmにつきZ方向に1.0f進む設定。
		*/
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		/*
		開店前の、Z方向向きのベクトルに自キャラの向きと同じ回転をかけることで、
		自キャラと同じ向きのベクトルになる。
		この計算を発射する瞬間の一度だけ行うことで、発射後は最初の向きを保ったまま
		同じ角度で進む。
		*/
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);


		// 弾があれば開放する
		/*if (bullet_)
		{
			delete bullet_;
			bullet_ = nullptr;
		}*/
		// ↑ポインタが複数持てて、前の弾を指すポインタを残しておけるから、
		// 　メモリリークの心配がなくなるので解放する必要がなくなる。


		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

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

// 衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision() {};

void Player::Draw(ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	// 全部の弾の描画を呼び出す。
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

}