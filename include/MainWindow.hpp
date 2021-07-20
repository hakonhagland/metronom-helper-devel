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
#include <QScrollBar>
#include <QSoundEffect>
#include <QStatusBar>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "BpmCalculator.hpp"
#include "RenderArea.hpp"
#include "BeatCircleTimer.hpp"
#include "WindowSetTimeSig.hpp"
#include "WindowSetVolume.hpp"
#include "WindowSetBPM.hpp"

#include <memory>

class BpmCalculator;
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
    double convertBpmToMilliSec_(int bpm);
    RenderArea *createRenderArea();
    void Execute();
    int getCurrentBpm() { return currentBpm_; }
    RenderArea *getRenderArea() {return renderArea_; }
    QScrollBar *getScrollBar() { return this->scrollBar_; }
    QTimer *getTimer();
    BeatCircleTimer *getBeatCircleTimer() { return beatCircleTimer_; };
    TimeSignature& getTimeSignature() { return timeSignature_; };
    void openBpmDialog(int barPos, int gridPos);
    void setBpmDialogLastBpm(int bpm) { this->setBpmDialogLastBpm_ = bpm; }
    void setFloatVolume(float v);
    void setScrollBar(QScrollBar *scrollBar) { this->scrollBar_ = scrollBar; }
    void setTimeSignature(int top, int bottom);
    RenderArea *renderArea() { return renderArea_; }
    void updateBpm(int bpm, int gridPos, int barPos);
private:
    void createActions();
    void createMenus();
    void createTimer();
    void createBeatCircleTimer();
    void playSoundAndAnimate();
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
    int setBpmDialogLastBpm_;
    std::unique_ptr<BpmCalculator> bpmCalculator_;
    QScrollBar *scrollBar_;

private slots:
    void save();
    void addBar();
    void quit();
    void setVolume();
    void play();
    void stop();
    void openTimeSignatureDialog();
    void timerEvent();
    void setBPM();
    void beatCircleTimerEvent();
    void sliderChanged(int);
};

#endif
