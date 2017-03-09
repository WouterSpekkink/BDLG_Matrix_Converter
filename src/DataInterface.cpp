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

  std::vector <std::string> labels;
  std::vector <std::string>::iterator rIt;
  std::vector <std::string>::iterator hIt;
  for (rIt = rowNames.begin(); rIt != rowNames.end(); rIt++) {
    for (hIt = header.begin(); hIt != header.end(); hIt++) {
      std::string s1 = *rIt;
      std::string s2 = *hIt;
      std::string currentLabel =  s1 + s2;
      labels.push_back(currentLabel);
    }
  }

  std::vector <std::vector <short> > newMatrix;
  for (std::vector<std::string>::size_type i = 0; i != labels.size(); i++) {
    std::vector <short> currentRow;
    for (std::vector<std::string>::size_type j = 0; j != labels.size(); j++) {
      currentRow.push_back(0);
    }
    newMatrix.push_back(currentRow);
  }

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
    if (indexes[i] != 0) {
      if (i != labels.size() - 1) {
	fileOut << labels[i] << sepChar;
      } else {
	fileOut << labels[i] << "\n";
      }
    }
  }

  
  std::vector<std::string>::iterator qIt = labels.begin();
  for (std::vector <std::vector <short> >::size_type i = 0; i != newMatrix.size(); i++) {
    if (indexes[i] != 0) {
      fileOut << *qIt << sepChar;
      std::vector<short> currentRow = newMatrix[i];
      std::vector <short>::iterator it;
      for (std::vector<short>::size_type j = 0; j != currentRow.size(); j++) {
	if(indexes[j] != 0) {
	   if (j != currentRow.size() - 1) {
	     fileOut << currentRow[j] << sepChar;
	   } else {
	     fileOut << currentRow[j] << "\n";
	   }
	}
      }
    }
    qIt++;
  }
  
  fileOut.close();

}



