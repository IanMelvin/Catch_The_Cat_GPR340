#ifndef MOBAGEN_RANDOMSCENARIOGENERATOR_H
#define MOBAGEN_RANDOMSCENARIOGENERATOR_H

#include "../GeneratorBase.h"

class RandomScenarioGenerator: public ScenarioGeneratorBase{
 public:
  enum Biome {
    OCEAN = 0,
    BEACH = 1,
    GRASSLAND = 2,
    FORREST = 3,
    DESERT = 4,
    MOUNTAIN = 5
  };

  std::vector<Color32> Generate(int sideSize, float displacement=0) override;
  std::string GetName() override;
  float HeightFiltering(int noise, int x, int y, int sideSize);
  std::vector<Color32> AltitudeFiltering(std::vector<Color32> &input);
  Color32 Biomes(float x, float y, float avg);
  Color32 GetColor(Biome biome);
  int waterLevel = 90;
  int beachLevel = 91;
};

#endif  // MOBAGEN_RANDOMSCENARIOGENERATOR_H