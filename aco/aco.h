#ifndef ANT_COLONY_OPTIMIZATION_ALGORITHM_ACO_ACO_H_
#define ANT_COLONY_OPTIMIZATION_ALGORITHM_ACO_ACO_H_

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "graph.h"

struct TsmResult {
  std::vector<unsigned> vertices;
  double distance{};

  TsmResult(const double distance = {}) : distance{distance} {}

  void SetMinPath(TsmResult &&path) {
    if (distance > path.distance) {
      *this = path;
    }
  }

  void CalcDistance(const Graph &graph) {
    distance = {};
    for (unsigned i = 1U; i < vertices.size(); ++i) {
      distance += graph(vertices[i - 1], vertices[i]);
    }
  }
};

class AntNet {
 public:
  AntNet(Graph &distance, const double init_pheromon_value);
  ~AntNet();

  TsmResult AntColonyAlgorithm(unsigned iterations);
  unsigned GetIterations() const;

  void SetThreadFlag(const bool flag);

 private:
  static constexpr double kAlpha = 1.0;
  static constexpr double kBeta = 4.0;
  static constexpr double kGamma = 0.9;

  Graph *m_attractiveness;
  Graph *m_pheromone;
  Graph *m_distance;
  Graph *m_visited;

  std::vector<TsmResult> m_ants;

  unsigned m_iterations = 0U;

  static constexpr double min_random_value = +0.0;
  static constexpr double max_random_value = +1.0;

  static std::random_device rd;
  static std::default_random_engine generator;
  static std::uniform_real_distribution<double> distribution;

  bool m_thread_flag = false;

  void AntMovement();
  static void ClearAntsData(TsmResult &ant);

  void CalculateAttractiveness(Graph &distance);
  unsigned CalcProbability(unsigned ant, unsigned location);
  double CalcSumAttractivenessPheromone(unsigned ant, unsigned location);

  void UpdatePheromone();

  TsmResult BestPath() const;
};

#endif  // ANT_COLONY_OPTIMIZATION_ALGORITHM_ACO_ACO_H_
