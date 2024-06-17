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
#include"Skydome.h"
#include"RailCamera.h"
#include<list>
#include<sstream>

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

	void CheckAllCollisions();

	// 敵の弾を登録する関数
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	// 敵発生データの読み込み
	void LoadEnemyPopData();

	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	// 敵発生
	void OccurrenceEnemy(Vector3 position);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;

	DebugCamera* debugCamera_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;
	float playerRad_ = 1.0f;
	Vector3 playerPosition = {0, 0, 50.0f};

	// 自弾半径
	float playerBulletRad_ = 1.0f;

	// 敵弾半径
	float enemyBulletRad_ = 1.0f;

	// 敵
	float enemyRad_ = 1.0f;
	std::list<Enemy*> enemies_;// 敵リスト
	std::stringstream enemyPopCommands;// 敵発生コマンド
	Vector3 position_ = {5, 2, 80};// 初期位置
	Vector3 velocity_ = {0, 0, -0.05f};// 移動速度
	Vector3 approachVelocity_ = {0, 0, -0.1f};// 接近フェーズ速度
	Vector3 leaveVelocity_ = {-0.1f, 0.1f, 0};// 離脱フェーズ速度
	std::list<EnemyBullet*> enemyBullets_;// リスト


	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// レティクル用テクスチャ取得
	uint32_t textureReticle = 0;


	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// Skydome
	Skydome* skydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;
	Vector3 railCameraPosition = {0.0f, 0.0f, -100.0f};
	Vector3 railCameraRotate = {0.0f, 0.0f, 0.0f};

	// 待機中フラグ
	bool isWait = false;
	// 待機タイマー
	uint32_t waitTimer = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
