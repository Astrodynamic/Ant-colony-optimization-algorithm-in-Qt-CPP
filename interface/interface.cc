#include "interface.h"

const std::vector<std::string> Interface::m_menus{
    " -------------------------------------------------------------- \n"
    "|                     Simple Navigator 1.0                     |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 1. Exit                                                      |\n"
    "| 2. Loading a graph from a file                               |\n"
    "| 3. Loading a graph to a file                                 |\n"
    "| 4. Exporting the graph to a file (.dot)                      |\n"
    "| 5. Genarate graph randomly                                   |\n"
    "| 6. Genarate graph manually                                   |\n"
    "| 7. Solving the traveling salesman problem using              |\n"
    "|    the ant colony algorithm without multithreading           |\n"
    "| 8. Solving the traveling salesman problem using              |\n"
    "|    the ant colony algorithm with multithreading              |\n"
    "| 9. Comparison methods solving problem                        |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 1. Exit                                                      |\n"
    "| 2. Enter the full path to the graph file ...                 |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 1. Exit                                                      |\n"
    "| 2. Enter the number of towns (1 - 10'000) ...                |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 1. Exit                                                      |\n"
    "| 2. Enter the count of iterations algorithm ...               |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|            Successful completion of the programme            |\n"
    " -------------------------------------------------------------- \n"};

Interface::Interface() {
  m_menu_funcs.resize(MenuFuncs::kMenuFuncsAll);

  InitMainFuncMenu();
  InitLoadFromTxtMenu();
  InitLoadToTxtMenu();
  InitLoadToDotMenu();
  InitGenerateMenu();
  InitFillingMenu();
  InitWithThreadMenu();
  InitWithoutThreadMenu();
  InitComparisonMenu();

  ShowMenu(m_menus[MenuItem::kIntroduction]);
}

void Interface::InitMainFuncMenu() {
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::RunMenu, this, std::ref(m_menu_funcs[MenuFuncs::kLoadFromTxtMenu]), MenuItem::kLoadGraph));
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::RunMenu, this, std::ref(m_menu_funcs[MenuFuncs::kLoadToTxtMenu]), MenuItem::kLoadGraph));
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::RunMenu, this, std::ref(m_menu_funcs[MenuFuncs::kLoadToDotMenu]), MenuItem::kLoadGraph));
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::RunMenu, this, std::ref(m_menu_funcs[MenuFuncs::kGenerateMenu]), MenuItem::kGenerateGraph));
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::RunMenu, this, std::ref(m_menu_funcs[MenuFuncs::kFillingMenu]), MenuItem::kGenerateGraph));
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::RunMenu, this, std::ref(m_menu_funcs[MenuFuncs::kWithThreadMenu]), MenuItem::kIteration));
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::RunMenu, this, std::ref(m_menu_funcs[MenuFuncs::kWithoutThreadMenu]), MenuItem::kIteration));
  m_menu_funcs[MenuFuncs::kMainFuncMenu].push_back(std::bind(&Interface::RunMenu, this, std::ref(m_menu_funcs[MenuFuncs::kComparisonMenu]), MenuItem::kIteration));
}

void Interface::InitLoadFromTxtMenu() {
  m_menu_funcs[MenuFuncs::kLoadFromTxtMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kLoadFromTxtMenu].push_back([&]() -> bool {
    std::string filename;
    std::getline(std::cin, filename);
    return !m_graph.LoadGraphFromFile(filename);
  });
}

void Interface::InitLoadToTxtMenu() {
  m_menu_funcs[MenuFuncs::kLoadToTxtMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kLoadToTxtMenu].push_back([&]() -> bool {
    std::string filename;
    std::getline(std::cin, filename);
    return !m_graph.LoadGraphToFile(filename);
  });
}

void Interface::InitLoadToDotMenu() {
  m_menu_funcs[MenuFuncs::kLoadToDotMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kLoadToDotMenu].push_back([&]() -> bool {
    std::string filename;
    std::getline(std::cin, filename);
    return !m_graph.ExportGraphToDot(filename);
  });
}

void Interface::InitGenerateMenu() {
  m_menu_funcs[MenuFuncs::kGenerateMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kGenerateMenu].push_back([&]() -> bool {
    m_graph.Generate(CheckInputItem(10'000U));
    return false;
  });
}

void Interface::InitFillingMenu() {
  m_menu_funcs[MenuFuncs::kFillingMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kFillingMenu].push_back([&]() -> bool {
    m_graph.SetDimension(CheckInputItem(10'000U));
    bool flag{true};
    for (unsigned i = 0; flag && i < m_graph.GetDimension(); ++i) {
      for (unsigned j = 0; flag && j < m_graph.GetDimension(); ++j) {
        if (!(std::cin >> m_graph(i, j))) {
          std::cin.clear();
          flag = false;
        }
      }
    }
    return !flag;
  });
}

void Interface::InitWithThreadMenu() {
  m_menu_funcs[MenuFuncs::kWithThreadMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kWithThreadMenu].push_back([&]() -> bool {
    bool flag{static_cast<bool>(m_graph.GetDimension())};
    if (flag) {
      AntNet net(m_graph, 0.5);
      net.SetThreadFlag(true);
      ShowPath(net.AntColonyAlgorithm(CheckInputItem(10'000U)));
    } else {
      std::cout << "You need load gpaph\n";
    }
    return !flag;
  });
}

void Interface::InitWithoutThreadMenu() {
  m_menu_funcs[MenuFuncs::kWithoutThreadMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kWithoutThreadMenu].push_back([&]() -> bool {
    bool flag{static_cast<bool>(m_graph.GetDimension())};
    if (flag) {
      AntNet net(m_graph, 0.5);
      net.SetThreadFlag(false);
      ShowPath(net.AntColonyAlgorithm(CheckInputItem(10'000U)));
    } else {
      std::cout << "You need load gpaph\n";
    }
    return !flag;
  });
}

void Interface::ShowPath(TsmResult && path) {
  std::cout << "Distance of the path: " << path.distance << "\n";
  std::cout << "Path: ";
  for (unsigned i = 0; i < path.vertices.size(); ++i) {
    std::cout << path.vertices[i] << ((i < path.vertices.size() - 1) ?  "->" : "");
  }
  std::cout << "\n";
}

void Interface::InitComparisonMenu() {
  m_menu_funcs[MenuFuncs::kComparisonMenu].push_back(std::bind(&Interface::Exit, this));
  m_menu_funcs[MenuFuncs::kComparisonMenu].push_back([&]() -> bool {
    bool flag{static_cast<bool>(m_graph.GetDimension())};
    if (flag) {
      AntNet net(m_graph, 0.5);
      unsigned iterations = CheckInputItem(10'000U);
      CheckRunningTime(net, iterations, true);
      CheckRunningTime(net, iterations, false);
    } else {
      std::cout << "Incorrect gpaph\n";
    }

    return !flag;
  });
}

void Interface::CheckRunningTime(AntNet &net, const unsigned iterations, const bool thread) {
  net.SetThreadFlag(thread);
  tp begin = std::chrono::steady_clock::now();
  TsmResult result{net.AntColonyAlgorithm(iterations)};
  tp end = std::chrono::steady_clock::now();

  std::cout << "Run with" << (thread ? "" : "out") << "multithreading.\n";

  std::cout << "\nTime of the running(sec) = ";

  std::cout << std::chrono::duration_cast<ms>(end - begin).count() / 1000.0 << "\n";
}

Interface::~Interface() { ShowMenu(m_menus[MenuItem::kCompletion]); }

bool Interface::Exit() { return false; }

void Interface::Exec() {
  RunMenu(m_menu_funcs[MenuFuncs::kMainFuncMenu], MenuItem::kMainMenu);
}

const std::size_t Interface::ShowMenu(const std::string &menu, const std::size_t items) {
  std::cout << menu;
  return items ? CheckInputItem(items) : 0;
}

const std::size_t Interface::CheckInputItem(const std::size_t count) {
  std::string line;
  std::getline(std::cin, line);

  int result;
  while (!sscanf(line.c_str(), "%d", &result) || result <= 0 || result > count) {
    std::cout << "Incorrect input, try again: ";
    std::getline(std::cin, line);
  }

  return result;
}

bool Interface::RunMenu(std::vector<std::function<bool(void)> > & func, MenuItem menu) {
  bool flag{};
  std::size_t item{};
  std::size_t items{func.size()};
  do {
    item = ShowMenu(m_menus[menu], items) - 1;
  } while (items && (flag = func[item]()));

  return !flag;
}