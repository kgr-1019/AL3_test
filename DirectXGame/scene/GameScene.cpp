#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;// 3Dモデルデータの解放
	delete player_;// 自キャラの解放
	delete debugCamera_;// デバッグカメラ
	delete enemy_;// 敵
	delete modelSkydome_;// Skydome
	// 弾のポインタが１つの時は１回deleteすればよかったが、
	// 複数になったのでfor文で回して全てのポインタを開放する必要がある。
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("nyan.png");

	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);


	// ビュープロジェクションのfarZを適度に大きい値に変更する
	viewProjection_.farZ = 2000;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();



	// 自キャラの生成
	player_ = new Player();
	// 敵キャラの生成
	enemy_ = new Enemy();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// Skydome の生成
	skydome_ = new Skydome();
	// レールカメラの生成
	railCamera_ = new RailCamera();



	// 自キャラの初期化
	player_->Initialize(model_,textureHandle_,playerPosition);
	// 敵の初期化(nullじゃないときだけ実行)
	if (enemy_ != nullptr) 
	{
		enemy_->Initialize(model_, position_, velocity_, approachVelocity_, leaveVelocity_);

		// 敵キャラにゲームシーンを渡す
		enemy_->SetGameScene(this);
	}
	// Skydome の初期化
	skydome_->Initialize(modelSkydome_,textureHandle_);
	// railCamera の初期化
	railCamera_->Initialize(railCameraPosition, railCameraRotate);



	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
}

void GameScene::Update() 
{
	// 自キャラの更新
	player_->Update();
	// 敵の更新
	enemy_->Update();
	// Skydome の更新
	skydome_->Update();

	

	// デバッグカメラ有効無効切り替え
	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN))
	{
		if (isDebugCameraActive_)
		{
			isDebugCameraActive_ = false;
		}
		if (!isDebugCameraActive_) 
		{
			isDebugCameraActive_ = true;
		}
	}
	#endif

	// カメラの処理
	if (isDebugCameraActive_)
	{
		// デバッグカメラの更新
		debugCamera_->Update();
		

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// レールカメラの更新
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	// 当たり判定呼び出し
	CheckAllCollisions();


	// 敵の弾更新
	// for文でリスト内のすべてのPlayerBulletについて1個1個処理していく。
	for (EnemyBullet* enemyBullet : enemyBullets_) // 範囲for文。コンテナや配列を簡潔に扱うためのfor文の別表現。
	{
		// PlayerBulletのポインタのリストからPlayerBulletのポインタを1個づつ
		// 取り出しながら処理を回していく。
		// PlayerBullet* がリスト内の要素1個分の型。
		enemyBullet->Update();
	}
}

// 当たり判定
void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB, posC, posD;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region
	// 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets)
	{
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) + 
			(posB.y - posA.y) * (posB.y - posA.y) + 
			(posB.z - posA.z) * (posB.z - posA.z);
		if (distance <= (playerRad_ + enemyBulletRad_) * (playerRad_ + enemyBulletRad_))
		{
			player_->OnCollision();
			bullet->OnCollision();
		}
	}

	#pragma endregion
	
	#pragma region 
	// 自弾と敵キャラ
	posC = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {

		posD = bullet->GetWorldPosition();

		float distance = (posD.x - posC.x) * (posD.x - posC.x) + (posD.y - posC.y) * (posD.y - posC.y) + (posD.z - posC.z) * (posD.z - posC.z);
		if (distance <= (enemyRad_ + playerBulletRad_) * (enemyRad_ + playerBulletRad_)) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
    #pragma endregion

	#pragma region 
	// 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) 
	{
		for (EnemyBullet* enemyBullet : enemyBullets) 
		{
			// 自弾の座標
			posD = playerBullet->GetWorldPosition();
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			float distance = (posD.x - posB.x) * (posD.x - posB.x) + (posD.y - posB.y) * (posD.y - posB.y) + (posD.z - posB.z) * (posD.z - posB.z);
			if (distance <= (enemyBulletRad_ + playerBulletRad_) * (enemyBulletRad_ + playerBulletRad_)) {
				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}

    #pragma endregion
}


void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet)
{
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}


void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);
	// 敵の描画
	enemy_->Draw(viewProjection_);
	// Skydome の描画
	skydome_->Draw(viewProjection_);
	// 敵の弾描画
	// 全部の弾の描画を呼び出す
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
