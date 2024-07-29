#pragma once
#include"WorldTransform.h"
#include "ViewProjection.h"
#include"Player.h"

class RailCamera 
{
public:
	void Initialize(const Vector3& railCameraPosition, const Vector3& railCameraRotate);

	void Update();

	// ビュープロジェクションの取得関数
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	bool isGoal() { return isGoal_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 速度
	Vector3 velocity_ = {0, 0, 0.05f};

	// 回転
	Vector3 rotate_ = {0, 0, 0};

	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};

	// ゴールしたかどうか
	bool isGoal_ = false;
};
