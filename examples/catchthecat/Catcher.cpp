#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) { return world->pathDetection(); }
