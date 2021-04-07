#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void Update(void);

private:
    QTimer                                  *mUpdateTimer;
    Ui::MainWindow                          *ui;
    QList<QPair<QString, QList<QString>>>   mArtDirectory;
    

};
#endif // MAINWINDOW_H
