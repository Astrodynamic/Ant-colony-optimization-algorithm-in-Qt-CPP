#include "aco.h"

std::random_device AntNet::rd;
std::default_random_engine AntNet::generator(rd());
std::uniform_real_distribution<double> AntNet::distribution(min_random_value, max_random_value);

AntNet::AntNet(Graph &distance, const double init_pheromon_value)
    : m_attractiveness{new Graph(distance.GetDimension())},
      m_pheromone{new Graph(distance.GetDimension())},
      m_distance{new Graph(distance)},
      m_visited{new Graph(distance.GetDimension())} {
  CalculateAttractiveness(distance);
  m_pheromone->Fill(init_pheromon_value);
  m_ants.resize(distance.GetDimension(), {});
}

AntNet::~AntNet() {
  delete m_attractiveness;
  delete m_pheromone;
  delete m_distance;
  delete m_visited;
}

TsmResult AntNet::AntColonyAlgorithm(unsigned iterations) {
  TsmResult result(INFINITY);

  if (m_thread_flag) {
    for (unsigned i = 0U; i < iterations; ++i, ++m_iterations) {
      AntMovement();
      std::thread th_up_pheromone(&AntNet::UpdatePheromone, this);
      std::thread th_set_min_path(&TsmResult::SetMinPath, &result, std::move(BestPath()));
      th_up_pheromone.join();
      th_set_min_path.join();
    }
  } else {
    for (unsigned i = 0U; i < iterations; ++i, ++m_iterations) {
      AntMovement();
      UpdatePheromone();
      result.SetMinPath(BestPath());
    }
  }
  return result;
}

const unsigned AntNet::GetIterations() const { return m_iterations; }

void AntNet::CalculateAttractiveness(Graph &distance) {
  for (unsigned i = 0U; i < distance.GetDimension(); ++i) {
    for (unsigned j = 0U; j < distance.GetDimension(); ++j) {
      if (distance(i, j) > 0.0) {
        m_attractiveness->operator()(i, j) = 1.0 / distance(i, j);
      }
    }
  }
}

void AntNet::ClearAntsData(TsmResult &ant) {
  ant.vertices.clear();
  ant.distance = {};
}

TsmResult AntNet::BestPath() const {
  unsigned idx{};
  double min_distance = m_ants[idx].distance;
  for (unsigned i = 1U; i < m_ants.size(); ++i) {
    if (min_distance > m_ants[i].distance) {
      min_distance = m_ants[i].distance;
      idx = i;
    }
  }
  return m_ants[idx];
}

void AntNet::AntMovement() {
  for (unsigned ant = 0U; ant < m_ants.size(); ++ant) {
    ClearAntsData(m_ants[ant]);
    unsigned location = ant;
    for (unsigned city = 0U; city < m_visited->GetDimension(); ++city) {
      m_visited->operator()(ant, location) = 1.0;
      m_ants[ant].vertices.push_back(location);
      location = CalcProbability(ant, location);
      m_ants[ant].distance += m_distance->operator()(m_ants[ant].vertices.back(), location);
    }
    m_ants[ant].vertices.push_back(ant);
    m_ants[ant].distance += m_distance->operator()(location, ant);
  }
  m_visited->Fill(0.0);
}

double AntNet::CalcSumAttractivenessPheromone(unsigned ant, unsigned location) {
  double sum{};
  for (unsigned city = 0U; city < m_visited->GetDimension(); ++city) {
    if (m_visited->operator()(ant, city) < 0.5) {
      double tau = std::pow(m_pheromone->operator()(location, city), kAlpha);
      double eta = std::pow(m_attractiveness->operator()(location, city), kBeta);
      sum += tau * eta;
    }
  }
  return sum;
}

unsigned AntNet::CalcProbability(unsigned ant, unsigned location) {
  double mark = distribution(generator);
  double mark_sum{};

  double sum = CalcSumAttractivenessPheromone(ant, location);
  for (unsigned city = 0U; city < m_visited->GetDimension(); ++city) {
    if (m_visited->operator()(ant, city) < 0.5) {
      double tau = std::pow(m_pheromone->operator()(location, city), kAlpha);
      double eta = std::pow(m_attractiveness->operator()(location, city), kBeta);
      if (mark_sum += tau * eta / sum, mark < mark_sum) {
        location = city;
        break;
      }
    }
  }

  return location;
}

void AntNet::UpdatePheromone() {
  m_pheromone->operator*=(kGamma);
  for (unsigned ant = 0U; ant < m_ants.size(); ++ant) {
    double delta = 1.0 / m_ants[ant].distance;
    for (unsigned i = 1U; i < m_ants[ant].vertices.size(); ++i) {
      unsigned row = m_ants[ant].vertices[i - 1];
      unsigned col = m_ants[ant].vertices[i];
      m_pheromone->operator()(row, col) += delta;
    }
  }
}

void AntNet::SetThreadFlag(const bool flag) {
  m_thread_flag = flag;
}