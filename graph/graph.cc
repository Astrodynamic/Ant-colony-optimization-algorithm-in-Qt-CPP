#include "graph.h"

std::random_device Graph::rd;
std::default_random_engine Graph::generator(rd());
std::uniform_real_distribution<double> Graph::distribution(min_random_value,
                                                           max_random_value);

Graph::Graph() : m_matrix{nullptr}, m_dimension{} {}

Graph::Graph(unsigned dimension) : m_matrix{nullptr}, m_dimension{dimension} {
  InitMatrix(dimension);
}

Graph::Graph(Graph& graph) : m_dimension{graph.m_dimension} {
  InitMatrix(m_dimension);

  for (unsigned i = 0; i < m_dimension; ++i) {
    for (unsigned j = 0; j < m_dimension; ++j) {
      m_matrix[i][j] = graph(i, j);
    }
  }
}

Graph::~Graph() { DestroyMatrix(); }

void Graph::InitMatrix(unsigned dimension) {
  if (dimension) {
    m_matrix = new double*[dimension];
    for (unsigned i = 0U; i < dimension; ++i) {
      m_matrix[i] = new double[dimension]{};
    }
  }
}

void Graph::DestroyMatrix() {
  if (m_matrix) {
    for (unsigned i = 0U; i < m_dimension; ++i) {
      if (m_matrix[i]) {
        delete[] m_matrix[i];
      }
    }
    delete[] m_matrix;
  }
  m_matrix = nullptr;
}

unsigned Graph::GetDimension() const { return m_dimension; }

void Graph::SetDimension(const unsigned dimension) {
  DestroyMatrix();
  m_dimension = dimension;
  InitMatrix(dimension);
}

double& Graph::operator()(const unsigned row, const unsigned col) {
  return m_matrix[row][col];
}

const double& Graph::operator()(const unsigned row, const unsigned col) const {
  return m_matrix[row][col];
}

Graph& Graph::operator=(const Graph& other) {
  if (this != &other) {
    DestroyMatrix();
    m_dimension = other.m_dimension;
    InitMatrix(m_dimension);
    for (unsigned i = 0; i < m_dimension; ++i) {
      for (unsigned j = 0; j < m_dimension; ++j) {
        m_matrix[i][j] = other.m_matrix[i][j];
      }
    }
  }
  return *this;
}

Graph& Graph::operator*=(const double value) {
  for (unsigned i = 0U; i < m_dimension; ++i) {
    for (unsigned j = 0U; j < m_dimension; ++j) {
      m_matrix[i][j] *= value;
    }
  }
  return *this;
}

void Graph::Fill(double value) {
  for (unsigned i = 0U; i < m_dimension; ++i) {
    for (unsigned j = 0U; j < m_dimension; ++j) {
      m_matrix[i][j] = value;
    }
  }
}

void Graph::Generate(const unsigned dimension) {
  DestroyMatrix();
  m_dimension = dimension;
  InitMatrix(dimension);
  for (unsigned i = 0U; i < m_dimension; ++i) {
    for (unsigned j = i; j < m_dimension; ++j) {
      if (i != j) {
        m_matrix[i][j] = distribution(generator);
        m_matrix[j][i] = m_matrix[i][j];
      }
    }
  }
}

bool Graph::LoadGraphFromFile(std::string filename) {
  std::ifstream file(filename);

  bool flag = true;
  if (!file.is_open()) {
    std::cout << "File (" << filename << ") is not open" << std::endl;
    flag = false;
  } else {
    DestroyMatrix();

    file >> m_dimension;
    InitMatrix(m_dimension);
    for (unsigned row = 0U; row < m_dimension; ++row) {
      for (unsigned col = 0U; col < m_dimension; ++col) {
        file >> m_matrix[row][col];
      }
    }
  }
  file.close();
  return flag;
}

bool Graph::LoadGraphToFile(std::string filename) const {
  std::ofstream file(filename);

  bool flag = true;
  if (!file.is_open()) {
    std::cout << "File (" << filename << ") is not open" << std::endl;
    flag = false;
  } else {
    file << m_dimension << '\n';
    for (unsigned row = 0U; row < m_dimension; ++row) {
      for (unsigned col = 0U; col < m_dimension; ++col) {
        file << m_matrix[row][col] << ' ';
      }
      file << '\n';
    }
  }
  file.close();
  return flag;
}

bool Graph::ExportGraphToDot(std::string filename) const {
  std::ofstream file(filename);

  bool flag = true;
  if (!file.is_open()) {
    std::cout << "File (" << filename << ") is not open" << std::endl;
    flag = false;
  } else {
    file << "graph incidence_matrix {\n";
    for (unsigned i = 0U; i < m_dimension; ++i) {
      for (unsigned j = 0U; j < m_dimension; ++j) {
        if (m_matrix[i][j] > 0) {
          file << " " << i + 1 << " -- " << j + 1;
          file << " [weight=" << m_matrix[i][j] << "];\n";
        }
      }
    }
    file << "}\n";
  }
  file.close();
  return flag;
}
