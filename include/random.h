#pragma once
#include <random>
#include <glm/glm.hpp>

namespace Random
{
  static std::random_device rd;
  static std::mt19937_64 mt(rd());

  inline double real(double min, double max) { return std::uniform_real_distribution<double>(min, max)(rd); }
  inline int integer(int min, int max) { return std::uniform_int_distribution<int>(min, max)(rd); }
  inline int integer(int max) { return Random::integer(0, max); }
  inline bool flip(double chance) { return Random::real(0, 100) <= chance; }

  inline glm::vec2 vec2(glm::vec2 min, glm::vec2 max) { return { real(min.x, max.x), real(min.y, max.y) }; }
  inline glm::vec2 vec2(glm::vec2 max) { return vec2({ 0,0 }, max); }

  std::vector<glm::vec2> PoissonDisk(float radius, glm::vec2 region, int numSamplesBeforeRejection = 30);
}
