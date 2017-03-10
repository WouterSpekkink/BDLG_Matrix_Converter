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

#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>
#include <string>
#include <vector>

class DataInterface : public QObject {
  friend class MainDialog;
  Q_OBJECT

public:
  DataInterface();
  void readFile(const QString &fileName, const QString &sep);
  void writeMatrix(const QString &fileName, const QString &sep);
  void writeNodes(const QString &fileName, const QString &sep);
  void writeEdges(const QString &fileName, const QString &sep);
  ~DataInterface() {}
  
signals:
  void importFinished();
		       
private:
  std::vector<std::string> header; 
  std::vector<std::string> rowNames;
  std::vector<std::string> labels;
  std::vector<std::vector <short> > rowData;
  std::vector<std::vector <short> > newMatrix;
};
#endif
