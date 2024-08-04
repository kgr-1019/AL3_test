#pragma once
#include"WorldTransform.h"
#include "ViewProjection.h"
#include"Player.h"

class RailCamera 
{
public:
	void Initialize(const Vector3& railCameraPosition, const Vector3& railCameraRotate);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void DrawUI();

	// ビュープロジェクションの取得関数
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	bool isGoal() { return isGoal_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// モデル
	Model* model_ = nullptr;

	// 速度
	Vector3 velocity_ = {0, 0, 0.05f};

	// 回転
	Vector3 rotate_ = {0, 0, 0};

	// テクスチャハンドル
	uint32_t textureClear_ = 0;
	uint32_t textureSusume_ = 0;
	// スプライト
	Sprite* spriteClear_ = nullptr;
	Sprite* spriteSusume_ = nullptr;


	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};

	// ゴールしたかどうか
	bool isGoal_ = false;
	bool isGoalCount = false;
	int goalCount = 0;
};
