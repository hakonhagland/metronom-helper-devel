#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSoundEffect>
#include <QStatusBar>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "RenderArea.hpp"
#include "BeatCircleTimer.hpp"
#include "WindowSetTimeSig.hpp"
#include "WindowSetVolume.hpp"
#include "WindowSetBPM.hpp"

class RenderArea;
class BeatCircleTimer;

class TimeSignature {
public:
    TimeSignature(int top, int bottom) : top_(top), bottom_(bottom) { }

    int getTop() { return top_; }
    int top_;
    int bottom_;
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow(){}
    //
    void Execute();
    double convertBpmToMilliSec_(int bpm);
    RenderArea *createRenderArea();
    RenderArea *getRenderArea() {return renderArea_; }
    QTimer *getTimer();
    BeatCircleTimer *getBeatCircleTimer() { return beatCircleTimer_; };
    TimeSignature& getTimeSignature() { return timeSignature_; };
    void setFloatVolume(float v);
    void setTimeSignature(int top, int bottom);
    void updateBpm(int bpm);
private:
    void createActions();
    void createMenus();
    void createTimer();
    void createBeatCircleTimer();
    void positionWindow_(QMainWindow *dialog, int dw, int dh);
    RenderArea *renderArea_;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *actionsMenu;
    QAction *quitAct;
    QAction *saveAct;
    QAction *addBarAct;
    QAction *setTimeSigAct;
    QAction *setBpmAct;
    QAction *playAct;
    QAction *stopAct;
    QAction *volumeAct;
    WindowSetTimeSig *timeSigDialog_;
    WindowSetVolume *volumeDialog_;
    WindowSetBPM *bpmDialog_;
    bool playing_;
    float floatVolume_;
    QTimer *timer_;
    BeatCircleTimer *beatCircleTimer_;
    QSoundEffect *effect_;
    TimeSignature timeSignature_;
    int currentBpm_;


private slots:
    void save();
    void addBar();
    void quit();
    void setVolume();
    void play();
    void stop();
    void openTimeSignatureDialog();
    void setBPM();
    void timerEvent();
    void beatCircleTimerEvent();
};

#endif
