#ifndef TUBES_H
#define TUBES_H

#include "Vector2.h"

typedef struct tubes_t
{
 	int topTubeHeight;
 	int bottomTubeHeight; /* Also serves as y coordinate */

 	Vector2i topTubeCoords;
 	Vector2i bottomTubeCoords;

 	int xCoord;
 	int bottomTubeYCoord;
} tubes_t;

void TubesGenerate(tubes_t *tubes, int prevTubesXCoord);
void TubesSetCoords(tubes_t *tubes, int topX, int topY, int bottomX, int bottomY);

#endif