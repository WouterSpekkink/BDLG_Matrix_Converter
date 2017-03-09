#include <QApplication>
#include "../include/MainDialog.h"

int main(int argc, char *argv[]) 
{
  // Setting up the app
  QApplication MatrixConverter(argc, argv);

  MainDialog *dialog = new MainDialog;
  dialog->show();
  return MatrixConverter.exec();
}
