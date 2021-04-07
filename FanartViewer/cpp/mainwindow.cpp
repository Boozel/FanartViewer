#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QPixmap>
#include <qbitmap.h>
#include <QRandomGenerator>

#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Stolen from the internet to make the background transparent (http://www.ti-r.com/?Articles/Qt/QtWindowBackgroundTransparency)
    // ****************************************************
    //Opacity to 1 to make the window fully opaque
    setWindowOpacity(1);

    // Remove the frame of the window
    //setWindowFlags(windowFlags() | Qt::FramelessWindowHint );

    // Instruct Qt to use a translucent background
    setAttribute(Qt::WA_TranslucentBackground, true);
    // End of dirty theft from a lovely person thanks dawg
    // ****************************************************
    
    // create and start update timer
    mUpdateTimer = new QTimer(this);
    connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(Update()));

    mUpdateTimer->setInterval(10000); // Tick every ten seconds
    mUpdateTimer->start();

    // Retrieve all art files
    //   'dir' starts in CWD of .exe
    QDir dir;
    //   CD to adjacent dir 'pictures'
    dir.cd(dir.absolutePath() + "/pictures");
    //   store TLD
    QString tlwd = dir.absolutePath();
    //   Get only files and directories
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);

    //   Create a list of all of the top-level directory names. Theoretically, all the artists.
    QStringList artists = dir.entryList (QDir::NoFilter, QDir::DirsFirst);

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
        dir.cd(tlwd);

        //   Append the sum of this artist's efforts into the pile
        mArtDirectory.append(mArtistPieces);
    }

    //   run our Update method - throw up some art
    Update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update()
{
    // Randomize and artist and a picture
    // TODO: Check that the selected folder isn't empty (SEGFAULT!)
    quint32 artist = QRandomGenerator::global()->bounded(0, mArtDirectory.size());
    quint32 picture = QRandomGenerator::global()->bounded(0, mArtDirectory[artist].second.size());
    // Load picture into QPixMap
    QPixmap currentPic(mArtDirectory[artist].second[picture]);

    // If the picture is NOT a picture or is broken... Get a different one
    // TODO: This will run forever if you, for some reason, have a folder of trash
    while (currentPic.isNull())
    {
        qDebug() << "This image is broken: " + mArtDirectory[artist].second[picture] + " Next...";
        quint32 picture = QRandomGenerator::global()->bounded(0, mArtDirectory[artist].second.size());
        currentPic = mArtDirectory[artist].second[picture];
    }
    // Put the picture into the application's render
    ui->picDisplayLabel->setPixmap(currentPic.scaled(ui->picDisplayLabel->width(), ui->picDisplayLabel->height(), Qt::KeepAspectRatio));
    ui->picDisplayLabel->setMask(currentPic.mask());
    ui->picDisplayLabel->show();

    // put up name.
    // TODO: write an overridden label object so we can do our own draws.
    //       THIS IS THE ONLY WAY WE COULD OUTLINE TEXT.
    ui->artistNameDisplayLabel->setText(mArtDirectory[artist].first);
}
