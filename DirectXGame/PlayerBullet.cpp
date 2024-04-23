#include "PlayerBullet.h"
#include"cassert"
#include "TextureManager.h"
#include"Player.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) 
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ関数に移し替える
	/*
	クラス外から受け取った値を、クラス内のいろんなところで参照したい場合は
	このように引数で外部から値を受け取ってクラス内のメンバ変数に
	移し替えるのが基本テクニック。
	*/
	velocity_ = velocity;
};

void PlayerBullet::Update() 
{ 
	// 座標を移動させる(1フレームの移動量を足しこむ)
	/*
	内部的にはX座標、Y座標、Z座標をそれぞれ足しているだけだが、
	ベクトルとしてセットで扱うことに慣れておくことが3Dゲーム制作では重要となる。
	要素に分解せず、ベクトルの加算用の関数などでまとめて加算すること。
	*/
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 時間経過でデス
	/*
	毎フレームタイマーをカウントダウンしていき、タイマーが 0 になったら
	デスフラグを立てる。
	デスフラグは死亡の目印で、これが立っているものは次フレームの戦闘で削除する。
	*/
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}


	//　行列を更新
	worldTransform_.UpdateMatrix(); 
};

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{ 
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};
