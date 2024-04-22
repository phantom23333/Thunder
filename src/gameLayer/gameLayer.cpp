#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Input.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <assertion.h>
#include <future>

#include "bullet.h"
#include "enemy.h"
#include "raudio.h"
#include "tileRenderer.h"
#include "glui/glui.h"


constexpr int BACKGROUND = 4;
constexpr float shipSize = 250.f;
constexpr float enemySize = 250.f;
constexpr int bulletSize = 100;

constexpr float playerDmg = 0.5f;
constexpr float enemyDmg = 0.05f;

static float enemyRespawnTime = 1.5f;
constexpr float ResetEnemyRespawnTime = 1.5f;

struct GameplayData
{
	glm::vec2 playerPos = { 100, 100 };
	std::vector<Bullet> Bullets;
	std::vector<Enemy> Enemies;
	float health = 1.0f;
};

GameplayData data;
gl2d::Renderer2D renderer;

gl2d::Texture spaceShipTexture;
gl2d::TextureAtlasPadding spaceShipAtlas;

gl2d::Texture backgroundTexture[BACKGROUND];
TileRenderer tileRenderer[BACKGROUND];

gl2d::Texture bulletTexture;
gl2d::TextureAtlasPadding bulletAtlas;

gl2d::Texture healthBar;
gl2d::Texture health;

static std::random_device rd;
static std::mt19937 engine(rd());

Sound shootSound;

void restartGame()
{
	data = {};
	renderer.currentCamera.follow(data.playerPos
		, 550, 0, 0, renderer.windowW, renderer.windowH);
	healthBar.loadFromFile(RESOURCE_PATH "healthBar.png", true);
	health.loadFromFile(RESOURCE_PATH "health.png", true);

}

bool hasIntersection(glm::vec2 shipPosition, glm::vec2 bulletPosition, float size)
{
	return glm::length(bulletPosition - shipPosition) < size;
}

void spanwEnemy()
{
	glm::uvec2 shipTypes[] = { {0,0}, {0,1}, {2,0}, {3, 1} };

	Enemy e;
	e.position = data.playerPos;

	glm::vec2 offset(2000, 0);
	offset = glm::vec2(glm::vec4(offset, 0, 1) * glm::rotate(glm::mat4(1.0f), (glm::degrees((float)(rand() % 360))), glm::vec3(0, 0, 1)));

	e.position += offset;

	e.speed = 800 + rand() % 1000;
	e.turnSpeed = 2.2f + (rand() & 1000) / 500.f;
	e.type = shipTypes[rand() % 4];
	e.fireRange = 1.5 + (rand() % 1000) / 2000.f;
	e.fireTimeReset = 0.1 + (rand() % 1000) / 500;
	e.bulletSpeed = rand() % 3000 + 1000;

	data.Enemies.push_back(e);
}

bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	spaceShipTexture.loadFromFileWithPixelPadding(RESOURCE_PATH "spaceShip/stitchedFiles/spaceships.png", 128, true, false);
	spaceShipAtlas = gl2d::TextureAtlasPadding(5, 2, spaceShipTexture.GetSize().x, spaceShipTexture.GetSize().y);

	bulletTexture.loadFromFileWithPixelPadding
	(RESOURCE_PATH "spaceShip/stitchedFiles/projectiles.png", 500, false);
	bulletAtlas = gl2d::TextureAtlasPadding(3, 2, bulletTexture.GetSize().x, bulletTexture.GetSize().y);


	shootSound = LoadSound(RESOURCE_PATH "shoot.flac");
	SetSoundVolume(shootSound, 0.1f);


    for (int i = 0; i < BACKGROUND; i++)
    {
        std::string filename = RESOURCE_PATH + std::string("background") + std::to_string(i + 1) + ".png";
		backgroundTexture[i].loadFromFile(filename.c_str(), true);
		tileRenderer[i].texture = backgroundTexture[i];
    }
	restartGame();

	return true;
}



bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h

	glViewport(0, 0, w, h);
	std::uniform_int_distribution<int> distribution(-w / 2, w / 2);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion

	// INPUT
	glm::vec2 move = {};

	// is only the direction of the input
	if (platform::isButtonHeld(platform::Button::W))
	{
		move.y = -1;
	}
	if (platform::isButtonHeld(platform::Button::S))
	{
		move.y = 1;
	}
	if (platform::isButtonHeld(platform::Button::A))
	{
		move.x = -1;
	}
	if (platform::isButtonHeld(platform::Button::D))
	{
		move.x = 1;
	}

	if (move.x != 0 || move.y != 0)
	{
		move = glm::normalize(move);
		move *= deltaTime * 1200;
		data.playerPos += move;

	}

	glm::vec2 mousePos = platform::getRelMousePosition();
	glm::vec2 screenCenter(w / 2.f, h / 2.f);
	glm::vec2 mouseDirection = mousePos - screenCenter;
	if (glm::length(mouseDirection) == 0.0f)
	{
		mouseDirection = { 1, 0 };
	} else
	{
		mouseDirection = glm::normalize(mouseDirection);
	}


	if (platform::isLMousePressed())
	{
		
		Bullet b;
		b.size = bulletSize;
		b.position = data.playerPos;
		b.fireDirection = mouseDirection;
		PlaySound(shootSound);
		data.Bullets.push_back(b);
	}


	// update

	glm::uvec2 typeArray[] = { {0,0}, {0,1}, {2, 0}, {3,1} };
	auto SpawnEnemy = [&distribution, &typeArray]()
		{
			Enemy e;
			e.position = data.playerPos + glm::vec2(distribution(engine), distribution(engine));
			e.type = typeArray[std::abs(distribution(engine) % 4)];
			e.turnSpeed = 2.0f + std::abs(distribution(engine) % 3);
			e.speed = 700.0f + static_cast<float>(std::abs(distribution(engine)) % 200);
			data.Enemies.push_back(e);
		};


	if (data.Enemies.size() < 10)
	{
		enemyRespawnTime -= deltaTime;
		if (enemyRespawnTime < 0)
		{
			SpawnEnemy();
			spanwEnemy();
			enemyRespawnTime = ResetEnemyRespawnTime;
		}

	}

	float maximumDistance = 4000.0f;
	data.Bullets.erase(std::remove_if(data.Bullets.begin(), data.Bullets.end(), [&maximumDistance](const Bullet& bullet)
		{
			if (glm::distance(bullet.position, data.playerPos) > maximumDistance)
			{
				return true;
			}
			bool flagRemoveBullet = false;
			// calculate whether player's bullet shoots the enemy bullet
			if (!bullet.isEnemy)
			{
				
				data.Enemies.erase(std::ranges::remove_if(data.Enemies, [&flagRemoveBullet, &bullet](Enemy& enemy)
				{
						
					if (hasIntersection(enemy.position, bullet.position, enemySize))
					{
						flagRemoveBullet = true;
						enemy.health -= playerDmg;
						if (enemy.health < 0.0f)
						{
							return true;
						}
							
					}
					return false;
				}).begin(), data.Enemies.end());

			} else
			{
				if (hasIntersection(data.playerPos, bullet.position, shipSize))
				{
					flagRemoveBullet = true;
					data.health -= enemyDmg;
					if (data.health < 0.0f)
					{
						
					}
				}
			}
			return flagRemoveBullet;

		}), data.Bullets.end());

    for (auto& bullet : data.Bullets) {
        bullet.update(deltaTime);
    }

	for (auto& enemy : data.Enemies)
	{
		enemy.playerPosition = data.playerPos;
		enemy.update(deltaTime);
	}

	for (auto& enemy : data.Enemies)
	{
		if (enemy.shoot)
		{
			Bullet b;
			b.isEnemy = true;
			b.size = bulletSize;
			b.position = enemy.position;
			b.fireDirection = enemy.viewDirection;
			b.speed = enemy.bulletSpeed;
			data.Bullets.push_back(b);
		}
	}

	if (data.health < 0.0f)
	{
		restartGame();
	}

	// Renderer
	for (int i = 0; i < BACKGROUND; ++i)
	{
		tileRenderer[i].paralaxStrengh = i * 0.2f;
		if (tileRenderer[i].paralaxStrengh < 1.0f)
		{
			tileRenderer[i].paralaxStrengh = 0.9f;
		}
		tileRenderer[i].render(renderer);
	}


	renderer.currentCamera.zoom = 0.5;
	renderer.currentCamera.follow({ data.playerPos}, deltaTime * 450.0f, 1, 10, w, h);

	for (auto& enemy : data.Enemies)
	{
		enemy.size = (int)enemySize;
		enemy.render(renderer, spaceShipTexture, spaceShipAtlas);
	}

	renderSpaceShip(renderer, spaceShipAtlas.get(3, 0), (int)shipSize, mouseDirection, data.playerPos, spaceShipTexture);


	for (auto& b : data.Bullets)
	{
		b.render(renderer, bulletTexture, bulletAtlas);
	}


	renderer.pushCamera();
	{
		
		glui::Frame f({ 0,0, w, h });

		glui::Box healthBox = glui::Box().xLeftPerc(0.65).yTopPerc(0.1).
			xDimensionPercentage(0.3).yAspectRatio(1.f / 8.f);

		renderer.renderRectangle(healthBox, healthBar);

		glm::vec4 newRect = healthBox();
		newRect.z *= data.health;

		glm::vec4 textCoords = { 0,1,1,0 };
		textCoords.z *= data.health;

		renderer.renderRectangle(newRect, health, Colors_White, {}, {},
			textCoords);

	}
	renderer.popCamera();

	renderer.flush();


	ImGui::Begin("Bullet Count");
	ImGui::Text("Bullets Count: %i", static_cast<int>(data.Bullets.size()));
	ImGui::Separator();
	ImGui::Text("Enemy Count: %i", static_cast<int>(data.Enemies.size()));
	ImGui::Separator();
	if (ImGui::Button("Spawn Enemy"))
	{
		SpawnEnemy();
	}
	ImGui::Separator();
	if (ImGui::Button("restart Game"))
	{
		restartGame();
	}
	ImGui::SliderFloat("Player Health", &data.health, 0, 1);
	ImGui::End();


	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{



}