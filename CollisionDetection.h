#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include "Vector2.h"

int isRectanglesOverlap_v(Vector2i rect1Top, Vector2i rect1Right, Vector2i rect2Top, Vector2i rect2Right);
int isRectanglesOverlap_p(int rect1TopX, int rect1TopY, int rect1RightX, int rect1RightY, int rect2TopX, int rect2TopY, int rect2RightX, int rect2RightY);
int contains(int x, int y, int rectTopX, int rectTopY, int rectBottomX, int rectBottomY);
int doRectanglesCollide(int rect1TopX, int rect1TopY, int rect1Height, int rect1Width, int rect2TopX, int rect2TopY, int rect2RightX, int rect2RightY);

#endif