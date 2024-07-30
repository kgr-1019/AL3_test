#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include<fstream>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;// 3Dモデルデータの解放
	if (player_ != nullptr) {
		delete player_; // 自キャラの解放
		for (Enemy* enemy : enemies_) {
			delete enemy;
		}
	}
	delete modelPlayer_;
	delete debugCamera_;// デバッグカメラ
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
	//textureHandle_ = TextureManager::Load("nyan.png");
	textureReticle = TextureManager::Load("Reticle.png");


	// 3Dモデルの生成
	model_ = Model::Create();
	modelPlayer_ = Model::CreateFromOBJ("Player", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);


	// ビュープロジェクションのfarZを適度に大きい値に変更する
	viewProjection_.farZ = 2000;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// プレイヤーの生成
	player_ = new Player();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// Skydome の生成
	skydome_ = new Skydome();
	// レールカメラの生成
	railCamera_ = new RailCamera();


	// 自キャラの初期化
	player_->Initialize(modelPlayer_, textureHandle_, playerPosition);
	// Skydome の初期化
	skydome_->Initialize(modelSkydome_,textureHandle_);
	// railCamera の初期化
	railCamera_->Initialize(railCameraPosition, railCameraRotate);


	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	if (player_ != nullptr) {
		// 自キャラとレールカメラの親子関係を結ぶ
		player_->SetParent(&railCamera_->GetWorldTransform());
	}
}

void GameScene::Update() 
{
	LoadEnemyPopData();
	UpdateEnemyPopCommands();

	// 自キャラの更新
	if (player_ != nullptr) {

		player_->Update(viewProjection_);

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
	}
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
		// ゴールしたら
		if (railCamera_->isGoal())
		{
			// シーン切り替え
			finished_=true;
		}
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


	// デスフラグの立ったオブジェクトを削除
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
	enemyBullets_.remove_if([](EnemyBullet* enemyBullet) {
		if (enemyBullet->IsDead()) {
			delete enemyBullet;
			return true;
		}
		return false;
	});

	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
}

// 当たり判定
void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB, posC, posD;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	
	#pragma region

	
		// 自キャラと敵キャラ
		for (Enemy* enemy : enemies_) {
			// 敵の座標
			posC = enemy->GetWorldPosition();
			// 自キャラの座標
			posA = player_->GetWorldPosition();
			float distance = (posC.x - posA.x) * (posC.x - posA.x) + (posC.y - posA.y) * (posC.y - posA.y) + (posC.z - posA.z) * (posC.z - posA.z);
			if (distance <= (playerRad_ + enemyRad_) * (playerRad_ + enemyRad_)) {
				enemy->OnCollision();
				player_->OnCollision(enemy);
			}
		}

#pragma endregion

#pragma region

		// 自キャラと敵弾全ての当たり判定
		for (EnemyBullet* enemyBullet : enemyBullets_) {
			// 自キャラの座標
			posA = player_->GetWorldPosition();
			// 敵弾の座標
			posD = enemyBullet->GetWorldPosition();

			float distance = (posD.x - posA.x) * (posD.x - posA.x) + (posD.y - posA.y) * (posD.y - posA.y) + (posD.z - posA.z) * (posD.z - posA.z);
			if (distance <= (playerRad_ + enemyBulletRad_) * (playerRad_ + enemyBulletRad_)) {
			    finished_ = true;
				enemyBullet->OnCollision();
			}
		}

#pragma endregion

#pragma region
		// 自弾と敵キャラ

		for (Enemy* enemy : enemies_) {
			posC = enemy->GetWorldPosition();
			for (PlayerBullet* playerBullet : playerBullets) {
				// 自弾の座標
				posB = playerBullet->GetWorldPosition();
				float distance = (posB.x - posC.x) * (posB.x - posC.x) + (posB.y - posC.y) * (posB.y - posC.y) + (posB.z - posC.z) * (posB.z - posC.z);
				if (distance <= (enemyRad_ + playerBulletRad_) * (enemyRad_ + playerBulletRad_)) {
					enemy->OnBulletCollision();
					playerBullet->OnCollision();
				}
			}
		}
#pragma endregion

#pragma region
		// 自弾と敵弾の当たり判定
		for (PlayerBullet* playerBullet : playerBullets) {
			// 自弾の座標
			posB = playerBullet->GetWorldPosition();
			for (EnemyBullet* enemyBullet : enemyBullets_) {

				// 敵弾の座標
				posD = enemyBullet->GetWorldPosition();

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


void GameScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPOP.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::OccurrenceEnemy(Vector3 position)
{
	// 敵を生成
	Enemy* newEnemy = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	newEnemy->SetGameScene(this);
	// 敵の初期化
	newEnemy->Initialize(model_, position, velocity_, approachVelocity_, leaveVelocity_);
	enemies_.push_back(newEnemy);
}

void GameScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			isWait = false;
		}
		return;
	}


	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0)
		{
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			
			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			OccurrenceEnemy(Vector3(x, y, z));	
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait = true;
			// 待機タイマー
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
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
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	
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

	// レティクル
	player_->DrawUI();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
