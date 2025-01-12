﻿#include <Novice.h>

#include "BaseData.hpp"
#include "SpriteClass.hpp"

#include "Particle.hpp"

#include "Camera.hpp"


#include "Math.hpp"
#include "Vector2.hpp"
#include "Matrix2x2.hpp"

#include "Easing.hpp"


#include "Controller.hpp"


#include "Map.hpp"


#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Boss.hpp"



const char kWindowTitle[] = "LC1A_05_オヌキセイヤ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, ScreenSize.x, ScreenSize.y);
	//	Novice::SetWindowMode(kFullscreen);

		// キー入力結果を受け取る箱


	const int WhiteBG = Novice::LoadTexture("./Resources/Texture/Background/WhiteBG.png");


	ImportTexture();

	unsigned int pressFlame = 0;

	for (int i = 0; i < EnemyMax; i++) {
		enemy[i].Reset();
	}

	Player player[3] = { Player(Novice::LoadTexture("./Resources/Texture/Entity/Player/Idle/Heart.png"), 224, 224, 0, 1),Player(Novice::LoadTexture("./Resources/Texture/Entity/Player/Idle/Heart.png"), 224, 224, 0, 1),Player(TestCircleTexture, 32, 32, 0, 1) };

	for (int i = 0; i < playerCount; i++) {
		player[i].Reset(i);
	}

	Particle test;

	Bullet::BulletReset();



	Boss boss;



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///



		TextureUpdate();
		flame++;
		test.CheckDelete();
		for (int i = 0; i < playerCount; i++) {
			player[i].EntityMoveInput();

			if (player[i].velocity.Length() != 0 || isKeyPress(DIK_SPACE)) {
				test.Spawn(player[i].position);
			}
		}

		test.Update();

		for (int i = 0; i < EnemyMax; i++) {
			enemy[i].Despawn();
		}


		if (isKeyTrigger(DIK_A)) {
			for (int i = 0; i < EnemyMax; i++) {
				if (!enemy[i].isAlive) {
					enemy[i].Spawn();
					break;
				}
			}
			enemy[0].textureHandle = TestUVTexture;
			enemy[0].srcH = 512;
			enemy[0].srcW = 512;
			enemy[0].color = 0xFFFFFFFF;
		}

		player[1].nowPolar.theta = player[0].nowPolar.theta + Degree2Radian(360 / playerCount);
		player[1].prePolar.theta = player[0].prePolar.theta + Degree2Radian(360 / playerCount);
		for (int i = 0; i < playerCount; i++) {
			player[i].EntityUpdate();
		}

		if (isKeyTrigger(DIK_B)) {
			enemy[0].position = MapCentor;
		}

		for (int i = 0; i < EnemyMax; i++) {
			enemy[i].EntityMoveUpdate();
		}

		for (int i = 0; i < playerCount; i++) {
			player[i].EnemyHit();
		}

		Bullet::BulletUpdate();
		Bullet::BulletDelete();
		Bullet::BulletEnemyHit();
		Bullet::BulletReset();

		if (isKeyTrigger(DIK_D)) {
			boss.isAlive = true;




			/*if (isCross(boss.endPosition, (boss.endPosition + ((boss.statePosition - boss.endPosition).Nomalize()) * 1000), ZeroVector2, { 0,ScreenSize.y }) ||
				isCross(boss.endPosition, (boss.endPosition + ((boss.statePosition - boss.endPosition).Nomalize()) * 1000), ZeroVector2, { ScreenSize.x,ScreenSize.y })) {

			}*/


		}
		if (isKeyTrigger(DIK_E)) {


		}





		if (boss.isAlive) {
			boss.count++;

			if (boss.count % 100 == 0 && boss.count != 0 && !boss.attackFlag) {
				boss.attackFlag = true;

				boss.statePosition = PolarToVector2(PolarCoordinates{ MapRadius,Degree2Radian(GetRandom(0,359)) }) + MapCentor;
				boss.endPosition = PolarToVector2(PolarCoordinates{ MapRadius,Degree2Radian(GetRandom(0,359)) }) + MapCentor;

				boss.statePosition = boss.statePosition + ((boss.statePosition - boss.endPosition).Nomalize()) * 1400;
				boss.endPosition = boss.endPosition + ((boss.statePosition - boss.endPosition).Nomalize()) * -1400;

			}

			if (boss.attackFlag) {

				for (int i = 0; i < 2; i++) {
					Novice::ScreenPrintf(10, 710 + i * 20, "%f", Ellipse2LineLength(player[i].position, boss.statePosition, boss.endPosition));
					player[i].position.Vector2Printf(0, 770 + i * 40);
					boss.statePosition.Vector2Printf(0, 850);
					boss.endPosition.Vector2Printf(0, 890);

					if (Ellipse2LineLength(player[i].position, boss.statePosition, boss.endPosition) <= player[i].radius && player[i].velocity.Length() != 0) {
						Boss::HitDamage();
					}


				}


			}




			/*
			switch (movePattern1)
			{
			case 0://上スタート




				switch (movePattern2)
				{
				case 0://左から1番目
					if (boss.endPosition.y > 0) {
						boss.nowEndPosition.y -= 30;
						boss.nowEndPosition.x += 17;
					}


					boss.statePosition.x = MiddleCentor.x - MapRadius;
					boss.statePosition.y = ScreenSize.y;

					boss.endPosition.x = MiddleCentor.x - MapRadius + boss.nowEndPosition.x;
					boss.endPosition.y = boss.statePosition.y + boss.nowEndPosition.y;
					break;

				case 1://左から2番目
					if (boss.endPosition.y > 0) {
						boss.nowEndPosition.y -= 30;
						boss.nowEndPosition.x += 0;
					}


					boss.statePosition.x = MiddleCentor.x - MapRadius * 0.5;
					boss.statePosition.y = ScreenSize.y;

					boss.endPosition.x = MiddleCentor.x - MapRadius * 0.5 + boss.nowEndPosition.x;
					boss.endPosition.y = boss.statePosition.y + boss.nowEndPosition.y;
					break;

				case 2://左から3番目
					if (boss.endPosition.y > 0) {
						boss.nowEndPosition.y -= 30;
						boss.nowEndPosition.x -= 0;
					}


					boss.statePosition.x = MiddleCentor.x + MapRadius * 0.5;
					boss.statePosition.y = ScreenSize.y;

					boss.endPosition.x = MiddleCentor.x + MapRadius * 0.5 + boss.nowEndPosition.x;
					boss.endPosition.y = boss.statePosition.y + boss.nowEndPosition.y;
					break;

				case 3://左から4番目
					if (boss.endPosition.y > 0) {
						boss.nowEndPosition.y -= 30;
						boss.nowEndPosition.x -= 17;
					}


					boss.statePosition.x = MiddleCentor.x + MapRadius;
					boss.statePosition.y = ScreenSize.y;

					boss.endPosition.x = MiddleCentor.x - MapRadius + boss.nowEndPosition.x;
					boss.endPosition.y = boss.statePosition.y + boss.nowEndPosition.y;
					break;

				}


				boss.count++;
				break;


			case 1://下スタート



				switch (movePattern2)
				{
				case 0://左から1番目
					if (boss.endPosition.y > 0) {
						boss.nowEndPosition.y += 30;
						boss.nowEndPosition.x += 17;
					}


					boss.statePosition.x = MiddleCentor.x + MapRadius;
					boss.statePosition.y = boss.statePosition.y + boss.nowEndPosition.y;

					boss.endPosition.x = MiddleCentor.x - MapRadius;
					boss.endPosition.y = ScreenSize.y;
					break;

				case 1://左から2番目
					if (boss.endPosition.y > 0) {
						boss.nowEndPosition.y += 30;
						boss.nowEndPosition.x += 17;
					}



					boss.statePosition.x = MiddleCentor.x - MapRadius * 0.5;
					boss.statePosition.y = boss.statePosition.y + boss.nowEndPosition.y;

					boss.endPosition.x = MiddleCentor.x - MapRadius * 0.5;
					boss.endPosition.y = ScreenSize.y;
					break;

				case 2://左から3番目
					if (boss.endPosition.y > 0) {
						boss.nowEndPosition.y += 30;
						boss.nowEndPosition.x += 17;
					}



					boss.statePosition.x = MiddleCentor.x + MapRadius * 0.5;
					boss.statePosition.y = boss.statePosition.y + boss.nowEndPosition.y;

					boss.endPosition.x = MiddleCentor.x + MapRadius * 0.5;
					boss.endPosition.y = ScreenSize.y;
					break;

				case 3://左から4番目
					if (boss.endPosition.y > 0) {
						boss.nowEndPosition.y += 30;
						boss.nowEndPosition.x += 17;
					}



					boss.statePosition.x = MiddleCentor.x + MapRadius;
					boss.statePosition.y = boss.statePosition.y + boss.nowEndPosition.y;

					boss.endPosition.x = MiddleCentor.x - MapRadius;
					boss.endPosition.y = ScreenSize.y;
					break;

				}
				break;


			case 2://右スタート

				switch (movePattern2)
				{
				case 0://上から1番目
					if (boss.endPosition.x > 0) {
						boss.nowEndPosition.x -= 30;
						boss.nowEndPosition.y -= 17;
					}


					boss.statePosition.x = ScreenSize.x;
					boss.statePosition.y = MiddleCentor.y + MapRadius;

					boss.endPosition.x = boss.statePosition.x + boss.nowEndPosition.x;
					boss.endPosition.y = MiddleCentor.y + MapRadius;
					break;

				case 1://上から2番目
					if (boss.endPosition.x > 0) {
						boss.nowEndPosition.x -= 30;
						boss.nowEndPosition.y -= 17;
					}


					boss.statePosition.x = ScreenSize.x;
					boss.statePosition.y = MiddleCentor.y + MapRadius * 0.5;

					boss.endPosition.x = boss.statePosition.x + boss.nowEndPosition.x;
					boss.endPosition.y = MiddleCentor.y + MapRadius * 0.5;
					break;

				case 2://上から3番目
					if (boss.endPosition.x > 0) {
						boss.nowEndPosition.x -= 30;
						boss.nowEndPosition.y -= 17;
					}


					boss.statePosition.x = ScreenSize.x;
					boss.statePosition.y = MiddleCentor.y - MapRadius * 0.5;

					boss.endPosition.x = boss.statePosition.x + boss.nowEndPosition.x;
					boss.endPosition.y = MiddleCentor.y - MapRadius * 0.5;
					break;

				case 3://上から4番目
					if (boss.endPosition.x > 0) {
						boss.nowEndPosition.x -= 30;
						boss.nowEndPosition.y -= 17;
					}


					boss.statePosition.x = ScreenSize.x;
					boss.statePosition.y = MiddleCentor.y - MapRadius;

					boss.endPosition.x = boss.statePosition.x + boss.nowEndPosition.x;
					boss.endPosition.y = MiddleCentor.y - MapRadius;
					break;

				}

				break;


			case 3://左スタート


				switch (movePattern2)
				{
				case 0://上から1番目


					boss.statePosition.x = ScreenSize.x;
					boss.statePosition.y = MiddleCentor.y + MapRadius;

					boss.endPosition.x = boss.statePosition.x + boss.nowEndPosition.x;
					boss.endPosition.y = MiddleCentor.y + MapRadius;
					break;

				case 1://上から2番目
					boss.statePosition.x = ScreenSize.x;
					boss.statePosition.y = MiddleCentor.y + MapRadius * 0.5;

					boss.endPosition.x = boss.statePosition.x + boss.nowEndPosition.x;
					boss.endPosition.y = MiddleCentor.y + MapRadius * 0.5;
					break;

				case 2://上から3番目
					boss.statePosition.x = ScreenSize.x;
					boss.statePosition.y = MiddleCentor.y - MapRadius * 0.5;

					boss.endPosition.x = boss.statePosition.x + boss.nowEndPosition.x;
					boss.endPosition.y = MiddleCentor.y - MapRadius * 0.5;
					break;

				case 3://上から4番目
					boss.statePosition.x = ScreenSize.x;
					boss.statePosition.y = MiddleCentor.y - MapRadius;

					boss.endPosition.x = boss.statePosition.x + boss.nowEndPosition.x;
					boss.endPosition.y = MiddleCentor.y - MapRadius;
					break;

				}


				break;
			}
			*/


		}



		Camera::Update();



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawQuadFunction(MiddleCentor, { 2048,1280 }, 0, 0, 1024, 640, WhiteBG, WHITE);
		/*
		Novice::SetBlendMode(BlendMode::kBlendModeAdd);
		Novice::DrawBox(0, 0, ScreenSize.x, ScreenSize.y, 0.0f, 0x444444FF, kFillModeSolid);
		Novice::SetBlendMode(BlendMode::kBlendModeNormal);
		*/
		Bullet::DrawBullet();

		//Novice::DrawBox(0, 0, ScreenSize.x, ScreenSize.y, 0.0f, 0x000000FF, kFillModeSolid);
		enemy[0].position.Vector2Printf(0, 0);
		boss.statePosition.Vector2Printf(0, 50);

		Novice::ScreenPrintf(10, 100, "%.3f", Length(Bullet::bullet[0].position, enemy[0].position) - (Bullet::bullet[0].radius + enemy[0].radius));

		Novice::DrawEllipse(MiddleCentor.x + Camera::scroll.x, ToWorld(MiddleCentor.y + Camera::scroll.y), MapRadius, MapRadius, 0.0f, 0xFFFFFFFF, kFillModeWireFrame);

		Novice::DrawEllipse(MiddleCentor.x + Camera::scroll.x, ToWorld(MiddleCentor.y + Camera::scroll.y), 64, 64, 0.0f, 0x00FF00FF, kFillModeSolid);

		//	Novice::DrawEllipse(test.particles[0].position.x, ToWorld(test.particles[0].position.y), 100, 100, 0.0f, 0x00FF0077, kFillModeSolid);


		test.Draw();

		for (int i = 0; i < playerCount; i++) {
			player[i].DrawEntity();
		}

		for (int i = 0; i < EnemyMax; i++) {
			if (enemy[i].isAlive) {
				enemy[i].DrawEntity();
			}
		}
		if (boss.isAlive) {
			if (boss.attackFlag) {
				Novice::DrawLine(boss.statePosition.x, ToWorld(boss.statePosition.y), boss.endPosition.x, ToWorld(boss.endPosition.y), 0x00ffffff);
				boss.BossDraw();
			}

		//	Novice::DrawEllipse(boss.position.x, ToWorld(boss.position.y), boss.srcX, boss.srcY, 0.0f, boss.color, kFillModeSolid);

		}



		//	enemy[0].DrawEntity();

		Novice::ScreenPrintf(100, 100, "boss.attackFlag %d", boss.attackFlag);
		Novice::ScreenPrintf(100, 150, "boss.count %f", boss.count);
		/*Novice::ScreenPrintf(100, 170, "bullet.y %f", );*/
		Novice::ScreenPrintf(100, 250, "velocity.x %f", player[0].velocity.x);
		Novice::ScreenPrintf(100, 270, "velocity.y %f", player[0].velocity.y);



		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
