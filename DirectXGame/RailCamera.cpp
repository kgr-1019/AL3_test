#include "RailCamera.h"
//#include"imgui.h"
void RailCamera::Initialize(const Vector3& railCameraPosition, const Vector3& railCameraRotate) { 
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = railCameraPosition;
	worldTransform_.rotation_ = railCameraRotate;

	// ファイル名を指定してテクスチャを読み込む
	textureClear_ = TextureManager::Load("Clear.png");
	textureSusume_ = TextureManager::Load("susume.png");
	// スプライトの生成
	spriteClear_ = Sprite::Create(textureClear_, {400, 250});
	spriteSusume_ = Sprite::Create(textureSusume_, {400, 0});

	// ビュープロジェクションのfarZを適度に大きい値に変更する
	viewProjection_.farZ = 2000;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	worldTransform_.Initialize();
}

void RailCamera::Update()
{ 
	// ゴールの位置過ぎたら
	if (worldTransform_.translation_.z >= 0.0f)
	{
		goalCount++;
		isGoalCount = true;
	} 
	else 
	{
		isGoalCount = false;
	}

	if (goalCount >= 200)
	{
		isGoal_ = true;
	}

	// ワールドトランスフォームの座標の数値を加算したりする(移動)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_); 
	// ワールドトランスフォームの角度の数値を加算したりする(回転)
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotate_); 


	// ワールドトランスフォームのワールド行列を再計算
	worldTransform_.UpdateMatrix();

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//// カメラの座標を画面表示する処理
	//ImGui::Begin("Camera");
	//ImGui::DragFloat3("translate", &worldTransform_.translation_.x,0.01f);
	//ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x,0.01f);
	//ImGui::End();
}

void RailCamera::DrawUI() {
	spriteSusume_->Draw();
	if (isGoalCount) {

		spriteClear_->Draw();
		
	}
}

void RailCamera::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }
