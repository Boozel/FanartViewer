#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QList>

#include <QTimer>
#include <QElapsedTimer>
#include <QPropertyAnimation>

#include <QMutex>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupMasterQueue();
    void DetermineLengthToDisplay(QMovie*);

public slots:
    void Update(void);
    void ResetOldImageLabel(void);
    void FindEndOfMovie(int);

private:
    Ui::MainWindow                          *_ui;
    QList<QPair<QString, QList<QString>>>   _artDirectory;
    QMovie                                  *_currentMovie;
    QPixmap                                 _previousFirstFrame;
    QString                                 _prevMovieFilepath;
    bool                                    _bJustLaunched;
    bool                                    _bFirstPlay;
    bool                                    _bReleaseGif;
    bool                                    _bIsGif;
    int                                     _queuepos;
    QList<QPair<QString, QString>>          _masterQueue;
    QElapsedTimer                           _elapsedTimer;
    QPropertyAnimation                      *_slideOut;
    QMutex                                  _gifMutex;
};
#endif // MAINWINDOW_H
