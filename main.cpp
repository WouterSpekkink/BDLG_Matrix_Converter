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


#include <QApplication>
#include "./include/MainDialog.h"

int main(int argc, char *argv[]) 
{
  // Setting up the app
  QApplication MatrixConverter(argc, argv);

  QString version = QString("1.0.0");
  MatrixConverter.setApplicationVersion(version);

  MainDialog *dialog = new MainDialog;
  dialog->show();
  return MatrixConverter.exec();
}
