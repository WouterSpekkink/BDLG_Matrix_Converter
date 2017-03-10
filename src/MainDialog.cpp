#include <QtGui>

#include "../include/MainDialog.h"
#include <QFileDialog>
#include <QMessageBox>

// Initializing some constants
const QString MainDialog::SEPDEFAULT = QString("-Select delimiter-");

// This is the constructor for this class
MainDialog::MainDialog(QWidget *parent) : QDialog(parent) {
  dataInterface = new DataInterface();
  importLabel = new QLabel(tr("<h3>Import incidence matrix</h3>"));
  openFile = new QPushButton(tr("Select file")); // Button to select filename
  sepSelector = new QComboBox(this); // Combobox to select delimiter for columns.
  sepSelector->setEnabled(false); // This is initially disabled.
  initializeSeps();
  sep = SEPDEFAULT;
  importFile = new QPushButton(tr("Import matrix")); // Button to start importing files.
  importFile->setEnabled(false); // This is initially disabled.
  exitButton = new QPushButton(tr("Exit program")); // The exit button
  exportLabel = new QLabel(tr("<h3>Export BDLG matrix</h3>"));
  writeMatrixButton = new QPushButton(tr("Export matrix"));
  writeMatrixButton->setEnabled(false);
  writeNodesButton = new QPushButton(tr("Export node list"));
  writeNodesButton->setEnabled(false);
  writeEdgesButton = new QPushButton(tr("Export edge list"));
  writeEdgesButton->setEnabled(false);

  connect(openFile, SIGNAL(clicked()), this, SLOT(getFile()));
  connect(dataInterface, SIGNAL(importFinished()), this, SLOT(enableOptions()));
  connect(sepSelector, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setSep(const QString &)));
  connect(importFile, SIGNAL(clicked()), this, SLOT(readNewData()));
  connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
  connect(writeMatrixButton, SIGNAL(clicked()), this, SLOT(writeMatrix()));
  connect(writeNodesButton, SIGNAL(clicked()), this, SLOT(writeNodes()));
  connect(writeEdgesButton, SIGNAL(clicked()), this, SLOT(writeEdges()));

  QPointer<QVBoxLayout> topLayout = new QVBoxLayout;
  topLayout->addWidget(importLabel);
  QPointer<QHBoxLayout> topLayoutOne = new QHBoxLayout;
  topLayoutOne->addWidget(openFile);
  topLayoutOne->addWidget(sepSelector);
  topLayout->addLayout(topLayoutOne);
  topLayout->addWidget(importFile);

  QPointer<QVBoxLayout> middleLayout = new QVBoxLayout;
  middleLayout->addWidget(writeMatrixButton);
  middleLayout->addWidget(writeNodesButton);
  middleLayout->addWidget(writeEdgesButton);

  QPointer<QVBoxLayout> lowerLayout = new QVBoxLayout;
  lowerLayout->addWidget(exitButton);
  
  QPointer<QFrame> horizontalSepOne = new QFrame();
  horizontalSepOne->setFrameShape(QFrame::HLine);
  QPointer<QFrame> horizontalSepTwo = new QFrame();
  horizontalSepTwo->setFrameShape(QFrame::HLine);
 
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout; 
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(horizontalSepOne);
  mainLayout->addLayout(middleLayout);
  mainLayout->addWidget(horizontalSepTwo);
  mainLayout->addLayout(lowerLayout);
  setLayout(mainLayout);
  setWindowTitle(tr("Matrix converter"));
  resize(300,100);
}

// Private slots are listed below.

// I am reusing this a couple of times, so it makes sense to write a function for it.
void MainDialog::initializeSeps() {
  sepSelector->clear();
  sepSelector->addItem(SEPDEFAULT);
  sepSelector->addItem(",");
  sepSelector->addItem(";");
  sepSelector->addItem(":");
  sepSelector->addItem("|");
}
  
// This gets the filename, using a dialog that requires the user to select a file.
void MainDialog::getFile() {
  QString file = QFileDialog::getOpenFileName(this, tr("Open File"),"","Comma Delimited Files(*.csv)");
  if(!file.trimmed().isEmpty()) {
    fileName = file;
    initializeSeps();
    sepSelector->setEnabled(true);
    writeMatrixButton->setEnabled(false);
    writeNodesButton->setEnabled(false);
    writeEdgesButton->setEnabled(false);
    importFile->setEnabled(false);
  } else {
    sepSelector->setEnabled(false);
    writeMatrixButton->setEnabled(false);
    writeNodesButton->setEnabled(false);
    writeEdgesButton->setEnabled(false);
    importFile->setEnabled(false);
  }
}

void MainDialog::setSep(const QString &selection) {
  sep = selection;
  if (sep != "-Select a delimiter-") {
    importFile->setEnabled(true);
  } else {
    importFile->setEnabled(false);
  }
}

void MainDialog::readNewData() {
  dataInterface->readFile(fileName, sep);
}

void MainDialog::enableOptions() {
  writeMatrixButton->setEnabled(true);
  writeNodesButton->setEnabled(true);
  writeEdgesButton->setEnabled(true);
}

void MainDialog::writeMatrix() {
  QString QsaveFile = QFileDialog::getSaveFileName(this, tr("Export File"),"", tr("csv files (*.csv)"));
  if (!QsaveFile.trimmed().isEmpty()) {
    if (!QsaveFile.endsWith(".csv")) {
      QsaveFile.append(".csv");
    }
    dataInterface->writeMatrix(QsaveFile, sep);
  }
}

void MainDialog::writeNodes() {
  QString QsaveFile = QFileDialog::getSaveFileName(this, tr("Export File"),"", tr("csv files (*.csv)"));
  if (!QsaveFile.trimmed().isEmpty()) {
    if (!QsaveFile.endsWith(".csv")) {
      QsaveFile.append(".csv");
    }
    dataInterface->writeNodes(QsaveFile, sep);
  }
}

void MainDialog::writeEdges() {
  QString QsaveFile = QFileDialog::getSaveFileName(this, tr("Export File"),"", tr("csv files (*.csv)"));
  if (!QsaveFile.trimmed().isEmpty()) {
    if (!QsaveFile.endsWith(".csv")) {
      QsaveFile.append(".csv");
    }
    dataInterface->writeEdges(QsaveFile, sep);
  }
}


void MainDialog::finalBusiness() {
  delete dataInterface;
}





