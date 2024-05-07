#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Player.h"
#include"DebugCamera.h"
#include"Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数


	// ビュープロジェクション
	ViewProjection viewProjection_;

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* model_ = nullptr;

	DebugCamera* debugCamera_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	// 敵
	Enemy* enemy_ = nullptr;
	Vector3 position_ = {0, 2, 20};// 初期位置
	Vector3 velocity_ = {0, 0, -0.1f};// 移動速度
	Vector3 approachVelocity_ = {0, 0, -0.1f};// 接近フェーズ速度
	Vector3 leaveVelocity_ = {-0.1f, 0.1f, 0};// 離脱フェーズ速度

	uint32_t textureHandle_ = 0;// テクスチャハンドル

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
