#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Vector3.h"
#include"PlayerBullet.h"
#include<list>
#include "Sprite.h"
#include "TextureManager.h"
#include<WinApp.h>
#include"ViewProjection.h"

// 関数の宣言
Vector3 Add(const Vector3& translation, const Vector3& move);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(float k, const Vector3& v1);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Matrix4x4 MakeRotateXMatrix(float rotate);
Matrix4x4 MakeRotateYMatrix(float rotate);
Matrix4x4 MakeRotateZMatrix(float rotate);
Matrix4x4 Multiply(const Matrix4x4& rotateX, const Matrix4x4& rotateYZ);
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
Matrix4x4 Inverse(const Matrix4x4& m1);
Matrix4x4 MakeAffineMatrix(const Vector3& S, const Vector3& R, const Vector3& T);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);// ベクトル変換
Vector3 Normalize(const Vector3& v2);
Vector3 Transform(const Vector3& point, const Matrix4x4& transformMatrix);

class Enemy;

class Player {

public:// メンバ関数

	/// デストラクタ
	~Player();

	/// 初期化
	void Initialize(Model* model, uint32_t textureHandle,const Vector3& playerPosition);
	
	/// 更新
	void Update(const ViewProjection& viewProjection);

	/// 旋回(回転)
	void Rotate();

	/// 攻撃
	void Attack();

	/// 描画
	void Draw(ViewProjection& viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	Vector3 GetWorld3DReticlePosition();

	bool IsFinished() const { return finished_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision(Enemy* enemy);

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// 親となるワールドトランスフォームを設定するための関数
	void SetParent(const WorldTransform* parent);

	// レティクル2D描画
	void DrawUI();

private:// メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	Sprite* spriteBulletUI_ = nullptr;
	// レティクル初期位置
	Vector3 positionReticle = {640.0f, 360.0f, 0.0f};

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureReticle_ = 0;
	uint32_t textureBulletUI_ = 0;

	// キーボード入力
	Input* input_ = nullptr;

	// 自機弾
	PlayerBullet* bullet_ = nullptr;

	// 弾
	// listの導入
	// PlayerBulletのポインタのリスト。配列みたいな性質を持つリストで複数管理することによって
	// 複数のUpdateとDrawがまとめて呼び出せるようになった。
	std::list<PlayerBullet*> bullets_; // 配列みたいなもの(コンテナ)になったので、複数形のsを追加しておく

	// デスフラグ
	//bool isDead_ = false;
	bool finished_ = false;

	// 残弾数
	int bulletCount = 5;

	int bulletTimer = 0;
};
