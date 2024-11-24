
#include <QApplication>
#include <QMainWindow>
#include "mainWindow.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  MainDashboard window;
  window.show();

  return app.exec();
}