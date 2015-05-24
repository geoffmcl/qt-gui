/* ==================================================
   Qt GUI project
   Created: Geoff R. McLane - Mar 2012
   License: GNU GPL3 (or later)
   ================================================== */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QFrame>
#include <QPlainTextEdit>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStringList>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

    QSettings *settings;    // = new QSettings(tmp,QSettings::IniFormat,this);

    QWidget *widget;    // = new QWidget(this);
    QVBoxLayout *mainLayout;    // = new QVBoxLayout(this);
    QMenuBar *menuBar;  // = new QMenuBar(this);
    QMenu *menuFile;    // = new QMenu(tr("&File"),this);
    QAction *dialogAct;  // = menuFile->addAction(tr("&Dialog"),this,SLOT(on_dialog()));

    QAction *exitAct;   // = menuFile->addAction(tr("&Quit",this,SLOT(on_exit())));
    QMenu *menuHelp;    // = new QMenu(tr("&Help"),this);
    QAction *helpAct;   // = menuHelp->addAction(tr("&About"),this,SLOT(on_about()));
    QAction *abtqtAct;  // = menuHelp->addAction(tr("&About Qt"),this,SLOT(on_about_qt()));

    QStatusBar *statusBar;  // = new QStatusBar(this);
    QLabel *labelTime;  // = new QLabel(tr("00:00:00"),this);
    QPlainTextEdit *sourceEdit;
    QPlainTextEdit *resultsEdit;    // = new QPlainTextEdit(this);
    QTimer *timer;      // = new QTimer(this);


public slots:
    void on_dialog();
    void on_exit();
    void on_time_out();
    void on_about();
    void on_about_qt();


};

#endif // MAINWINDOW_H
