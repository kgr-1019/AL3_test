#pragma once
#include"WorldTransform.h"
#include "ViewProjection.h"
#include"Player.h"
#include"imgui.h"
class RailCamera 
{
public:
	void Initialize(WorldTransform& worldTransform);

	void Update();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 速度
	Vector3 velocity_ = {0, 0, 5};

	// 回転
	Vector3 rotate_ = {3, 0, 0};

	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};
};
