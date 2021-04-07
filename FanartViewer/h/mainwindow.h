#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ArtistLabel : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent* event) override;
public:
    void UpdateArtistName(QString);
private:
    QString     _artistName;
};

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
    ArtistLabel                             *mArtistLabel;
    QList<QPair<QString, QList<QString>>>   mArtDirectory;
    

};
#endif // MAINWINDOW_H
