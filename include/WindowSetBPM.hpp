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
    WindowSetBPM(MainWindow *main_window, int barPos, int gridPos, int lastBpm);

    void Execute();
private:
    void createActions();
    MainWindow *mainWindow_;
    int barPos_;
    int gridPos_;
    QLabel *label_;
    QSlider *slider_;
    QPushButton *ok_button_;
    QPushButton *cancel_button_;
private slots:
    void updateBPM(int);
    void updateBPM2(int);
    void ok();
    void cancel();
};

#endif
