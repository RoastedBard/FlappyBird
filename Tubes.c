#include "Tubes.h"

#include <stdlib.h>

#include "Constants.h"

void TubesGenerate(tubes_t *tubes, int prevTubesXCoord)
{
	tubes->topTubeHeight = rand()%((levelHeight - floorHight) - gapHeight);
	tubes->bottomTubeHeight = (levelHeight - floorHight) - (tubes->topTubeHeight + gapHeight);

    tubes->topTubeCoords.x = prevTubesXCoord + tubeWidth + distanceBetweenTubes;
	tubes->topTubeCoords.y = 0;

	tubes->bottomTubeCoords.x = tubes->topTubeCoords.x;
	tubes->bottomTubeCoords.y = tubes->topTubeHeight + gapHeight;
}

void TubesSetCoords(tubes_t *tubes, int topX, int topY, int bottomX, int bottomY)
{
	tubes->topTubeCoords.x = topX;
	tubes->topTubeCoords.y = topY;

	tubes->bottomTubeCoords.x = bottomX;
	tubes->bottomTubeCoords.y = bottomY;
}