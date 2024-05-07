#pragma once
#include"Model.h"
#include"Vector3.h"
#include "WorldTransform.h"
class Enemy 
{
public:// メンバ関数

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& appvelocity, const Vector3& leavelocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	// 行動フェーズ
	/*
	状態遷移の実装にもいろいろな方法があるが共通して行うこととして
	enum の定義がある。

	いくつかある行動フェーズのうち、今どれなのかを1つの変数で持つと
	一気に状態がわかり安くなる。

	そのために都合の良い型として enum を新たに定義する。
	*/
	enum class Phase
	{
		Approach,// 接近する
		Leave,// 離脱する
	};

private:// メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 速度
	Vector3 velocity_;
	Vector3 approachVelocity_;// 接近フェーズ
	Vector3 leaveVelocity_;// 離脱フェーズ

	// フェーズ
	/*
	enum 型の変数を持たせる。

	メンバ変数として持たせることで、Enemy のインスタンスが生きている間
	そのインスタンス(つまり敵一体)についての全ての関数から、フェーズを
	読み書きできる。

	またフェーズの初期値(初期フェーズ)も設定しておく。
	単に Approach と指定するのではなく、どの列挙型に属するのかを指定する。
	*/
	Phase phase_ = Phase::Approach;
};
