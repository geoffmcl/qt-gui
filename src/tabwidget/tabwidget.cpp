/*
   20150606: Test to set TabWidget tab bar different colors
   from : http://www.codeprogress.com/cpp/libraries/qt/qtChangeTabTextColor.php#.VXLZZs-qpBc
    Summary : This code shows how to add a tab to QTabWidget and change its tab title color.
    Qt Version:  4.7.4   
    
    If QT GUI styling such as background or foreground coloring does not work as expected , check your qtconfig settings!
    
 */

#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>


/**
 *  Derived Class from QTabWidget.
 *  Protected QTabWidget::tabBar() method is 
 *  overridden in order to make it accessible.
 */

class MyTabWidget:public QTabWidget
{
public:
    MyTabWidget(QWidget* parent = 0)
    {
      setParent(parent);
    }
    
    //Overridden method from QTabWidget
    QTabBar* tabBar()
    {
      return QTabWidget::tabBar();
    }
};

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QMainWindow *window = new QMainWindow();
    
    
        window->setWindowTitle(QString::fromUtf8("MyWindow"));
        window->resize(336, 227);
	
        QWidget *centralWidget = new QWidget(window);
	MyTabWidget *tabs = new MyTabWidget(centralWidget);
	
	tabs->setFixedSize(330, 220);

  	tabs->addTab(new QWidget(),"TAB 1");  
	tabs->addTab(new QWidget(),"TAB 2");
	tabs->addTab(new QWidget(),"TAB 3");
	
	tabs->tabBar()->setTabTextColor(0,Qt::blue);
	tabs->tabBar()->setTabTextColor(1,Qt::red);
	tabs->tabBar()->setTabTextColor(2,Qt::green);
	
	window->setCentralWidget(centralWidget);

	window->show();
	return app.exec();
}

// eof
