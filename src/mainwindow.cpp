/* ==================================================
   Qt GUI project
   Created: Geoff R. McLane - Mar 2012
   License: GNU GPL3 (or later)
   ================================================== */
#include "app_config.h"
#include "mainwindow.h"
#include "mydialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget = new QWidget(this);
    setCentralWidget(widget);
    QString tmp = QDir(
                QDesktopServices::storageLocation(
                    QDesktopServices::DataLocation)).absolutePath();
    // create path is not exist
    if(!QFile::exists(tmp)){
        QDir dir;
        dir.mkpath(tmp);
    }
    tmp.append("/");
    tmp.append(DEF_INI_FILE);
    settings = new QSettings(tmp,QSettings::IniFormat,this);

    restoreGeometry(settings->value("mainWindowGeometry").toByteArray());

    int m = 10;
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(m,m,m,m);
    mainLayout->setSpacing(m);
    widget->setLayout(mainLayout);

    // Top MENU BAR
    menuBar = new QMenuBar(this);
    mainLayout->addWidget(menuBar);

    menuFile = new QMenu(tr("&File"),this);
    dialogAct = menuFile->addAction(tr("&Dialog"),this,SLOT(on_dialog()));
    exitAct = menuFile->addAction(tr("&Quit"),this,SLOT(on_exit()));
    menuBar->addMenu(menuFile);

    menuHelp = new QMenu(tr("&Help"),this);
    helpAct = menuHelp->addAction(tr("&About"),this,SLOT(on_about()));
    abtqtAct = menuHelp->addAction(tr("&About Qt"),this,SLOT(on_about_qt()));
    menuBar->addMenu(menuHelp);

    // Body of Window
    // source edit
    sourceEdit = new QPlainTextEdit(this);
    sourceEdit->setReadOnly(true);
    mainLayout->addWidget(sourceEdit);
    // results edit
    resultsEdit = new QPlainTextEdit(this);
    resultsEdit->setReadOnly(true);
    mainLayout->addWidget(resultsEdit);

    // Bottom STATUS BAR
    statusBar = new QStatusBar(this);
    labelTime = new QLabel(tr("00:00:00"),this);
    labelTime->setFrameStyle(QFrame::Panel | QFrame::Raised);
    statusBar->addPermanentWidget(labelTime);
    mainLayout->addWidget(statusBar);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(on_time_out()));


    // LAST ACTION: start timer ticking
    timer->start(1000); // each second

}

MainWindow::~MainWindow()
{
    
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    settings->setValue("mainWindowGeometry", saveGeometry());
    //outLog(util_getDateTimestg()+" - Application close",0x8001);
    event->accept();
}

void MainWindow::on_dialog()
{
    QStringList list;
    // list << "Item 1" << "Item 2" << "Item 3";
    int i, max = 20;
    for (i = 0; i < max; i++) {
        QString s = QString("Item %1").arg(i+1);
        list << s;
    }
    MyDialog *dialog = new MyDialog(list,this);
    // dialog->show(); // non-modal
    dialog->exec(); // modal
    delete dialog;
}

void MainWindow::on_exit()
{
    close();
}

void MainWindow::on_about()
{
    QString msg;
    msg = "Version: ";
    msg.append(APP_VERS);
    msg.append(", dated ");
    msg.append(APP_DATE);
    msg.append("\n");
    msg.append("Built: ");
    msg.append(__DATE__);
    msg.append(" at ");
    msg.append(__TIME__);
    msg.append("\n\n");
    msg.append("Qt example Gui shows various aspects of Qt.\n");
    msg.append("License: GNU GPL v3 or later\n\n");
    // msg.append("Source: http://geoffair.org/projects/qt_osm_map.htm");
    //msg.append("The session log file is [");
    //msg.append(util_getLogFile());
    //msg.append("]\n");
    QMessageBox::about(this, tr("About Qt OSM Map"), msg);
}

void MainWindow::on_about_qt()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_time_out()
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString dt = datetime.toString("HH:mm:ss");
    labelTime->setText(dt);
}

// eof - mainwindow.cpp

