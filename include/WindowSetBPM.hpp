#ifndef WINDOW_SET_BPM_HPP
#define WINDOW_SET_BPM_HPP
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QStatusBar>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
class MainWindow;

class WindowSetBPM: public QMainWindow
{
    Q_OBJECT

public:
    ~WindowSetBPM(){}
    WindowSetBPM(MainWindow *main_window);
    
    void Execute();
private:
    void createActions();
    QLabel *label_;
    QSlider *slider_;
    QPushButton *ok_button_;
    QPushButton *cancel_button_;
    MainWindow *mainWindow_;
private slots:
    void updateBPM(int);
    void updateBPM2(int);
    void ok();
    void cancel();
};

#endif