#ifndef BPM_CALCULATOR_H
#define BPM_CALCULATOR_H

#include "MainWindow.hpp"

class MainWindow;

class BpmCalculator
{
public:
    BpmCalculator(MainWindow *parent);

    double calculate(int metroPos);
    double calculateLastPoint();

private:
    MainWindow *mainWindow_;
};

#endif // BPM_CALCULATOR_H
