/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *picDisplayLabel;
    QLabel *artistNameDisplayLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        picDisplayLabel = new QLabel(centralwidget);
        picDisplayLabel->setObjectName(QString::fromUtf8("picDisplayLabel"));
        picDisplayLabel->setGeometry(QRect(0, 0, 800, 600));
        picDisplayLabel->setAlignment(Qt::AlignCenter);
        artistNameDisplayLabel = new QLabel(centralwidget);
        artistNameDisplayLabel->setObjectName(QString::fromUtf8("artistNameDisplayLabel"));
        artistNameDisplayLabel->setGeometry(QRect(200, 560, 400, 30));
        artistNameDisplayLabel->setAutoFillBackground(false);
        artistNameDisplayLabel->setStyleSheet(QString::fromUtf8("font: 20pt \"Impact\";color: rgb(255,255,255);background-color:rgb(0,0,0)"));
        artistNameDisplayLabel->setScaledContents(false);
        artistNameDisplayLabel->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        picDisplayLabel->setText(QString());
        artistNameDisplayLabel->setText(QCoreApplication::translate("MainWindow", "Sebben & Sebben Placeholder Text", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
