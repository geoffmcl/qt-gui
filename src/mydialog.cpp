#include <QDialog>
#include "mydialog.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QListWidget;
class QListWidgetItem;

MyDialog::MyDialog(QStringList list, QWidget *parent) :
    QDialog(parent)
{
    int i;
    //label = new QLabel(tr("Polygon list:"));
    //lineEdit = new QLineEdit;
    //label->setBuddy(lineEdit);

    listWidget = new QListWidget;
    listWidget->setSelectionMode( QAbstractItemView::MultiSelection );
    for (i = 0; i < list.size(); i++) {
        QString s = list.at(i);
        listWidget->addItem(s);
        fullList << s;  // Keep local copy for restore
    }
    listWidget->setWindowTitle("List of Polygons");

    //caseCheckBox = new QCheckBox(tr("Match &case"));
    closeButton = new QPushButton(tr("Close"));
    restoreButton = new QPushButton(tr("Restore"));
    deleteButton = new QPushButton(tr("Delete"));

    connect(closeButton, SIGNAL(clicked()),this, SLOT(close()));
    connect(restoreButton, SIGNAL(clicked()),this, SLOT(restoreItems()));
    connect(deleteButton, SIGNAL(clicked()),this, SLOT(deleteItem()));

    QVBoxLayout *Layout = new QVBoxLayout;
    Layout->addWidget(listWidget);
    //Layout->addWidget(caseCheckBox);

    QHBoxLayout *hozLayout = new QHBoxLayout;
    hozLayout->addWidget(restoreButton);
    hozLayout->addWidget(deleteButton);
    hozLayout->addWidget(closeButton);

    //Layout->addWidget(closeButton);
    //setLayout(Layout);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(Layout);
    mainLayout->addLayout(hozLayout);
    setLayout(mainLayout);

#if 0 // 00000000000000000000000000
    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    //topLeftLayout->addWidget(label);
    //topLeftLayout->addWidget(lineEdit);
    topLeftLayout->addWidget(listWidget);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(caseCheckBox);
    // leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    // rightLayout->addWidget(findButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
#endif // 000000000000000000000000000000000000000

    setWindowTitle(tr("Polygons Selected"));
    setFixedHeight(sizeHint().height());
}

void MyDialog::deleteItem()
{
    //int ci = listWidget->currentItem();
    //if ((ci >= 0) && (ci < listWidget->size())) {
    //    listWidget->takeItem(listWidget->row(ci));
    //}
    QList<QListWidgetItem*> selection = listWidget->selectedItems();
    int sel = selection.size();
    if (sel) {
        //QListWidgetItem *item = listWidget->currentItem();
        //if (item) {
        // listWidget->takeItem(listWidget->row(item));
        //    int r = listWidget->row(item);
        //    listWidget->takeItem(r);
        //    delete item;
        //}
        for (int i = 0; i < sel; i++) {
            QListWidgetItem *item = selection.at(i);
            if (item) {
                int r = listWidget->row(item);
                listWidget->takeItem(r);
                delete item;
            }

        }
    }
}

void MyDialog::deleteAll()
{
    if (listWidget->count() != 0)
      listWidget->clear();
}

void MyDialog::restoreItems()
{
    deleteAll();
    for (int i = 0; i < fullList.size(); i++) {
        QString s = fullList.at(i);
        listWidget->addItem(s);
    }
}

// eof
