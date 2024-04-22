#include <bullet.h>

void Bullet::render(gl2d::Renderer2D& renderer,
	gl2d::Texture bulletsTexture, gl2d::TextureAtlasPadding bulletsAtlas)
{
	float spaceShipAngle = atan2(fireDirection.y, -fireDirection.x);
	spaceShipAngle = glm::degrees(spaceShipAngle) + 90.f;

	auto textureCoordinate = bulletsAtlas.get(1, 1);

	if (isEnemy)
	{
		textureCoordinate = bulletsAtlas.get(0, 0);
	}

	for (int i = 0; i < 5; i++)
	{
		glm::vec4 color(1 * (i + 4) / 5.f, 1 * (i + 4) / 5.f, 1 * (i + 4) / 5.f, (i + 1) / 5.f);

		renderer.renderRectangle({ position - glm::vec2(50,50) + (float)i * 25.f * fireDirection, 100,100 },
			bulletsTexture, color, {}, spaceShipAngle, textureCoordinate);
	}
}

void Bullet::update(float deltaTime)
{
	position += fireDirection * deltaTime * speed;
}
