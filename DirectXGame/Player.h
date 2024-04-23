#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Vector3.h"
#include"PlayerBullet.h"

Vector3 Add(const Vector3& translation, const Vector3& move);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Matrix4x4 MakeRotateXMatrix(const Vector3& rotate);
Matrix4x4 MakeRotateYMatrix(const Vector3& rotate);
Matrix4x4 MakeRotateZMatrix(const Vector3& rotate);
Matrix4x4 Multiply(const Matrix4x4& rotateX, const Matrix4x4& rotateYZ);
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeAffineMatrix(const Vector3& S, const Matrix4x4& R, const Vector3& T);

class Player {

public:// メンバ関数


	/// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	
	/// 更新
	void Update();

	/// 旋回(回転)
	void Rotate();

	/// 攻撃
	void Attack();

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

	PlayerBullet* bullet_ = nullptr;


};
