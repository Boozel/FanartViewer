#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QPixmap>
#include <qbitmap.h>
#include <QRandomGenerator>
#include <qmovie.h>

#include <qpainter.h>

#include <qdebug.h>

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _settings = new QSettings("Boozel", "Fanart Viewer");
    RunSetup(true);
    
    
    // Stolen from the internet to make the background transparent (http://www.ti-r.com/?Articles/Qt/QtWindowBackgroundTransparency)
    // ****************************************************
    //Opacity to 1 to make the window fully opaque
    //setWindowOpacity(1);

    // Remove the frame of the window
    //setWindowFlags(windowFlags() | Qt::FramelessWindowHint );

    // Instruct Qt to use a translucent background
    //setAttribute(Qt::WA_TranslucentBackground, true);
    // End of dirty theft from a lovely person thanks dawg
    // ****************************************************

    // Initialize some stuff
    _bJustLaunched = true;
    _bFirstPlay = true;
    _bReleaseGif = false;
    _currentMovie = new QMovie();

    _slideOut = new QPropertyAnimation(_ui->picDisplayLabelPrevious, "geometry");
    _slideOut->setDuration(2000);
    _slideOut->setEasingCurve(QEasingCurve::InQuad);
    _slideOut->setStartValue(QRect(_ui->picDisplayLabelPrevious->geometry().x(), _ui->picDisplayLabelPrevious->geometry().y(), _ui->picDisplayLabelPrevious->geometry().width(), _ui->picDisplayLabelPrevious->geometry().height()));
    _slideOut->setEndValue(QRect(-1920, _ui->picDisplayLabelPrevious->geometry().y(), _ui->picDisplayLabelPrevious->geometry().width(), _ui->picDisplayLabelPrevious->geometry().height()));
    connect(_slideOut, SIGNAL(finished()), this, SLOT(ResetOldImageLabel()));

    // Retrieve all art files
    //   'dir' starts in CWD of .exe
    QDir dir;
    //   CD to adjacent dir 'pictures'
    dir.cd(_tld);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);
    //ui->picDisplayLabel->setScaledContents(true);

    //   Create a list of all of the top-level directory names. Theoretically, all the artists.
    QStringList artists = dir.entryList (QDir::NoFilter, QDir::DirsFirst);
    if (artists.size() == 0)
    {
        // Nothing to show - close program.
        close();
    }

    qDebug() << "Files in " << dir.absolutePath() << ":";

    //   For each artist directory, scan it for all files (hopefully, all images...)
    foreach(const QString &artist, artists)
    {
        //   Create a pair object that will hold the artist's name ('first') and a list of all found pieces ('second', list of absolute paths to art)
        QPair<QString, QList<QString>> mArtistPieces;
        mArtistPieces.first = artist;
        //   Chdir into the artist's folder
        dir.cd(dir.absolutePath() + "/" + mArtistPieces.first);
        QStringList pieces = dir.entryList(QDir::NoFilter, QDir::DirsFirst);
        qDebug() << QString("Artist: %1").arg(artist);

        //   Iterate over all found art
        foreach(const QString &picture, pieces)
        {
            //   Store each piece
            qDebug() << QString("Picture: %1").arg(picture);
            mArtistPieces.second.append(dir.absolutePath() + "/" + picture);
        }

        //   return to /pictures dir
        dir.cd(_tld);

        //   Append the sum of this artist's efforts into the pile
        _artDirectory.append(mArtistPieces);
    }

    // Hide the "previous" image label - there is currently no previous image.
    _ui->picDisplayLabelPrevious->setVisible(false);
    _ui->picDisplayLabelPrevious->setAttribute(Qt::WA_TranslucentBackground);

    // Randomize and queue all images
    setupMasterQueue();
    _queuepos = 0;

    //   run our Update method - throw up some art
    Update();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::SetMenuBar()
{
    _settingsMenu = menuBar()->addMenu(tr("&Settings"));
    // Open action
    _runSetup = new QAction(tr("&Run Setup"), this);
    _runSetup->setShortcuts(QKeySequence::Open);
    _runSetup->setStatusTip(tr("Run the initial setup process again."));
    connect(_runSetup, SIGNAL(triggered()), this, SLOT(RunForcedSetup()));
    _settingsMenu->addAction(_runSetup);
}


bool MainWindow::RunForcedSetup()
{
    _settings->setValue("needs_init", 0);
    RunSetup(false);
    
    return 0;
}

bool MainWindow::RunSetup(bool fullinit)
{
    if(_settings->value("needs_init").toInt() == 0)   //Uninitialized
    {
        _tld = "";
        while(_tld == "")
        {
            _tld = SetPictureTLD();
            
            if(_tld == NULL)
            {
                int ret = QMessageBox::warning(this, tr("Fanart Viewer"),
                                               tr("Fanart Viewer requires you to point it to the folder that contains all of your fanart.\nPlease select your fanart folder."),
                                               QMessageBox::Cancel | QMessageBox::Ok);
                
                if(ret == QMessageBox::Cancel)
                {
                    QMessageBox::warning(this, tr("Fanart Viewer"),
                                                   tr("Fanart Viewer will now close."),
                                                   QMessageBox::Ok);
                    exit(0);
                }
            }
            else
            {
                _settings->setValue("needs_init", 1);
                _settings->setValue("pictures_dir", _tld);
            }
        }
    }
    if(fullinit == true)
    {
        SetMenuBar();
    }
    _matteBkg = SetMatteBkgColor();
    SetAppDimesions();
    _tld = _settings->value("pictures_dir").value<QString>();
    this->setStyleSheet("background-color: " + _matteBkg.name());
    this->resize(_appW, _appH);
    return true;
}


QString MainWindow::SetPictureTLD()
{
    QString dir = QFileDialog::getExistingDirectory(this,
            tr("Open Fanart Master Folder"),
            "/",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    
    return dir;
}


bool MainWindow::SetAppDimesions()
{
    _appW = 1920;
    _appH = 1080;
    
    return true;
}

QColor MainWindow::SetMatteBkgColor()
{
    return QColor(0, 254, 0);
}

void MainWindow::setupMasterQueue()
{
    // If there's nothing to queue, close
    if (_artDirectory.size() <= 0)
    {
        close();
    }

    QList<QPair<QString, QString>> flatList;

    // Scan the entire art directory and load each image into the list
    for (int i = 0; i < _artDirectory.size(); i++)
    {
        for (int q = 0; q < _artDirectory[i].second.size(); q++)
        {
            flatList.append(QPair<QString, QString>(_artDirectory[i].first, _artDirectory[i].second[q]));
        }
    }

    // Load master queue
    while(flatList.size() > 0)
    {
        // Select random image
        quint32 image = QRandomGenerator::global()->bounded(0, flatList.size());

        // Add it to the queue to be played
        _masterQueue.append(flatList[image]);

        // 'pop' out the selected image
        flatList.removeAt(image);
    }

}


void MainWindow::FindEndOfMovie(int frame)
{
    // Since this is called from a signal that rapid fires, let's mutex it to be safe.
    if (_gifMutex.tryLock())
    {
        // Make sure data is valid
        if (_bIsGif && _currentMovie != nullptr)
        {
            // GIFs start at frame 1 and end at 0 for some reason? Weird.
            if (_currentMovie->currentFrameNumber() <= 1 && _bFirstPlay == true)
            {
                _bFirstPlay = false;

                // This starts a timer that counts realtime (not ticks). Calls to its 'elapsed' time returns the msecs since this call here.
                _elapsedTimer.start();
            }
            // If our frame count (frameCount is a counted value, so starts at 1) matches the current frame (The GIF has completed a play)
            if ((_currentMovie->frameCount() - 1) == _currentMovie->currentFrameNumber())
            {
                //Keep playing until the old image slides away; Make minimum display time 4 seconds
                if (_bReleaseGif && _elapsedTimer.elapsed() > 4000)
                {
                    _bFirstPlay = true;
                    // Unlock and prime a signal to call the Update() method. Not called directly to avoid thread lock.
                    QTimer::singleShot(100, this, SLOT(Update()));
                    _gifMutex.unlock();

                    // Return out to avoid a double free on the lock
                    return;
                }
            }
        }
        // Unlock if we get here.
        _gifMutex.unlock();
    }
    return;
}

void MainWindow::DetermineLengthToDisplay(QMovie* inMovie)
{
    int totalFrames = inMovie->frameCount();

    // Only calculate if it's not a still image (can be 1 or 0)
    if (totalFrames >= 2)
    {
        _bReleaseGif = false;
        _bIsGif = true;
    }

    // If this is just a still image, set a 5 second timer and move on
    else
    {
        _bIsGif = false;
        QTimer::singleShot(5000, this, SLOT(Update()));
    }
}

void MainWindow::Update()
{
    // Close if our queue is somehow empty
    if (_masterQueue.size() <= 0)
    {
        close();
    }

    if (!_bJustLaunched)
    {
        // Set the previous image label to the first frame of the previous image
        _ui->picDisplayLabelPrevious->setPixmap(_previousFirstFrame.scaled(_ui->picDisplayLabelPrevious->width(), _ui->picDisplayLabelPrevious->height(), Qt::KeepAspectRatio));

        // Clear memory for the next movie that will be loaded.
        if (_currentMovie != nullptr)
        {
            delete _currentMovie;
        }
    }

    // Store the next image
    _currentMovie = new QMovie(_masterQueue[_queuepos].second);

    // Generate a scaled framework for the new movie
    QPixmap scaletest(_masterQueue[_queuepos].second);

    //Validate gif - if image is broken, get a different one
    while (!_currentMovie->isValid())
    {
        qDebug() << "This image is broken: " + _masterQueue[_queuepos].second + " Next...";
        delete _currentMovie;

        _queuepos += 1;
        if (_queuepos >= _masterQueue.size())
        {
            _queuepos = 0;
        }
        _currentMovie = new QMovie(_masterQueue[_queuepos].second);
        scaletest = _masterQueue[_queuepos].second;
    }

    // Create the useable scaling data
    QSize scale = scaletest.scaled(_ui->picDisplayLabel->width(), _ui->picDisplayLabel->height(), Qt::KeepAspectRatio).size();

    // Put the picture into the application's render
    _currentMovie->setScaledSize(scale);
    _ui->picDisplayLabel->setMovie(_currentMovie);

    // Connect a signal that fires each frame - since .GIFs are fucky, we can't estimate their length.
    //      .GIFs also don't signal an end frame to QMovie, so... we have to just wait until we find the final frame.
    connect(_currentMovie, SIGNAL(frameChanged(int)), this, SLOT(FindEndOfMovie(int)));

    // Start playing it.
    _currentMovie->start();

    // Prepare the first frame for the slide-out animation when it finishes
    _previousFirstFrame = _currentMovie->currentPixmap();

    // Sets flags effectively letting us know if it is a .GIF or a still image.
    //      If it's a still image, it will set up a 4 second timer that will run update after it completes.
    //      If it's a .GIF, then we don't do much because our FindEndOfMovie() will be triggering.
    DetermineLengthToDisplay(_currentMovie);

    // put up name.
    // TODO: write an overridden label object so we can do our own draws.
    //       THIS IS THE ONLY WAY WE COULD OUTLINE TEXT.
    _ui->artistNameDisplayLabel->setText(_masterQueue[_queuepos].first);

    // Make the label that contains the previous image visible
    if (!_bJustLaunched)
    {
        _ui->picDisplayLabelPrevious->setVisible(true);
    }

    // Turn off this flag - only true the first time Update() runs since Launch.
    _bJustLaunched = false;

    // move over in the queue of images
    _queuepos += 1;
    if (_queuepos >= _masterQueue.size())
    {
        _queuepos = 0;
    }

    // Start the animation that slides the old image off the screen
    _slideOut->start();
}

void MainWindow::ResetOldImageLabel()
{
    // Reset the slide out animation
    _slideOut->stop();
    // This flag is used to prevent a very short .GIF from ending before the slide out animation completes.
    _bReleaseGif = true;
    // Hide the old image
    _ui->picDisplayLabelPrevious->setVisible(false);
    // Reset the image container to its default position (but invisible now)
    _ui->picDisplayLabelPrevious->setGeometry(QRect(0, 0, _ui->picDisplayLabelPrevious->geometry().width(), _ui->picDisplayLabelPrevious->geometry().height()));

}
