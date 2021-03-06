#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QList>

#include <QElapsedTimer>
#include <QSettings>
#include <QPropertyAnimation>
#include <QColor>
#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <qshortcut.h>
#include <QImage>


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
    bool RunForcedSetupDlg();
    bool SetMatteBkgColorDlg();
    void GetDimensionsDialog(void);
    void GetTimeToDisplayDialog(void);
    void GetWipeDirDialog(void);
    void GetAuthorModeDlg(void);
    void GetFontDlg(void);
    void GetFontFontDlg(void);
    void GetFontColorDlg(void);
    void GetFontStrokeColorDlg(void);
    void GetAttribBkgColorDlg(void);
    void GetAboutDlg(void);
    void SetMenuBar(void);
    void Update(void);
    void ResetOldImageLabel(void);
    void FindEndOfMovie();
    void toggleMenuBar(void);

private:
    bool        RunSetup(bool fullinit);
    QString     SetPictureTLD();
    bool        SetAppDimesions(int, int);
    void        SetMatteBkgColor(QColor);
    void        SetTimeToDisplay(int);
    void        SetWipeDir(int);
    void        SetAuthorMode(bool);
    void        GetAttributionValues(void);
    void        SetAttributionValues(void);
    
    bool        InitViewer();
    bool        SetAnimation();
    bool        SetGeometryLabels();
    QImage      setAuthorText(QString);
    
    void resizeEvent(QResizeEvent*);
    void showEvent(QShowEvent*);
    
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
    QTimer                                  *_updateTimer;
    QPropertyAnimation                      *_slideOut;
    QMutex                                  _gifMutex;
    
    // Fancy labels
    QLabel                                  *_picDisplayLabel;
    QLabel                                  *_picDisplayLabelPrevious;
    
    // App settings
    QSettings                               *_settings;
    QString                                 _tld;
    int                                     _wipedir;
    int                                     _appW;
    int                                     _appH;
    int                                     _authorModeOverlayEnabled;
    QFont                                   _font;
    QColor                                  _fontColor;
    QColor                                  _fontStrokeColor;
    QColor                                  _attribBkgColor;
    QColor                                  _matteBkg;
    int                                     _timeToDisplay;
    QShortcut                               *_menuBarSC;
    
    // Menu Navs
    QMenu                                   *_settingsMenu;
    QAction                                 *_toggleMenuBarVis;
    QAction                                 *_runSetupDlg;
    QAction                                 *_setMatteColorDlg;
    QAction                                 *_setWindowSize;
    QAction                                 *_setTimeToDisplay;
    QAction                                 *_setWipeDir;
    QAction                                 *_setAuthorMode;
    QAction                                 *_setFont;

    // About nav
    QMenu                                   * _helpMenu;
    QAction                                 *_about;
};
#endif // MAINWINDOW_H
