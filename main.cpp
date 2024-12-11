
#include <QApplication>
#include <QMainWindow>
#include "mainWindow.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QTranslator trn;
  if (trn.load(QLocale::system(), "window", "_", ":/i18n")) {
    app.installTranslator(&trn);
  }

  MainDashboard window;
  window.show();

  return app.exec();
}