/*
 Copyright 2017 Wouter Spekkink
 Authors : Wouter Spekkink <wouter.spekkink@gmail.com>
 Website : http://www.wouterspekkink.org

 This file is part of the BDLG_Matrix_Converter.

 DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.

 Copyright 2017 Wouter Spekkink. All rights reserved.

 The BDLF_Matrix_Converter is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 The BDLG_Matrix_Converter is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 #
 You should have received a copy of the GNU General Public License
 along with the BDLG_Matrix_Converter.  If not, see <http://www.gnu.org/licenses/>.
*/

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
