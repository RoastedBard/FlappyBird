#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Transform.h"
#include "ComponentHandle.h"

typedef struct gameObject_s
{
	Transform transform;

	unsigned sizeOfComponentArray;
	ComponentHandle *componentArray;
}gameObject_s;

void GameObjectAddComponent(unsigned type);

#endif