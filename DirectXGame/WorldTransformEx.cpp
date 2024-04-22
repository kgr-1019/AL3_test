#include"WorldTransform.h"
#include<cmath>

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


void WorldTransform::UpdateMatrix()
{
	// 平行移動行列
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translation_);
	// スケーリング行列
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale_);
	// 回転行列
	Matrix4x4 rotationXMatrix = MakeRotateXMatrix(rotation_);
	Matrix4x4 rotationYMatrix = MakeRotateYMatrix(rotation_);
	Matrix4x4 rotationZMatrix = MakeRotateZMatrix(rotation_);
	Matrix4x4 rotationMatrix = Multiply(rotationXMatrix, Multiply(rotationYMatrix, rotationZMatrix));
	// 平行、スケーリング、回転行列を合成
	matWorld_ = MakeAffineMatrix(scaleMatrix, rotationMatrix, translateMatrix);

	// 定数バッファに転送する
	TransferMatrix();

	
}
