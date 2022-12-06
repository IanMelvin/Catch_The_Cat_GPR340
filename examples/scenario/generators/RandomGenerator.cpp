#include "RandomGenerator.h"
#include "../FastNoiseLite.h"
#include "../PerlinNoise.hpp"
#include <iostream>
// do not use this one to your assignment. this is my sample generator
std::vector<Color32> RandomScenarioGenerator::Generate(int sideSize, float displacement) {
  std::vector<Color32> colors;
//   create your own function for noise generation
  FastNoiseLite base;
  base.SetFractalOctaves(3);
  base.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
  base.SetFractalType(FastNoiseLite::FractalType_FBm);
  FastNoiseLite cellular;
  cellular.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
  cellular.SetFractalOctaves(3);
  for(int l=0; l<sideSize; l++){
    for(int c=0;c<sideSize; c++){
      float c1 = ((base.GetNoise((float)c,(float)l, displacement*50) +1)/2) *255;
      float c2 = ((cellular.GetNoise((float)c,(float)l, displacement*50) +1)/2) *255;
      auto avg = (c1+c2)/2;
      //colors.emplace_back(avg, avg, avg);
      colors.emplace_back(Biomes(c1,c2,avg));
    }
  }
  std::cout<<colors.size() << std::endl;
  return colors;  // AltitudeFiltering(colors);
}
std::string RandomScenarioGenerator::GetName() { return "EXAMPLE"; }

float RandomScenarioGenerator::HeightFiltering(
    int noise, int x, int y, int sideSize) {
  float nx = ((2 * x) / (sideSize)) - 1;
  float ny = ((2 * y) / (sideSize)) - 1;
  float distance = 1 - (1 - (nx * nx)) * (1 - (ny * ny));
  float newElevation = (noise + (1 - (distance))) / 2;

  return newElevation;
}

std::vector<Color32> RandomScenarioGenerator::AltitudeFiltering(
    std::vector<Color32>& input) {
  std::vector<Color32> modifiedColors;
  for (Color32 var : input) {
    if (var.r < waterLevel) {
      modifiedColors.emplace_back(Color::Blue.Dark());
    } else if (var.r < beachLevel) {
      modifiedColors.emplace_back(Color::SandyBrown.Light());
    } else {
      modifiedColors.emplace_back(Color::Green.Light());
    }
  }
  return modifiedColors;
}



Color32 RandomScenarioGenerator::Biomes(float x, float y, float avg) {
  Biome biome;

  if (avg < waterLevel) return GetColor(OCEAN);
  if (avg < beachLevel) return GetColor(BEACH);

  if (avg > 120) return GetColor(MOUNTAIN);

  if (avg < 120) {
    if (y < 20) {
      return GetColor(DESERT);
    } else if (y < 60) {
      return GetColor(GRASSLAND);
    }
    else if (y < 100) {
      return GetColor(FORREST);
    }
  }

  biome = GRASSLAND;

  return GetColor(biome);
}

Color32 RandomScenarioGenerator::GetColor(Biome biome) {
  Color32 color;
  switch (biome) {
    case OCEAN:
      return Color::Blue.Dark();
    case BEACH:
      return Color::SandyBrown.Light();
    case GRASSLAND:
      return Color::Green.Light();
    case FORREST:
      return Color::Green.Dark();
    case DESERT:
      return Color::SandyBrown.Light();
    case MOUNTAIN:
      return Color::Gray;
    default:
      return Color::Black;
  }
}