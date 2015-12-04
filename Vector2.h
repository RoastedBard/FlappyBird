#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct 
{
	int x;
	int y;
} Vector2i;

void setXYi(Vector2i *vec, int x, int y);

typedef struct 
{
	float x;
	float y;
} Vector2f;

void setXYf(Vector2f *vec, float x, float y);

#endif