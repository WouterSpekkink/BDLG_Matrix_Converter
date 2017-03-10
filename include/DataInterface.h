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
