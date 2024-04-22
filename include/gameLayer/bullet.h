#pragma once
#include <gl2d/gl2d.h>

struct Bullet {
	glm::vec2 position = {};
	glm::vec2 fireDirection = {};
	float initDistance = 10;
	float speed = 2000.0f;
	int size = 0;
	void render(gl2d::Renderer2D& renderer, gl2d::Texture bulletsTexture, gl2d::TextureAtlasPadding bulletsAtlas);

	void update(float deltaTime);

	bool isEnemy = false;

};