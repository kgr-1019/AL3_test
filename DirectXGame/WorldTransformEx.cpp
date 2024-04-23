#include"WorldTransform.h"
#include"Player.h"



void WorldTransform::UpdateMatrix()
{
	// 平行、スケーリング、回転行列を合成
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 定数バッファに転送する
	TransferMatrix();
}
