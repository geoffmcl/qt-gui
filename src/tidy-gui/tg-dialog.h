/*\
 * tg-main.cpp
 *
 * Copyright (c) 2015 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>
#include <QSettings>

typedef struct tabINFOSTR {
    int count;
}INFOSTR, *PINFOSTR;

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

class TabDialog : public QDialog
{
    Q_OBJECT

public:
    TabDialog(const QString &fileName, QWidget *parent = 0);
    QSettings *settings;    // = new QSettings(tmp,QSettings::IniFormat,this);
    void closeEvent(QCloseEvent *event);

public slots:
    void onQuit();


private:
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
};


////////////////////////////////////////////////
// TABS
class GeneralTab : public QWidget
{
    Q_OBJECT

public:
    GeneralTab(const QFileInfo &fileInfo, QWidget *parent = 0);
};



///////////////////////////////////////////////////////

class DiagnosticsTab : public QWidget
{
    Q_OBJECT
public:
    DiagnosticsTab( PINFOSTR pinf, QWidget *parent = 0);
};

class EncodingTab : public QWidget
{
    Q_OBJECT
public:
    EncodingTab( PINFOSTR pinf, QWidget *parent = 0);
};

class MarkupTab : public QWidget
{
    Q_OBJECT
public:
    MarkupTab( PINFOSTR pinf, QWidget *parent = 0);
};

class MiscTab : public QWidget
{
    Q_OBJECT
public:
    MiscTab( PINFOSTR pinf, QWidget *parent = 0);
};

class PrintTab : public QWidget
{
    Q_OBJECT
public:
    PrintTab( PINFOSTR pinf, QWidget *parent = 0);
};

////////////////////////////////////////////////////////


#endif
