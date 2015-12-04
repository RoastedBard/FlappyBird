#include "Sprite.h"

void SpriteInitialize(sprite_t *sprite, int x, int y, int width, int height, unsigned int horizontalFlip, unsigned int verticalFlip)
{
	sprite->x = x;
	sprite->y = y;
	sprite->width = width;
	sprite->height = height;
	sprite->horizontalFlip = horizontalFlip;
	sprite->verticalFlip = verticalFlip;
}