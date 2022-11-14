#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) { 
	return world->pathDetection(); 
}
