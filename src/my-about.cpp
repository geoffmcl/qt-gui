/*\
 * my-about.cpp
 *
 * Copyright (c) 2015 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <stdio.h>
#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextBrowser>

// other includes
#include "my-about.h"

static const char *module = "my-about";

static const char *about2 =
    "<h1>This is TidyGUI2 - Version 2.0.0</h1>\n\n"
    "<p>Original based on Dave Raggett's HTML Tidy<br>\n"
    "Now uses HTACG Tidy, version 5, ver May 2015</p>\n\n"
    "<p>HTML Tidy Home Page:<br><a href=\"http://www.html-tidy.org/\">http://www.html-tidy.org/</a></p> \n\n"
    "<p>A large part of this software (HTML Tidy library) is\n"
    "Copyright © 1998-2015 World Wide Web Consortium</p>\n\n"
    "<p>HTML Tidy Original Home Page:<br>\n"
    "<a href=\"http://www.w3.org/People/Raggett/tidy\">http://www.w3.org/People/Raggett/tidy</a><br> \n\n";


My_About::My_About(QWidget *parent) :
        QDialog(parent)
{
    QPushButton *okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()),this, SLOT(close()));

    QTextEdit *bigEditor = new QTextEdit;
    //QTextBrowser *bigEditor = new QTextBrowser;
    bigEditor->setHtml(about2);
    //bigEditor->setOpenExternalLink();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->addWidget(bigEditor);
    mainLayout->addWidget(okButton);
    setLayout(mainLayout);
    setWindowTitle(tr("About Qt GUI"));
    //setWindowIcon(QIcon(":/icon/tidyicon"));

}


// eof = my-about.cpp
