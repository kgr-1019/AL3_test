#include "RailCamera.h"

void RailCamera::Initialize(WorldTransform& worldTransform) 
{ 
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update()
{ 
	// ワールドトランスフォームの座標の数値を加算したりする(移動)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_); 
	// ワールドトランスフォームの角度の数値を加算したりする(回転)
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotate_); 


	// ワールドトランスフォームのワールド行列を再計算
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::DragFloat3("inputFloat3", worldTransform_.translation_.x,0.01f);
	ImGui::DragFloat3("inputFloat3", worldTransform_.rotation_.x,0.01f);
	ImGui::End();
}
