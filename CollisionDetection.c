#include "CollisionDetection.h"

int isRectanglesOverlap_v(Vector2i rect1Top, Vector2i rect1Right, Vector2i rect2Top, Vector2i rect2Right)
{
	if(rect1Top.x > rect2Right.x || rect2Top.x > rect1Right.x)
		return 0;

	if(rect1Top.y < rect2Right.y || rect2Top.y < rect1Right.y)
		return 0;

	return 1;
}

int isRectanglesOverlap_p(int rect1TopX, int rect1TopY, int rect1RightX, int rect1RightY, int rect2TopX, int rect2TopY, int rect2RightX, int rect2RightY)
{
	if(rect1TopX > rect2RightX || rect2TopX > rect1RightX)
		return 0;

	if(rect1TopY < rect2RightY || rect2TopY < rect1RightY)
		return 0;

	return 1;
}

int contains(int x, int y, int rectTopX, int rectTopY, int rectBottomX, int rectBottomY)
{
	if((x >= rectTopX && x <= rectBottomX) && (y >= rectTopY && y <= rectBottomY))
		return 1;
	return 0;
}

int doRectanglesCollide(int rect1TopX, int rect1TopY, int rect1Height, int rect1Width, int rect2TopX, int rect2TopY, int rect2RightX, int rect2RightY)
{
	if(contains(rect1TopX, rect1TopY, rect2TopX, rect2TopY, rect2RightX, rect2RightY)     ||
	   contains(rect1TopX + rect1Width, rect1TopY + rect1Height, rect2TopX, rect2TopY, rect2RightX, rect2RightY) ||
	   contains(rect1TopX + rect1Width, rect1TopY, rect2TopX, rect2TopY, rect2RightX, rect2RightY) ||
	   contains(rect1TopX, rect1TopY + rect1Height, rect2TopX, rect2TopY, rect2RightX, rect2RightY))
		return 1;
	return 0;
}