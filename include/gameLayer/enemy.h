#pragma once
#include "gl2d/gl2d.h"
#include "glm/vec2.hpp"

struct Enemy {
	glm::uvec2 type = {};

	glm::vec2 position = {};
	glm::vec2 viewDirection = {1, 0};
	int size = 0;
	float speed = 50.0f;
	float turnSpeed = 3.0f;
	float health = 1.0f;

	float fireTime = 1.0f;
	float fireTimeReset = 0.5f;
	float fireRange = 1500.0f;
	float bulletSpeed = 2000.0f;

	bool shoot = false;

	glm::vec2 playerPosition = {};


	void render(gl2d::Renderer2D& renderer, gl2d::Texture enemyTexture, gl2d::TextureAtlasPadding enemyAtlas);

	void update(float deltaTime);
};
