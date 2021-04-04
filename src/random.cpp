#include "random.h"
#include "grid.h"

std::vector<glm::vec2> Random::PoissonDisk(float radius, glm::vec2 region, int numSamplesBeforeRejection) {
  float cellSize = radius/sqrt(2);
  int width = std::ceil(region.x/cellSize);
  int height = std::ceil(region.y/cellSize);

  std::vector<glm::vec2> points;
  std::vector<glm::vec2> spawnPoints;
  std::vector<std::vector<int>> grid(width, std::vector(height, 0));

  auto IsValid = [&](glm::vec2 candidate) {
    if (candidate.x >= 0 && candidate.x < region.x && candidate.y >= 0 && candidate.y < region.y) {
      int cellX = candidate.x / cellSize;
      int cellY = candidate.y / cellSize;
      int searchStartX = std::max(cellX - 2, 0);
      int searchEndX = std::min(cellX + 2, width - 1);
      int searchStartY = std::max(cellY - 2, 0);
      int searchEndY = std::min(cellY + 2, height - 1);

      for (int i = searchStartX; i <= searchEndX; ++i) {
        for (int j = searchStartY; j <= searchEndY; ++j) {
          int pointIndex = grid[i][j] - 1;
          if (pointIndex != -1) {
            float dist = glm::distance(candidate, points[pointIndex]);
            if (dist < radius) {
              return false;
            }
          }
        }
      }
      return true;
    }
    return false;
  };

  spawnPoints.push_back(region/2.0f);
  while (spawnPoints.size() > 0) {
    int spawnIndex = Random::real(0.0, spawnPoints.size());
    glm::vec2 spawnCenter = spawnPoints[spawnIndex];

    bool candidateAccepted = false;
    for (int i = 0; i < numSamplesBeforeRejection; ++i) {
      float angle = Random::real(0.0, 1.0) * 2 * M_PI;
      glm::vec2 dir{ sin(angle), cos(angle) };
      glm::vec2 candidate = spawnCenter + dir * (float)Random::real(radius, 2*radius);
      if (IsValid(candidate)) {
        points.push_back(candidate);
        spawnPoints.push_back(candidate);
        grid[candidate.x/cellSize][candidate.y/cellSize] = points.size();
        candidateAccepted = true;
        break;
      }
    }

    if (!candidateAccepted) {
      spawnPoints.erase(spawnPoints.begin() + spawnIndex);
    }
  }
  return points;
}

