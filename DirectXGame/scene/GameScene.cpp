#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


	// 画像読み込み
	textureHandle_ = TextureManager::Load("tex1.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});


	model_ = Model::Create();// Model の new と Initialize() の呼び出しを1つにまとめた関数


	worldTransform_.Initialize(); // ワールドトランスフォームの初期化
	viewProjection_.Initialize(); // ビュープロジェクションの初期化

	// サウンド
	soundDataHandle_ = audio_->LoadWave("fanfare.wav"); // サウンドデータの読み込み
	// audio_->PlayWave(soundDataHandle_);// 音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true); // 音声再生(ループ、停止)


	// ライン描画が参照するビュープロジェクションを指定する（アドレスなし）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);


	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);// 画面横幅、画面縦幅

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレスなし）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

	//スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();
	//座標を{ 2 ,1 }移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);


	// スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
	// 音声停止
		audio_->StopWave(voiceHandle_);
	}


	// デバックテキストの表示
	ImGui::Begin("Debug1");
	//ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFlont3);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFlont3, 0.0f, 1.0f);
	ImGui::End();


	// デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();


	// デバッグカメラの更新
	debugCamera_->Update();


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

	model_->Draw(worldTransform_, viewProjection_, textureHandle_);// 3Dモデル描画
	
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);// モデルを連動させる

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	// ラインを描画する
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f}); // 始点座標、終点座標、色(RGBA)


#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
