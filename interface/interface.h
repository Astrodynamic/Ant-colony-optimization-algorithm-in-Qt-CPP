#ifndef PARALLELS_SRC_INTERFACE_INTERFACE_H_
#define PARALLELS_SRC_INTERFACE_INTERFACE_H_

#include <functional>
#include <string>
#include <vector>
#include <chrono>

#include "aco.h"

class Interface {
 public:
  Interface();
  ~Interface();

  void Exec();

 private:
  using tp = std::chrono::steady_clock::time_point;
  using ms = std::chrono::milliseconds;

  Graph m_graph;

  enum MenuItem {
    kIntroduction = 0,
    kMainMenu,
    kLoadGraph,
    kGenerateGraph,
    kIteration,
    kCompletion
  };

  enum MenuFuncs {
    kMainFuncMenu = 0,
    kLoadFromTxtMenu,
    kLoadToTxtMenu,
    kLoadToDotMenu,
    kGenerateMenu,
    kFillingMenu,
    kWithThreadMenu,
    kWithoutThreadMenu,
    kComparisonMenu,
    kMenuFuncsAll
  };

  static const std::vector<std::string> m_menus;
  std::vector<std::vector<std::function<bool(void)> > > m_menu_funcs;

  void InitMainFuncMenu();
  void InitLoadFromTxtMenu();
  void InitLoadToTxtMenu();
  void InitLoadToDotMenu();
  void InitGenerateMenu();
  void InitFillingMenu();
  void InitWithThreadMenu();
  void InitWithoutThreadMenu();
  void InitComparisonMenu();

  const std::size_t ShowMenu(const std::string &menu, const std::size_t items = 0U);
  const std::size_t CheckInputItem(const std::size_t count);

  bool Exit();
  bool RunMenu(std::vector<std::function<bool(void)> > & func, MenuItem menu);

  void ShowPath(TsmResult && path);
  void CheckRunningTime(AntNet &net, const unsigned iterations, const bool thread);
};

#endif  // PARALLELS_SRC_INTERFACE_INTERFACE_H_