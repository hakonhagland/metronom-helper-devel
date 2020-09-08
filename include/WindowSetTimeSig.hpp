#ifndef WINDOW_SET_TIME_SIG_HPP
#define WINDOW_SET_TIME_SIG_HPP
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QStatusBar>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
class MainWindow;

class WindowSetTimeSig: public QMainWindow
{
    Q_OBJECT

public:
    ~WindowSetTimeSig(){}
    WindowSetTimeSig(MainWindow *mainWindow);
    
    void Execute();
private:
    void createActions();
    QPushButton *ok_button_;
    QPushButton *cancel_button_;
    QRadioButton *choice1_;
    QRadioButton *choice2_;
    MainWindow *mainWindow_;
private slots:
    void ok();
    void cancel();
    void choice1();
    void choice2();
};

#endif
