#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtGui>
#include <QApplication>
#include <QDialog>
#include "DataInterface.h"
#include <vector>
#include <string>

class QPushButton;
class QComboBox;
class QLabel;
class QSpinBox;
class QCheckBox;
class QTextEdit;

class MainDialog : public QDialog {
  friend class DataInterface;
  friend class Logger;
  Q_OBJECT

public:
  MainDialog(QWidget *parent=0);

private slots:
  void initializeSeps();
  void getFile();
  void setSep(const QString &selection);
  void readNewData();
  void enableOptions();
  void writeMatrix();
  void writeNodes();
  void writeEdges();
  void finalBusiness();
  
private:
  QPointer<DataInterface> dataInterface;
  QPointer<QLabel> importLabel;
  QPointer<QLabel> exportLabel;
  QPointer<QPushButton> openFile;
  QPointer<QPushButton> importFile;
  QPointer<QPushButton> exitButton;
  QPointer<QPushButton> writeMatrixButton;
  QPointer<QPushButton> writeNodesButton;
  QPointer<QPushButton> writeEdgesButton;
  QPointer<QComboBox> sepSelector;
  QString fileName;
  QString sep;
  static const QString SEPDEFAULT;
};

#endif
