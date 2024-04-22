#pragma once

#include <gl2d/gl2d.h>

struct TileRenderer {
	float backgroundSize = 10000;
	gl2d::Texture texture;
	float paralaxStrengh = 1.0f;
	void render(gl2d::Renderer2D& renderer);

	
};

inline void renderSpaceShip(gl2d::Renderer2D& renderer, glm::vec4 uv, int size, glm::vec2 viewDirection, glm::vec2 position, gl2d::Texture texture)
{
	float spaceShipAngle = glm::atan(viewDirection.y, -viewDirection.x);
	renderer.renderRectangle({ position - glm::vec2{size >> 1, size >> 1}, size, size }, texture, Colors_White, {},glm::degrees(spaceShipAngle) + 90.0f, uv);
}