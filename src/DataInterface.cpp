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

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <QPointer>
#include "../include/DataInterface.h"
#include <iostream>

DataInterface::DataInterface()  
{
}

void DataInterface::readFile(const QString &fileName, const QString &sep) 
{
  // Let's first make sure that our current DataInterface is clean.
  header.clear();
  rowNames.clear();
  rowData.clear();
  labels.clear();
  newMatrix.clear();

  const std::string inputFile = fileName.toStdString();
  std::string sepString = sep.toStdString();
  std::istringstream convert(sepString.c_str());
  char sepChar;
  convert >> sepChar;
    
  std::vector <std::vector <std::string> > dataVector;

  std::ifstream myFile (inputFile.c_str());

  while(myFile) {
    std::string buffer;
    if (!getline(myFile, buffer)) break;
    std::istringstream stringStream(buffer);
    std::vector <std::string> record;

    while(stringStream) {
      std::string s;
      if (!getline(stringStream, s, sepChar)) break;
      record.push_back(s);
    }
    dataVector.push_back(record);
  }

  std::vector<std::vector <std::string> >::iterator it;
  for (it = dataVector.begin(); it != dataVector.end(); it++) {
    if (it == dataVector.begin()) {
      std::vector<std::string> tempVector = *it;
      std::vector<std::string>::iterator it2;
      for (it2 = tempVector.begin() + 1; it2 != tempVector.end(); it2++) {
	header.push_back(*it2);
      }
    } else {
      std::vector<std::string> tempVector = *it;
      rowNames.push_back(tempVector[0]);
      std::vector<std::string>::iterator it2;
      std::vector<short> numbers;
      for (it2 = tempVector.begin() + 1; it2 != tempVector.end(); it2++) {
	short tempNumber = 0;
	std::istringstream convert(*it2);
	convert >> tempNumber;
	numbers.push_back(tempNumber);
      }
      rowData.push_back(numbers);
    }
  }

  // Make the labels for the new matrix.
  std::vector <std::string>::iterator rIt;
  std::vector <std::string>::iterator hIt;
  for (rIt = rowNames.begin(); rIt != rowNames.end(); rIt++) {
    for (hIt = header.begin(); hIt != header.end(); hIt++) {
      std::string s1 = *rIt;
      std::string::iterator et;
      for (et = s1.begin(); et != s1.end();) {
	if (*et == '"') {
	  s1.erase(et);
	} else {
	  et++;
	}
      }
      std::string s2 = *hIt;
      for (et = s2.begin(); et != s2.end();) {
	if (*et == '"') {
	  s2.erase(et);
	} else {
	  et++;
	}
      }      
      std::string currentLabel = s1 + char('-') + s2;
      labels.push_back(currentLabel);
    }
  }

  // Make the new matrix (empty version).
  for (std::vector<std::string>::size_type i = 0; i != labels.size(); i++) {
    std::vector <short> currentRow;
    for (std::vector<std::string>::size_type j = 0; j != labels.size(); j++) {
      currentRow.push_back(0);
    }
    newMatrix.push_back(currentRow);
  }
  
  // Two for-loops to fill the new matrix.
  for (std::vector <std::vector <short> >::size_type i = 0; i != rowData.size(); i++) {
    std::vector <short> currentRow = rowData[i];
    for (std::vector<short>::size_type j = 0; j != currentRow.size(); j++) {
      if (currentRow[j] == 1 && j != currentRow.size() - 1) {
	bool firstFound = false;
	for (std::vector<short>::size_type k = (j + 1); k != currentRow.size(); k++) {
	  if (currentRow[k] == 1 && !firstFound) {
	    firstFound = true;
	    int a = i;
	    int b = currentRow.size();
	    int c = j;
	    int d = k;
	    int indexOne = (a * b) + c;
	    int indexTwo = (a * b) + d;
	    newMatrix[indexOne][indexTwo] = 1;
	  }
	}
      }
    }
  }
  
  for (std::vector <std::vector <short> >::size_type i = 0; i != rowData.size(); i++) {
    std::vector <short> currentRow = rowData[i];
    for (std::vector<short>::size_type j = 0; j != currentRow.size(); j++) {
      if (currentRow[j] == 1 && i != rowData.size() - 1) {
	for (std::vector <std::vector <short> >::size_type k = i + 1; k !=  rowData.size(); k++) {
	  std::vector <short> newRow = rowData[k];
	  if (newRow[j] == 1) {
	    int a = i;
	    int b = currentRow.size();
	    int c = j;
	    int d = k;
	    int indexOne = (a * b) + c;
	    int indexTwo = (d * b) + c;
	    newMatrix[indexOne][indexTwo] = 1;
	    newMatrix[indexTwo][indexOne] = 1;
	  }
	}
      }
    }
  }
 
  emit importFinished();
  return;
}

// The following function writes the linkages to a file.
void DataInterface::writeMatrix(const QString &fileName, const QString &sep) {
  std::string saveFile = fileName.toStdString();
  std::string sepString = sep.toStdString();
  std::istringstream convert(sepString.c_str());
  char sepChar;
  convert >> sepChar;

  std::vector <int> indexes;
  
  for (std::vector <std::vector <short> >::size_type i = 0; i != newMatrix.size(); i++) {
    std::vector <short> currentRow = newMatrix[i];					       
    int sum = 0;
    for (std::vector <short>::size_type j = 0; j != currentRow.size(); j++) {
      if (currentRow[j] == 1) {
	sum++;
      }
    }
    if (sum > 0) {
      indexes.push_back(1);
    } else {
      indexes.push_back(0);
    }
  }
  
  std::ofstream fileOut(saveFile.c_str());

  fileOut << sepChar;
  for (std::vector <std::string>::size_type i = 0; i !=  labels.size(); i++) {
    if (i != labels.size() - 1) {
      if (indexes[i] != 0) {
	fileOut << labels[i] << sepChar;
      }
    } else {
      fileOut << labels[i] << "\n";
    }
  }

  std::vector<std::string>::iterator qIt = labels.begin();
  for (std::vector <std::vector <short> >::size_type i = 0; i != newMatrix.size(); i++) {
    if (indexes[i] != 0) {
      fileOut << *qIt << sepChar;
      std::vector<short> currentRow = newMatrix[i];
      std::vector <short>::iterator it;
      for (std::vector<short>::size_type j = 0; j != currentRow.size(); j++) {
	if (j != currentRow.size() - 1) {
	     if(indexes[j] != 0) {
	       fileOut << currentRow[j] << sepChar;
	     }
	} else {
	  fileOut << currentRow[j] << "\n";
	}
      }
    }
    qIt++;
  }
  fileOut.close();
}

void DataInterface::writeNodes(const QString &fileName, const QString &sep) {
  std::string saveFile = fileName.toStdString();
  std::string sepString = sep.toStdString();
  std::istringstream convert(sepString.c_str());
  char sepChar;
  convert >> sepChar;

  std::ofstream fileOut(saveFile.c_str());

  fileOut << "Id" << sepChar << "Label" << sepChar << "Actor" << sepChar
	  << "Order_Original" << sepChar << "Order_Closed\n";

  // It is easier hear to redo the labelling part and immediately create order variables.
  // Make the labels for the new matrix.
  std::vector <std::string> newLabels;
  std::vector <std::string>::iterator rIt;
  std::vector <std::string>::iterator hIt;
  
  std::vector <std::string> events;
  std::vector <std::string> actors;
  std::vector <int> closedOrder;
  std::string previousEvent = "";
  // These will include many duplicates, but it is of the same length as the labels vector.
  for (rIt = rowNames.begin(); rIt != rowNames.end(); rIt++) {
    int order = 0;
    for (hIt = header.begin(); hIt != header.end(); hIt++) {
      if (previousEvent != *hIt) {
	order++;
	previousEvent =  *hIt;
      }
      closedOrder.push_back(order);
      events.push_back(*hIt);
      actors.push_back(*rIt);
    }
  }

  // Now we find the nodes that we want to skip when writing the list.
  std::vector <int> indexes;
  for (std::vector <std::vector <short> >::size_type i = 0; i != newMatrix.size(); i++) {
    std::vector <short> currentRow = newMatrix[i];					       
    int sum = 0;
    for (std::vector <short>::size_type j = 0; j != currentRow.size(); j++) {
      if (currentRow[j] == 1) {
	sum++;
      }
    }
    if (sum > 0) {
      indexes.push_back(1);
    } else {
      indexes.push_back(0);
    }
  }

  // Now we write the list.
  for (std::vector <std::string>::size_type i = 0; i != labels.size(); i++) {
    int currentIndex = i;
    if (indexes[currentIndex] != 0) {
      fileOut << labels[i] << sepChar << labels[i] << sepChar << actors[i] << sepChar
	      << events[i] << sepChar << closedOrder[currentIndex] << "\n";
	
    }
  }

  fileOut.close();
}

void DataInterface::writeEdges(const QString &fileName, const QString &sep) {
  std::string saveFile = fileName.toStdString();
  std::string sepString = sep.toStdString();
  std::istringstream convert(sepString.c_str());
  char sepChar;
  convert >> sepChar;

  std::ofstream fileOut(saveFile.c_str());

  fileOut << "Source" << sepChar << "Target" << sepChar << "Type"  << sepChar << "Weight\n";

  for (std::vector <std::vector <short> >::size_type i = 0; i != newMatrix.size(); i++) {
    std::vector <short> currentRow = newMatrix[i];
    int rowIndex = i;
    for (std::vector <short>::size_type j =0; j != currentRow.size(); j++) {
      int colIndex = j;
      // Access labels with index!!!
      if (currentRow[j] == 1) {
	fileOut << labels[rowIndex] << sepChar << labels[colIndex]
		<< sepChar << "Directed" << sepChar << 1 << "\n";
      }
    }
  }
  fileOut.close();
}

