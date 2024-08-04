#include "TitleScene.h"
#include"TextureManager.h"

TitleScene::~TitleScene()
{ delete sprite_; }

void TitleScene::Initialize() { 
	
	dxCommon_ = DirectXCommon::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Title.png");
	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
}

void TitleScene::Update() {
	//XINPUT_STATE joyState;

	/*if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
	
		return;
	}*/

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() 
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();


	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();

}
