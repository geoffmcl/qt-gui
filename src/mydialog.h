#ifndef MYDIALOG_H
#define MYDIALOG_H
#include <QtGui>
#include <QStringList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDialog>

class MyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MyDialog(QStringList list, QWidget *parent = 0);

signals:

public slots:
    void deleteItem();
    void restoreItems();
    void deleteAll();

private:
    //QLabel *label;
    //QLineEdit *lineEdit;
    QListWidget *listWidget;
    // QCheckBox *caseCheckBox;
    QPushButton *closeButton;
    QPushButton *restoreButton;
    QPushButton *deleteButton;
    QStringList fullList;

};

#endif // MYDIALOG_H
