#include <enemy.h>

#include "tileRenderer.h"

void Enemy::render(gl2d::Renderer2D& renderer, gl2d::Texture enemyTexture, gl2d::TextureAtlasPadding enemyAtlas)
{
	renderSpaceShip(renderer, enemyAtlas.get(1, 0), size, viewDirection, position, enemyTexture);
}

void Enemy::update(float deltaTime)
{
	// move around the player position
	glm::vec2 directionToPlayer = playerPosition - position;
	if (glm::length(directionToPlayer) == 0) { directionToPlayer = { 1,0 }; }
	else { directionToPlayer = glm::normalize(directionToPlayer); }

	glm::vec2 newDirection = {};

	if (glm::length(directionToPlayer + viewDirection) <= 0.15f)
	{
		if (rand() % 2)
		{
			newDirection = glm::vec2(directionToPlayer.y, -directionToPlayer.x);
		} else
		{
			newDirection = glm::vec2(-directionToPlayer.y, directionToPlayer.x);
		}
		
	}
	else
	{
		newDirection =
			deltaTime * turnSpeed * directionToPlayer + viewDirection;
	}
	float length = glm::length(newDirection);
	length = glm::clamp(length, 0.1f, 3.0f);

	viewDirection = glm::normalize(newDirection);
	position += viewDirection * deltaTime * speed * length;

	// after the position is updated then decide if we could shoot
	if (glm::length(position - playerPosition) < fireRange) 
	{
		shoot = false;
		if (fireTime <= 0)
		{
			shoot = !shoot;
			fireTime = fireTimeReset;
		}
	}
	
	fireTime -= deltaTime;

}
