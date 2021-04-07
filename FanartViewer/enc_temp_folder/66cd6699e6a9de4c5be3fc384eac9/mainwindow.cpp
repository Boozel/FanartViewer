#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create and start update timer
    mUpdateTimer = new QTimer(this);
    connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(Update()));

    mUpdateTimer->setInterval(1000); // Tick every second
    mUpdateTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update()
{
    qDebug("Test");
}

