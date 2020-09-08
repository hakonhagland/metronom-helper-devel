#ifndef WINDOW_SET_VOLUME_HPP
#define WINDOW_SET_VOLUME_HPP
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QStatusBar>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
class MainWindow;

class WindowSetVolume: public QMainWindow
{
    Q_OBJECT

public:
    ~WindowSetVolume(){}
    WindowSetVolume(MainWindow *mainWindow);
    
    void Execute();
private:
    void createActions();
    QPushButton *ok_button_;
    QPushButton *cancel_button_;
    QSlider *slider_;
    MainWindow *mainWindow_;
                           
private slots:
    void ok();
    void cancel();
};

#endif
