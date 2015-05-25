/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QXmlSimpleReader>
#include <QIODevice>
#include "tabdialog.h"

//! [0]
TabDialog::TabDialog(const QString &fileName, QWidget *parent)
    : QDialog(parent)
{
    QFileInfo fileInfo(fileName);

    tabWidget = new QTabWidget;
    tabWidget->addTab(new GeneralTab(fileInfo), tr("General"));
    tabWidget->addTab(new PermissionsTab(fileInfo), tr("Permissions"));
    tabWidget->addTab(new ApplicationsTab(fileInfo), tr("Applications"));
    tabWidget->addTab(new TidyTab(fileName.toStdString().c_str()), tr("Tidy"));
//! [0]

//! [1] //! [2]
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
//! [1] //! [3]
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
//! [2] //! [3]

//! [4]
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
//! [4]

//! [5]
    setWindowTitle(tr("Tab Dialog"));
}
//! [5]

//! [6]
GeneralTab::GeneralTab(const QFileInfo &fileInfo, QWidget *parent)
    : QWidget(parent)
{
    QLabel *fileNameLabel = new QLabel(tr("File Name:"));
    QLineEdit *fileNameEdit = new QLineEdit(fileInfo.fileName());

    QLabel *pathLabel = new QLabel(tr("Path:"));
    QLabel *pathValueLabel = new QLabel(fileInfo.absoluteFilePath());
    pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QLabel *sizeLabel = new QLabel(tr("Size:"));
    qlonglong size = fileInfo.size()/1024;
    QLabel *sizeValueLabel = new QLabel(tr("%1 K").arg(size));
    sizeValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QLabel *lastReadLabel = new QLabel(tr("Last Read:"));
    QLabel *lastReadValueLabel = new QLabel(fileInfo.lastRead().toString());
    lastReadValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QLabel *lastModLabel = new QLabel(tr("Last Modified:"));
    QLabel *lastModValueLabel = new QLabel(fileInfo.lastModified().toString());
    lastModValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fileNameLabel);
    mainLayout->addWidget(fileNameEdit);
    mainLayout->addWidget(pathLabel);
    mainLayout->addWidget(pathValueLabel);
    mainLayout->addWidget(sizeLabel);
    mainLayout->addWidget(sizeValueLabel);
    mainLayout->addWidget(lastReadLabel);
    mainLayout->addWidget(lastReadValueLabel);
    mainLayout->addWidget(lastModLabel);
    mainLayout->addWidget(lastModValueLabel);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
//! [6]

//! [7]
PermissionsTab::PermissionsTab(const QFileInfo &fileInfo, QWidget *parent)
    : QWidget(parent)
{
    QGroupBox *permissionsGroup = new QGroupBox(tr("Permissions"));

    QCheckBox *readable = new QCheckBox(tr("Readable"));
    if (fileInfo.isReadable())
        readable->setChecked(true);

    QCheckBox *writable = new QCheckBox(tr("Writable"));
    if ( fileInfo.isWritable() )
        writable->setChecked(true);

    QCheckBox *executable = new QCheckBox(tr("Executable"));
    if ( fileInfo.isExecutable() )
        executable->setChecked(true);

    QGroupBox *ownerGroup = new QGroupBox(tr("Ownership"));

    QLabel *ownerLabel = new QLabel(tr("Owner"));
    QLabel *ownerValueLabel = new QLabel(fileInfo.owner());
    ownerValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QLabel *groupLabel = new QLabel(tr("Group"));
    QLabel *groupValueLabel = new QLabel(fileInfo.group());
    groupValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QVBoxLayout *permissionsLayout = new QVBoxLayout;
    permissionsLayout->addWidget(readable);
    permissionsLayout->addWidget(writable);
    permissionsLayout->addWidget(executable);
    permissionsGroup->setLayout(permissionsLayout);

    QVBoxLayout *ownerLayout = new QVBoxLayout;
    ownerLayout->addWidget(ownerLabel);
    ownerLayout->addWidget(ownerValueLabel);
    ownerLayout->addWidget(groupLabel);
    ownerLayout->addWidget(groupValueLabel);
    ownerGroup->setLayout(ownerLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(permissionsGroup);
    mainLayout->addWidget(ownerGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
//! [7]

//! [8]
ApplicationsTab::ApplicationsTab(const QFileInfo &fileInfo, QWidget *parent)
    : QWidget(parent)
{
    QLabel *topLabel = new QLabel(tr("Open with:"));

    QListWidget *applicationsListBox = new QListWidget;
    QStringList applications;

    for (int i = 1; i <= 30; ++i)
        applications.append(tr("Application %1").arg(i));
    applicationsListBox->insertItems(0, applications);

    QCheckBox *alwaysCheckBox;

    if (fileInfo.suffix().isEmpty())
        alwaysCheckBox = new QCheckBox(tr("Always use this application to "
            "open this type of file"));
    else
        alwaysCheckBox = new QCheckBox(tr("Always use this application to "
            "open files with the extension '%1'").arg(fileInfo.suffix()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(applicationsListBox);
    layout->addWidget(alwaysCheckBox);
    setLayout(layout);
}
//! [8]

//! [6]
TidyTab::TidyTab(const char *xmlFile, QWidget *parent)
    : QWidget(parent)
{
    bool failed = true;
    bool isMisc = false;
    bool inOption = false;
    bool isName = false;
    bool isType = false;
    //QXmlSimpleReader xmlReader;
    //QIODevice dev;
    QString name, error, optname, opttype;
    QFile file(xmlFile);
    if (QFile::exists(xmlFile)) {
        if (file.open(QIODevice::ReadOnly)) {

            //QXmlInputSource *source = new QXmlInputSource(&file);    //QLabel *fileNameLabel = new QLabel(tr("File Name:"));
            QXmlStreamReader xml(&file);
            while (!xml.atEnd() && !xml.hasError()) {
                xml.readNext();
                if (xml.error()) {
                    printf("XML error: %s\n", xml.errorString().toStdString().c_str());
                    break;
                }
                if (xml.isStartElement()) {
                    name = xml.name().toString();
                    //QXmlStreamAttributes attributes = xml.attributes();
                    printf("Name: %s", name.toStdString().c_str());
                    if (name == "option") {
                        inOption = true;
                    } else if (inOption && (name == "name")) {
                        isName = true;
                    } else if (inOption && (name == "type")) {
                        isType = true;
                    }

                    foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                        name = attr.name().toString();
                        QString value = attr.value().toString();
                        printf(" %s=%s", name.toStdString().c_str(), value.toStdString().c_str());
                        if ((name == "class") && (value == "misc")) {
                            isMisc = true;
                        }
                    }
                    printf("\n");
                } else if (xml.isCharacters()) {
                    if (!xml.isWhitespace()) {
                        QString text = xml.text().toString();
                        if (inOption && isMisc) {
                            if (isName) {
                                printf("OptMisc: name: %s\n", text.toStdString().c_str());
                                optname = text; // got option name
                            } else if (isType) {
                                printf("OptMisc: type: %s\n", text.toStdString().c_str());
                                opttype = text; // got option type
                            } else {
                                printf("miscText: %s\n", text.toStdString().c_str());
                            }
                        } else {
                            printf("text: %s\n", text.toStdString().c_str());
                        }
                    }
                } else if ( xml.isEndElement() ) {
                    name = xml.name().toString();
                    printf("End: %s\n", name.toStdString().c_str());
                    if (name == "option") {
                        inOption = false;
                        isMisc = false;
                    } else if (name == "name") {
                        isName = false;
                    } else if (name == "type") {
                        isType = false;
                    }
                }
            }
            error = "xml decode not complete";
        } else {
            error = "Unable to open file";
        }
    } else {
        error = "File does NOT exist!";
    }
    if (failed) {
        QLabel *pathLabel = new QLabel(tr("File:"));
        QLabel *pathValueLabel = new QLabel(xmlFile);
        pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        QLabel *errorLabel = new QLabel(tr("Error:"));
        QLabel *errorValue = new QLabel(error);
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(pathLabel);
        mainLayout->addWidget(pathValueLabel);
        mainLayout->addWidget(errorLabel);
        mainLayout->addWidget(errorValue);
        mainLayout->addStretch(1);
        setLayout(mainLayout);
    }
    //QLineEdit *fileNameEdit = new QLineEdit(fileInfo.fileName());

    //QLabel *pathLabel = new QLabel(tr("Path:"));
    //QLabel *pathValueLabel = new QLabel(fileInfo.absoluteFilePath());
    //pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    //QLabel *sizeLabel = new QLabel(tr("Size:"));
    //qlonglong size = fileInfo.size()/1024;
    //QLabel *sizeValueLabel = new QLabel(tr("%1 K").arg(size));
    //sizeValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(fileNameLabel);
    //mainLayout->addWidget(fileNameEdit);
    //mainLayout->addWidget(pathLabel);
    //mainLayout->addWidget(pathValueLabel);
    //mainLayout->addWidget(sizeLabel);
    //mainLayout->addStretch(1);
    //setLayout(mainLayout);
}
//! [6]

