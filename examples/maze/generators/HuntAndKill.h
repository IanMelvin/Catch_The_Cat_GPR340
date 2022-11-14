#ifndef HUNTANDKILL_H
#define HUNTANDKILL_H

#include "../MazeGeneratorBase.h"
#include "Point2D.h"
#include <chrono>
#include <map>
#include <random>
#include <vector>

class HuntAndKill : public MazeGeneratorBase {
 private:
  Point2D currentPoint = {INT_MAX, INT_MAX};
  std::map<int, std::map<int, bool>> visited;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rng{seed};

  Point2D randomStartPoint(World* world);
  std::vector<Point2D> getVisitables(World* w, const Point2D& p);
  std::vector<Point2D> getVisited(World* w, const Point2D& p);
  Point2D checkIfVisitable(World* w, Point2D direction);
  Point2D checkIfVisited(World* w, Point2D direction);
  bool walk(World* world);
  bool hunt(World* world);

 public:
  HuntAndKill() = default;
  std::string GetName() override { return "Hunt & Kill"; };
  bool Step(World* world) override;
  void Clear(World* world) override;
};

#endif  // HUNTANDKILL_H