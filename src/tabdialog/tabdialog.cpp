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
#include "tabconfig.h"

static INFOSTR infostr;

//! [0]
TabDialog::TabDialog(const QString &fileName, QWidget *parent)
    : QDialog(parent)
{
    QFileInfo fileInfo(fileName);
    PINFOSTR pinfo = &infostr;

    tabWidget = new QTabWidget;
    tabWidget->addTab(new GeneralTab(fileInfo), tr("General"));
    //tabWidget->addTab(new PermissionsTab(fileInfo), tr("Permissions"));
    //tabWidget->addTab(new ApplicationsTab(fileInfo), tr("Applications"));
    //tabWidget->addTab(new TidyTab(fileName.toStdString().c_str()), tr("Tidy"));
    tabWidget->addTab(new DiagnosticsTab( pinfo ), tr("Diagnostics"));
    tabWidget->addTab(new EncodingTab( pinfo ), tr("Encoding"));
    tabWidget->addTab(new MarkupTab( pinfo ), tr("Markup"));
    tabWidget->addTab(new MiscTab( pinfo ), tr("Misc"));
    tabWidget->addTab(new PrintTab( pinfo ), tr("Print"));
    ////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////

DiagnosticsTab::DiagnosticsTab( PINFOSTR pinf, QWidget *parent)
    : QWidget(parent)
{
    int i;
    QString s;

    QCheckBox *show_info = new QCheckBox("show-info (Boolean)");
    if (getConfigBool("show-info")) {
        show_info->setChecked(true);
    }

    QCheckBox *show_warnings = new QCheckBox("show-warnings (Boolean)");
    if (getConfigBool("show-warnings")) {
        show_warnings->setChecked(true);
    }

    QLabel *show_errors = new QLabel("show-errors (Integer)");
    i = getConfigInt("show-errors");
    s = QString::number(i);
    QLineEdit *show_errorsEd = new QLineEdit(s);
    show_errorsEd->setMaximumWidth(50);

    QGroupBox *accessibility_check = new QGroupBox("accessibility-check");
    QComboBox *accessibility_checkComb = new QComboBox();
    accessibility_checkComb->addItem("0 (Tidy Classic)","0 (Tidy Classic)");
    accessibility_checkComb->addItem("1 (Priority 1 Checks)","1 (Priority 1 Checks)");
    accessibility_checkComb->addItem("2 (Priority 2 Checks)","2 (Priority 2 Checks)");
    accessibility_checkComb->addItem("3 (Priority 3 Checks)","3 (Priority 3 Checks)");
    s = getConfigEnc("accessibility-check");
    i = accessibility_checkComb->findData(s);
    if (i != -1) {
        accessibility_checkComb->setCurrentIndex(i);
    }
    QVBoxLayout *accessibility_checkLay = new QVBoxLayout;
    accessibility_checkLay->addWidget(accessibility_checkComb);
    accessibility_checkLay->addStretch(1);
    accessibility_check->setLayout(accessibility_checkLay);
    QVBoxLayout *DiagnosticsLayout = new QVBoxLayout;
    DiagnosticsLayout->addWidget(show_info);
    DiagnosticsLayout->addWidget(show_warnings);
    DiagnosticsLayout->addWidget(show_errors);
    DiagnosticsLayout->addWidget(show_errorsEd);
    DiagnosticsLayout->addWidget(accessibility_check);
    DiagnosticsLayout->addStretch(1);
    setLayout(DiagnosticsLayout);
}

EncodingTab::EncodingTab( PINFOSTR pinf, QWidget *parent)
    : QWidget(parent)
{
    int i;
    QString s;

    QGroupBox *char_encoding = new QGroupBox("char-encoding");
    QComboBox *char_encodingComb = new QComboBox();
    char_encodingComb->addItem("raw","raw");
    char_encodingComb->addItem("ascii","ascii");
    char_encodingComb->addItem("latin0","latin0");
    char_encodingComb->addItem("latin1","latin1");
    char_encodingComb->addItem("utf8","utf8");
    char_encodingComb->addItem("iso2022","iso2022");
    char_encodingComb->addItem("mac","mac");
    char_encodingComb->addItem("win1252","win1252");
    char_encodingComb->addItem("ibm858","ibm858");
    char_encodingComb->addItem("utf16le","utf16le");
    char_encodingComb->addItem("utf16be","utf16be");
    char_encodingComb->addItem("utf16","utf16");
    char_encodingComb->addItem("big5","big5");
    char_encodingComb->addItem("shiftjis","shiftjis");
    s = getConfigEnc("char-encoding");
    i = char_encodingComb->findData(s);
    if (i != -1) {
        char_encodingComb->setCurrentIndex(i);
    }
    QVBoxLayout *char_encodingLay = new QVBoxLayout;
    char_encodingLay->addWidget(char_encodingComb);
    char_encodingLay->addStretch(1);
    char_encoding->setLayout(char_encodingLay);

    QGroupBox *input_encoding = new QGroupBox("input-encoding");
    QComboBox *input_encodingComb = new QComboBox();
    input_encodingComb->addItem("raw","raw");
    input_encodingComb->addItem("ascii","ascii");
    input_encodingComb->addItem("latin0","latin0");
    input_encodingComb->addItem("latin1","latin1");
    input_encodingComb->addItem("utf8","utf8");
    input_encodingComb->addItem("iso2022","iso2022");
    input_encodingComb->addItem("mac","mac");
    input_encodingComb->addItem("win1252","win1252");
    input_encodingComb->addItem("ibm858","ibm858");
    input_encodingComb->addItem("utf16le","utf16le");
    input_encodingComb->addItem("utf16be","utf16be");
    input_encodingComb->addItem("utf16","utf16");
    input_encodingComb->addItem("big5","big5");
    input_encodingComb->addItem("shiftjis","shiftjis");
    s = getConfigEnc("input-encoding");
    i = input_encodingComb->findData(s);
    if (i != -1) {
        input_encodingComb->setCurrentIndex(i);
    }
    QVBoxLayout *input_encodingLay = new QVBoxLayout;
    input_encodingLay->addWidget(input_encodingComb);
    input_encodingLay->addStretch(1);
    input_encoding->setLayout(input_encodingLay);

    QGroupBox *output_encoding = new QGroupBox("output-encoding");
    QComboBox *output_encodingComb = new QComboBox();
    output_encodingComb->addItem("raw","raw");
    output_encodingComb->addItem("ascii","ascii");
    output_encodingComb->addItem("latin0","latin0");
    output_encodingComb->addItem("latin1","latin1");
    output_encodingComb->addItem("utf8","utf8");
    output_encodingComb->addItem("iso2022","iso2022");
    output_encodingComb->addItem("mac","mac");
    output_encodingComb->addItem("win1252","win1252");
    output_encodingComb->addItem("ibm858","ibm858");
    output_encodingComb->addItem("utf16le","utf16le");
    output_encodingComb->addItem("utf16be","utf16be");
    output_encodingComb->addItem("utf16","utf16");
    output_encodingComb->addItem("big5","big5");
    output_encodingComb->addItem("shiftjis","shiftjis");
    s = getConfigEnc("output-encoding");
    i = output_encodingComb->findData(s);
    if (i != -1) {
        output_encodingComb->setCurrentIndex(i);
    }
    QVBoxLayout *output_encodingLay = new QVBoxLayout;
    output_encodingLay->addWidget(output_encodingComb);
    output_encodingLay->addStretch(1);
    output_encoding->setLayout(output_encodingLay);

    QGroupBox *newline = new QGroupBox("newline");
    QComboBox *newlineComb = new QComboBox();
    newlineComb->addItem("LF","LF");
    newlineComb->addItem("CRLF","CRLF");
    newlineComb->addItem("CR","CR");
    s = getConfigEnc("newline");
    i = newlineComb->findData(s);
    if (i != -1) {
        newlineComb->setCurrentIndex(i);
    }
    QVBoxLayout *newlineLay = new QVBoxLayout;
    newlineLay->addWidget(newlineComb);
    newlineLay->addStretch(1);
    newline->setLayout(newlineLay);

    QGroupBox *doctype = new QGroupBox("doctype");
    QComboBox *doctypeComb = new QComboBox();
    doctypeComb->addItem("html5","html5");
    doctypeComb->addItem("omit","omit");
    doctypeComb->addItem("auto","auto");
    doctypeComb->addItem("strict","strict");
    doctypeComb->addItem("transitional","transitional");
    doctypeComb->addItem("user","user");
    s = getConfigEnc("doctype");
    i = doctypeComb->findData(s);
    if (i != -1) {
        doctypeComb->setCurrentIndex(i);
    }
    QVBoxLayout *doctypeLay = new QVBoxLayout;
    doctypeLay->addWidget(doctypeComb);
    doctypeLay->addStretch(1);
    doctype->setLayout(doctypeLay);

    QGroupBox *repeated_attributes = new QGroupBox("repeated-attributes");
    QComboBox *repeated_attributesComb = new QComboBox();
    repeated_attributesComb->addItem("keep-first","keep-first");
    repeated_attributesComb->addItem("keep-last","keep-last");
    s = getConfigEnc("repeated-attributes");
    i = repeated_attributesComb->findData(s);
    if (i != -1) {
        repeated_attributesComb->setCurrentIndex(i);
    }
    QVBoxLayout *repeated_attributesLay = new QVBoxLayout;
    repeated_attributesLay->addWidget(repeated_attributesComb);
    repeated_attributesLay->addStretch(1);
    repeated_attributes->setLayout(repeated_attributesLay);

    QCheckBox *ascii_chars = new QCheckBox("ascii-chars (Boolean)");
    if (getConfigBool("ascii-chars")) {
        ascii_chars->setChecked(true);
    }

    QLabel *language = new QLabel("language (String)");
    s = getConfigStg("language");
    QLineEdit *languageEd = new QLineEdit(s);

    QCheckBox *output_bom = new QCheckBox("output-bom (AutoBool)");
    if (getConfigBool("output-bom")) {
        output_bom->setChecked(true);
    }
    QVBoxLayout *EncodingLayout = new QVBoxLayout;
    EncodingLayout->addWidget(char_encoding);
    EncodingLayout->addWidget(input_encoding);
    EncodingLayout->addWidget(output_encoding);
    EncodingLayout->addWidget(newline);
    EncodingLayout->addWidget(doctype);
    EncodingLayout->addWidget(repeated_attributes);
    EncodingLayout->addWidget(ascii_chars);
    EncodingLayout->addWidget(language);
    EncodingLayout->addWidget(languageEd);
    EncodingLayout->addWidget(output_bom);
    EncodingLayout->addStretch(1);
    setLayout(EncodingLayout);
}

MarkupTab::MarkupTab( PINFOSTR pinf, QWidget *parent)
    : QWidget(parent)
{
    int i;
    QString s;

    QCheckBox *coerce_endtags = new QCheckBox("coerce-endtags (Boolean)");
    if (getConfigBool("coerce-endtags")) {
        coerce_endtags->setChecked(true);
    }

    QCheckBox *omit_optional_tags = new QCheckBox("omit-optional-tags (Boolean)");
    if (getConfigBool("omit-optional-tags")) {
        omit_optional_tags->setChecked(true);
    }

    QCheckBox *hide_endtags = new QCheckBox("hide-endtags (Boolean)");
    if (getConfigBool("hide-endtags")) {
        hide_endtags->setChecked(true);
    }

    QCheckBox *input_xml = new QCheckBox("input-xml (Boolean)");
    if (getConfigBool("input-xml")) {
        input_xml->setChecked(true);
    }

    QCheckBox *output_xml = new QCheckBox("output-xml (Boolean)");
    if (getConfigBool("output-xml")) {
        output_xml->setChecked(true);
    }

    QCheckBox *output_xhtml = new QCheckBox("output-xhtml (Boolean)");
    if (getConfigBool("output-xhtml")) {
        output_xhtml->setChecked(true);
    }

    QCheckBox *output_html = new QCheckBox("output-html (Boolean)");
    if (getConfigBool("output-html")) {
        output_html->setChecked(true);
    }

    QCheckBox *add_xml_decl = new QCheckBox("add-xml-decl (Boolean)");
    if (getConfigBool("add-xml-decl")) {
        add_xml_decl->setChecked(true);
    }

    QCheckBox *uppercase_tags = new QCheckBox("uppercase-tags (Boolean)");
    if (getConfigBool("uppercase-tags")) {
        uppercase_tags->setChecked(true);
    }

    QCheckBox *uppercase_attributes = new QCheckBox("uppercase-attributes (Boolean)");
    if (getConfigBool("uppercase-attributes")) {
        uppercase_attributes->setChecked(true);
    }

    QCheckBox *bare = new QCheckBox("bare (Boolean)");
    if (getConfigBool("bare")) {
        bare->setChecked(true);
    }

    QCheckBox *clean = new QCheckBox("clean (Boolean)");
    if (getConfigBool("clean")) {
        clean->setChecked(true);
    }

    QCheckBox *gdoc = new QCheckBox("gdoc (Boolean)");
    if (getConfigBool("gdoc")) {
        gdoc->setChecked(true);
    }

    QCheckBox *logical_emphasis = new QCheckBox("logical-emphasis (Boolean)");
    if (getConfigBool("logical-emphasis")) {
        logical_emphasis->setChecked(true);
    }

    QCheckBox *drop_proprietary_attributes = new QCheckBox("drop-proprietary-attributes (Boolean)");
    if (getConfigBool("drop-proprietary-attributes")) {
        drop_proprietary_attributes->setChecked(true);
    }

    QCheckBox *drop_font_tags = new QCheckBox("drop-font-tags (Boolean)");
    if (getConfigBool("drop-font-tags")) {
        drop_font_tags->setChecked(true);
    }

    QCheckBox *drop_empty_elements = new QCheckBox("drop-empty-elements (Boolean)");
    if (getConfigBool("drop-empty-elements")) {
        drop_empty_elements->setChecked(true);
    }

    QCheckBox *drop_empty_paras = new QCheckBox("drop-empty-paras (Boolean)");
    if (getConfigBool("drop-empty-paras")) {
        drop_empty_paras->setChecked(true);
    }

    QCheckBox *fix_bad_comments = new QCheckBox("fix-bad-comments (Boolean)");
    if (getConfigBool("fix-bad-comments")) {
        fix_bad_comments->setChecked(true);
    }

    QCheckBox *numeric_entities = new QCheckBox("numeric-entities (Boolean)");
    if (getConfigBool("numeric-entities")) {
        numeric_entities->setChecked(true);
    }

    QCheckBox *quote_marks = new QCheckBox("quote-marks (Boolean)");
    if (getConfigBool("quote-marks")) {
        quote_marks->setChecked(true);
    }

    QCheckBox *quote_nbsp = new QCheckBox("quote-nbsp (Boolean)");
    if (getConfigBool("quote-nbsp")) {
        quote_nbsp->setChecked(true);
    }

    QCheckBox *quote_ampersand = new QCheckBox("quote-ampersand (Boolean)");
    if (getConfigBool("quote-ampersand")) {
        quote_ampersand->setChecked(true);
    }

    QCheckBox *fix_backslash = new QCheckBox("fix-backslash (Boolean)");
    if (getConfigBool("fix-backslash")) {
        fix_backslash->setChecked(true);
    }

    QCheckBox *assume_xml_procins = new QCheckBox("assume-xml-procins (Boolean)");
    if (getConfigBool("assume-xml-procins")) {
        assume_xml_procins->setChecked(true);
    }

    QCheckBox *add_xml_space = new QCheckBox("add-xml-space (Boolean)");
    if (getConfigBool("add-xml-space")) {
        add_xml_space->setChecked(true);
    }

    QCheckBox *enclose_text = new QCheckBox("enclose-text (Boolean)");
    if (getConfigBool("enclose-text")) {
        enclose_text->setChecked(true);
    }

    QCheckBox *enclose_block_text = new QCheckBox("enclose-block-text (Boolean)");
    if (getConfigBool("enclose-block-text")) {
        enclose_block_text->setChecked(true);
    }

    QCheckBox *word_2000 = new QCheckBox("word-2000 (Boolean)");
    if (getConfigBool("word-2000")) {
        word_2000->setChecked(true);
    }

    QCheckBox *literal_attributes = new QCheckBox("literal-attributes (Boolean)");
    if (getConfigBool("literal-attributes")) {
        literal_attributes->setChecked(true);
    }

    QCheckBox *show_body_only = new QCheckBox("show-body-only (AutoBool)");
    if (getConfigBool("show-body-only")) {
        show_body_only->setChecked(true);
    }

    QCheckBox *fix_uri = new QCheckBox("fix-uri (Boolean)");
    if (getConfigBool("fix-uri")) {
        fix_uri->setChecked(true);
    }

    QCheckBox *lower_literals = new QCheckBox("lower-literals (Boolean)");
    if (getConfigBool("lower-literals")) {
        lower_literals->setChecked(true);
    }

    QCheckBox *hide_comments = new QCheckBox("hide-comments (Boolean)");
    if (getConfigBool("hide-comments")) {
        hide_comments->setChecked(true);
    }

    QCheckBox *indent_cdata = new QCheckBox("indent-cdata (Boolean)");
    if (getConfigBool("indent-cdata")) {
        indent_cdata->setChecked(true);
    }

    QCheckBox *join_classes = new QCheckBox("join-classes (Boolean)");
    if (getConfigBool("join-classes")) {
        join_classes->setChecked(true);
    }

    QCheckBox *join_styles = new QCheckBox("join-styles (Boolean)");
    if (getConfigBool("join-styles")) {
        join_styles->setChecked(true);
    }

    QCheckBox *escape_cdata = new QCheckBox("escape-cdata (Boolean)");
    if (getConfigBool("escape-cdata")) {
        escape_cdata->setChecked(true);
    }

    QCheckBox *ncr = new QCheckBox("ncr (Boolean)");
    if (getConfigBool("ncr")) {
        ncr->setChecked(true);
    }

    QCheckBox *replace_color = new QCheckBox("replace-color (Boolean)");
    if (getConfigBool("replace-color")) {
        replace_color->setChecked(true);
    }

    QLabel *css_prefix = new QLabel("css-prefix (String)");
    s = getConfigStg("css-prefix");
    QLineEdit *css_prefixEd = new QLineEdit(s);

    QLabel *new_inline_tags = new QLabel("new-inline-tags (String)");
    s = getConfigStg("new-inline-tags");
    QLineEdit *new_inline_tagsEd = new QLineEdit(s);

    QLabel *new_blocklevel_tags = new QLabel("new-blocklevel-tags (String)");
    s = getConfigStg("new-blocklevel-tags");
    QLineEdit *new_blocklevel_tagsEd = new QLineEdit(s);

    QLabel *new_empty_tags = new QLabel("new-empty-tags (String)");
    s = getConfigStg("new-empty-tags");
    QLineEdit *new_empty_tagsEd = new QLineEdit(s);

    QLabel *new_pre_tags = new QLabel("new-pre-tags (String)");
    s = getConfigStg("new-pre-tags");
    QLineEdit *new_pre_tagsEd = new QLineEdit(s);

    QCheckBox *merge_emphasis = new QCheckBox("merge-emphasis (Boolean)");
    if (getConfigBool("merge-emphasis")) {
        merge_emphasis->setChecked(true);
    }

    QCheckBox *merge_divs = new QCheckBox("merge-divs (AutoBool)");
    if (getConfigBool("merge-divs")) {
        merge_divs->setChecked(true);
    }

    QCheckBox *decorate_inferred_ul = new QCheckBox("decorate-inferred-ul (Boolean)");
    if (getConfigBool("decorate-inferred-ul")) {
        decorate_inferred_ul->setChecked(true);
    }

    QCheckBox *preserve_entities = new QCheckBox("preserve-entities (Boolean)");
    if (getConfigBool("preserve-entities")) {
        preserve_entities->setChecked(true);
    }

    QCheckBox *merge_spans = new QCheckBox("merge-spans (AutoBool)");
    if (getConfigBool("merge-spans")) {
        merge_spans->setChecked(true);
    }

    QCheckBox *anchor_as_name = new QCheckBox("anchor-as-name (Boolean)");
    if (getConfigBool("anchor-as-name")) {
        anchor_as_name->setChecked(true);
    }
    QVBoxLayout *MarkupLayout1 = new QVBoxLayout;
    MarkupLayout1->addWidget(coerce_endtags);
    MarkupLayout1->addWidget(omit_optional_tags);
    MarkupLayout1->addWidget(hide_endtags);
    MarkupLayout1->addWidget(input_xml);
    MarkupLayout1->addWidget(output_xml);
    MarkupLayout1->addWidget(output_xhtml);
    MarkupLayout1->addWidget(output_html);
    MarkupLayout1->addWidget(add_xml_decl);
    MarkupLayout1->addWidget(uppercase_tags);
    MarkupLayout1->addWidget(uppercase_attributes);
    MarkupLayout1->addWidget(bare);
    MarkupLayout1->addWidget(clean);
    MarkupLayout1->addWidget(gdoc);
    MarkupLayout1->addWidget(logical_emphasis);
    MarkupLayout1->addWidget(drop_proprietary_attributes);
    MarkupLayout1->addWidget(drop_font_tags);
    MarkupLayout1->addWidget(drop_empty_elements);
    MarkupLayout1->addWidget(drop_empty_paras);
    MarkupLayout1->addWidget(fix_bad_comments);
    MarkupLayout1->addWidget(numeric_entities);
    MarkupLayout1->addWidget(quote_marks);
    MarkupLayout1->addWidget(quote_nbsp);
    MarkupLayout1->addWidget(quote_ampersand);
    MarkupLayout1->addWidget(fix_backslash);
    MarkupLayout1->addWidget(assume_xml_procins);
    MarkupLayout1->addWidget(add_xml_space);
    MarkupLayout1->addWidget(enclose_text);
    MarkupLayout1->addWidget(enclose_block_text);
    QVBoxLayout *MarkupLayout2 = new QVBoxLayout;
    MarkupLayout2->addWidget(word_2000);
    MarkupLayout2->addWidget(literal_attributes);
    MarkupLayout2->addWidget(show_body_only);
    MarkupLayout2->addWidget(fix_uri);
    MarkupLayout2->addWidget(lower_literals);
    MarkupLayout2->addWidget(hide_comments);
    MarkupLayout2->addWidget(indent_cdata);
    MarkupLayout2->addWidget(join_classes);
    MarkupLayout2->addWidget(join_styles);
    MarkupLayout2->addWidget(escape_cdata);
    MarkupLayout2->addWidget(ncr);
    MarkupLayout2->addWidget(replace_color);
    MarkupLayout2->addWidget(css_prefix);
    MarkupLayout2->addWidget(css_prefixEd);
    MarkupLayout2->addWidget(new_inline_tags);
    MarkupLayout2->addWidget(new_inline_tagsEd);
    MarkupLayout2->addWidget(new_blocklevel_tags);
    MarkupLayout2->addWidget(new_blocklevel_tagsEd);
    MarkupLayout2->addWidget(new_empty_tags);
    MarkupLayout2->addWidget(new_empty_tagsEd);
    MarkupLayout2->addWidget(new_pre_tags);
    MarkupLayout2->addWidget(new_pre_tagsEd);
    MarkupLayout2->addWidget(merge_emphasis);
    MarkupLayout2->addWidget(merge_divs);
    MarkupLayout2->addWidget(decorate_inferred_ul);
    MarkupLayout2->addWidget(preserve_entities);
    MarkupLayout2->addWidget(merge_spans);
    MarkupLayout2->addWidget(anchor_as_name);
    QHBoxLayout *MarkupLayout = new QHBoxLayout;
    MarkupLayout->addLayout(MarkupLayout1);
    MarkupLayout->addLayout(MarkupLayout2);
    setLayout(MarkupLayout);
}

MiscTab::MiscTab( PINFOSTR pinf, QWidget *parent)
    : QWidget(parent)
{
    int i;
    QString s;

    QLabel *alt_text = new QLabel("alt-text (String)");
    s = getConfigStg("alt-text");
    QLineEdit *alt_textEd = new QLineEdit(s);

    QLabel *slide_style = new QLabel("slide-style (String)");
    s = getConfigStg("slide-style");
    QLineEdit *slide_styleEd = new QLineEdit(s);

    QLabel *error_file = new QLabel("error-file (String)");
    s = getConfigStg("error-file");
    QLineEdit *error_fileEd = new QLineEdit(s);

    QLabel *output_file = new QLabel("output-file (String)");
    s = getConfigStg("output-file");
    QLineEdit *output_fileEd = new QLineEdit(s);

    QCheckBox *write_back = new QCheckBox("write-back (Boolean)");
    if (getConfigBool("write-back")) {
        write_back->setChecked(true);
    }

    QCheckBox *quiet = new QCheckBox("quiet (Boolean)");
    if (getConfigBool("quiet")) {
        quiet->setChecked(true);
    }

    QCheckBox *keep_time = new QCheckBox("keep-time (Boolean)");
    if (getConfigBool("keep-time")) {
        keep_time->setChecked(true);
    }

    QCheckBox *tidy_mark = new QCheckBox("tidy-mark (Boolean)");
    if (getConfigBool("tidy-mark")) {
        tidy_mark->setChecked(true);
    }

    QCheckBox *gnu_emacs = new QCheckBox("gnu-emacs (Boolean)");
    if (getConfigBool("gnu-emacs")) {
        gnu_emacs->setChecked(true);
    }

    QLabel *gnu_emacs_file = new QLabel("gnu-emacs-file (String)");
    s = getConfigStg("gnu-emacs-file");
    QLineEdit *gnu_emacs_fileEd = new QLineEdit(s);

    QCheckBox *force_output = new QCheckBox("force-output (Boolean)");
    if (getConfigBool("force-output")) {
        force_output->setChecked(true);
    }
    QVBoxLayout *MiscLayout = new QVBoxLayout;
    MiscLayout->addWidget(alt_text);
    MiscLayout->addWidget(alt_textEd);
    MiscLayout->addWidget(slide_style);
    MiscLayout->addWidget(slide_styleEd);
    MiscLayout->addWidget(error_file);
    MiscLayout->addWidget(error_fileEd);
    MiscLayout->addWidget(output_file);
    MiscLayout->addWidget(output_fileEd);
    MiscLayout->addWidget(write_back);
    MiscLayout->addWidget(quiet);
    MiscLayout->addWidget(keep_time);
    MiscLayout->addWidget(tidy_mark);
    MiscLayout->addWidget(gnu_emacs);
    MiscLayout->addWidget(gnu_emacs_file);
    MiscLayout->addWidget(gnu_emacs_fileEd);
    MiscLayout->addWidget(force_output);
    MiscLayout->addStretch(1);
    setLayout(MiscLayout);
}

PrintTab::PrintTab( PINFOSTR pinf, QWidget *parent)
    : QWidget(parent)
{
    int i;
    QString s;

    QLabel *indent_spaces = new QLabel("indent-spaces (Integer)");
    i = getConfigInt("indent-spaces");
    s = QString::number(i);
    QLineEdit *indent_spacesEd = new QLineEdit(s);
    indent_spacesEd->setMaximumWidth(50);

    QLabel *wrap = new QLabel("wrap (Integer)");
    i = getConfigInt("wrap");
    s = QString::number(i);
    QLineEdit *wrapEd = new QLineEdit(s);
    wrapEd->setMaximumWidth(50);

    QLabel *tab_size = new QLabel("tab-size (Integer)");
    i = getConfigInt("tab-size");
    s = QString::number(i);
    QLineEdit *tab_sizeEd = new QLineEdit(s);
    tab_sizeEd->setMaximumWidth(50);

    QCheckBox *markup = new QCheckBox("markup (Boolean)");
    if (getConfigBool("markup")) {
        markup->setChecked(true);
    }

    QCheckBox *indent = new QCheckBox("indent (AutoBool)");
    if (getConfigBool("indent")) {
        indent->setChecked(true);
    }

    QCheckBox *break_before_br = new QCheckBox("break-before-br (Boolean)");
    if (getConfigBool("break-before-br")) {
        break_before_br->setChecked(true);
    }

    QCheckBox *split = new QCheckBox("split (Boolean)");
    if (getConfigBool("split")) {
        split->setChecked(true);
    }

    QCheckBox *wrap_attributes = new QCheckBox("wrap-attributes (Boolean)");
    if (getConfigBool("wrap-attributes")) {
        wrap_attributes->setChecked(true);
    }

    QCheckBox *wrap_script_literals = new QCheckBox("wrap-script-literals (Boolean)");
    if (getConfigBool("wrap-script-literals")) {
        wrap_script_literals->setChecked(true);
    }

    QCheckBox *wrap_sections = new QCheckBox("wrap-sections (Boolean)");
    if (getConfigBool("wrap-sections")) {
        wrap_sections->setChecked(true);
    }

    QCheckBox *wrap_asp = new QCheckBox("wrap-asp (Boolean)");
    if (getConfigBool("wrap-asp")) {
        wrap_asp->setChecked(true);
    }

    QCheckBox *wrap_jste = new QCheckBox("wrap-jste (Boolean)");
    if (getConfigBool("wrap-jste")) {
        wrap_jste->setChecked(true);
    }

    QCheckBox *wrap_php = new QCheckBox("wrap-php (Boolean)");
    if (getConfigBool("wrap-php")) {
        wrap_php->setChecked(true);
    }

    QCheckBox *indent_attributes = new QCheckBox("indent-attributes (Boolean)");
    if (getConfigBool("indent-attributes")) {
        indent_attributes->setChecked(true);
    }

    QCheckBox *vertical_space = new QCheckBox("vertical-space (Boolean)");
    if (getConfigBool("vertical-space")) {
        vertical_space->setChecked(true);
    }

    QCheckBox *punctuation_wrap = new QCheckBox("punctuation-wrap (Boolean)");
    if (getConfigBool("punctuation-wrap")) {
        punctuation_wrap->setChecked(true);
    }

    QGroupBox *sort_attributes = new QGroupBox("sort-attributes");
    QComboBox *sort_attributesComb = new QComboBox();
    sort_attributesComb->addItem("none","none");
    sort_attributesComb->addItem("alpha","alpha");
    s = getConfigEnc("sort-attributes");
    i = sort_attributesComb->findData(s);
    if (i != -1) {
        sort_attributesComb->setCurrentIndex(i);
    }
    QVBoxLayout *sort_attributesLay = new QVBoxLayout;
    sort_attributesLay->addWidget(sort_attributesComb);
    sort_attributesLay->addStretch(1);
    sort_attributes->setLayout(sort_attributesLay);

    QCheckBox *indent_with_tabs = new QCheckBox("indent-with-tabs (Boolean)");
    if (getConfigBool("indent-with-tabs")) {
        indent_with_tabs->setChecked(true);
    }
    QVBoxLayout *PrintLayout = new QVBoxLayout;
    PrintLayout->addWidget(indent_spaces);
    PrintLayout->addWidget(indent_spacesEd);
    PrintLayout->addWidget(wrap);
    PrintLayout->addWidget(wrapEd);
    PrintLayout->addWidget(tab_size);
    PrintLayout->addWidget(tab_sizeEd);
    PrintLayout->addWidget(markup);
    PrintLayout->addWidget(indent);
    PrintLayout->addWidget(break_before_br);
    PrintLayout->addWidget(split);
    PrintLayout->addWidget(wrap_attributes);
    PrintLayout->addWidget(wrap_script_literals);
    PrintLayout->addWidget(wrap_sections);
    PrintLayout->addWidget(wrap_asp);
    PrintLayout->addWidget(wrap_jste);
    PrintLayout->addWidget(wrap_php);
    PrintLayout->addWidget(indent_attributes);
    PrintLayout->addWidget(vertical_space);
    PrintLayout->addWidget(punctuation_wrap);
    PrintLayout->addWidget(sort_attributes);
    PrintLayout->addWidget(indent_with_tabs);
    PrintLayout->addStretch(1);
    setLayout(PrintLayout);
}
/////////////////////////////////////////////////////////////////////////