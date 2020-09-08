#ifndef BEAT_CIRCLE_TIMER_H
#define BEAT_CIRCLE_TIMER_H

#include "MainWindow.hpp"

class MainWindow;

class BeatCircleTimer : public QTimer
{
    Q_OBJECT

public:
    //RenderArea(QWidget *parent = 0, QImage *image);
    BeatCircleTimer(MainWindow *parent);
    void timerEvent();
    void startTimer();
    double getRadiusDelta();

    enum class State {STOPPED, UP, DOWN};
public slots:

private:
    MainWindow *mainWindow_;
    int currentStep_;  // current step number
    double interval_;   // delay between ...
    int maxSteps_;  // number of animation steps for a beat
    State state_;  // current state
};

#endif // BEAT_CIRCLE_TIMER_H
