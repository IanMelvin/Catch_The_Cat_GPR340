#include "HuntAndKill.h"
#include "../World.h"
#include "Random.h"
#include <climits>
#include <iostream>

Point2D HuntAndKill::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(-sideOver2, sideOver2);

  int xValue = distr(gen);
  int yValue = distr(gen);

  if (!visited[xValue][yValue]) {
    return {xValue, yValue};
  }

  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> HuntAndKill::getVisitables(World* w, const Point2D& p) {
  std::vector<Point2D> visitables;
  visitables.emplace_back(checkIfVisitable(w, {p.x, p.y - 1}));
  visitables.emplace_back(checkIfVisitable(w, {p.x + 1, p.y}));
  visitables.emplace_back(checkIfVisitable(w, {p.x, p.y + 1}));
  visitables.emplace_back(checkIfVisitable(w, {p.x - 1, p.y}));

  std::shuffle(std::begin(visitables), std::end(visitables), rng);

  return visitables;
}

std::vector<Point2D> HuntAndKill::getVisited(World* w, const Point2D& p) {
  std::vector<Point2D> visited;
  visited.emplace_back(checkIfVisited(w, {p.x, p.y - 1}));
  visited.emplace_back(checkIfVisited(w, {p.x + 1, p.y}));
  visited.emplace_back(checkIfVisited(w, {p.x, p.y + 1}));
  visited.emplace_back(checkIfVisited(w, {p.x - 1, p.y}));

  return visited;
}

Point2D HuntAndKill::checkIfVisitable(World* w, Point2D direction) {
  auto sideOver2 = w->GetSize() / 2;

  if ((abs(direction.x) <= sideOver2 && abs(direction.y) <= sideOver2) &&
      !visited[direction.y][direction.x] &&
      w->GetNorth({direction.x, direction.y})) {
    return {direction.x, direction.y};
  }

  return {INT_MAX, INT_MAX};
}

Point2D HuntAndKill::checkIfVisited(World* w, Point2D direction) {
  auto sideOver2 = w->GetSize() / 2;

  if ((abs(direction.x) <= sideOver2 && abs(direction.y) <= sideOver2) &&
      visited[direction.y][direction.x]) {
    return {direction.x, direction.y};
  }

  return {INT_MAX, INT_MAX};
}

bool HuntAndKill::walk(World* world) {
  if (currentPoint.x == INT_MAX && currentPoint.y == INT_MAX) {
    currentPoint = randomStartPoint(world);

    if (currentPoint.x == INT_MAX && currentPoint.y == INT_MAX) {
      return false;
    }
  }

  visited[currentPoint.y][currentPoint.x] = true;
  world->SetNodeColor(currentPoint, Color::Black);
  Point2D next = {INT_MAX, INT_MAX};

  for each (auto visit in getVisitables(world, currentPoint)) {
    if (visit.x != INT_MAX && visit.y != INT_MAX) {
      next = visit;
      break;
    }
  }

  if (next.x == INT_MAX && next.y == INT_MAX) {
    return false;
  }

  auto delta = next - currentPoint;
  if (delta.y == -1)  // north
    world->SetNorth(currentPoint, false);
  else if (delta.x == 1)  // east
    world->SetEast(currentPoint, false);
  else if (delta.y == 1)  // south
    world->SetSouth(currentPoint, false);
  else if (delta.x == -1)  // west
    world->SetWest(currentPoint, false);

  currentPoint = next;
  return true;
}

bool HuntAndKill::hunt(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  Point2D connection = {INT_MAX, INT_MAX};
  for (int y = -sideOver2; y <= sideOver2; y++) {
    for (int x = -sideOver2; x <= sideOver2; x++) {
      if (!visited[y][x]) {
        for each (auto neightbor in getVisited(world, Point2D(x, y))) {
          if (neightbor.x != INT_MAX && neightbor.y != INT_MAX) {
            connection = neightbor;
            currentPoint = Point2D(x, y);
            break;
          }
        }
      }
    }
  }

  if (connection.x == INT_MAX && connection.y == INT_MAX) {
    return false;
  }

  auto delta = connection - currentPoint;
  // std::cout << "Y: " << delta.y << " X: " << delta.x << std::endl;
  if (delta.y == -1)  // north
    world->SetNorth(currentPoint, false);
  else if (delta.x == 1)  // east
    world->SetEast(currentPoint, false);
  else if (delta.y == 1)  // south
    world->SetSouth(currentPoint, false);
  else if (delta.x == -1)  // west
    world->SetWest(currentPoint, false);

  return true;
}

bool HuntAndKill::Step(World* world) {
  bool walkBool = walk(world);
  bool huntBool = false;

  if (!walkBool) {
    // std::cout << "running" << std::endl;
    huntBool = hunt(world);
  }

  return walkBool || huntBool;
}

void HuntAndKill::Clear(World* world) {
  visited.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int y = -sideOver2; y <= sideOver2; y++) {
    for (int x = -sideOver2; x <= sideOver2; x++) {
      visited[y][x] = false;
    }
  }
}