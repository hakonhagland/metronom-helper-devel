#include "BeatCircleTimer.hpp"
#include "MainWindow.hpp"

BeatCircleTimer::BeatCircleTimer(MainWindow *mainWindow)
    : QTimer(),
      mainWindow_(mainWindow),
      currentStep_(0),
      interval_(100),
      maxSteps_(5),
      state_{State::STOPPED}
{

}

double BeatCircleTimer::getRadiusDelta()
{
    return currentStep_ + 1;
}

void BeatCircleTimer::timerEvent()
{
    if (state_ == State::UP ) {
        if (currentStep_ == (maxSteps_-1) ) {
            state_ = State::DOWN;
            currentStep_ -= 1;
        }
        else {
            currentStep_++;
        }
    }
    else {
        if (currentStep_ == 0) {
            state_ = State::STOPPED;
            stop();
        }
        else {
            currentStep_--;
        }
    }

    mainWindow_->update();
}

void BeatCircleTimer::startTimer()
{
    state_ = State::UP;
    currentStep_ = 0;
    // interval in ms
    start(interval_);  //calls start QTimer function in parent class
}

