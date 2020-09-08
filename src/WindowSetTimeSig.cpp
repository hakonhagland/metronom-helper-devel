#include "MainWindow.hpp"
#include "WindowSetTimeSig.hpp"

WindowSetTimeSig::WindowSetTimeSig(MainWindow *mainWindow) {
    mainWindow_ = mainWindow;
    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);
    QGridLayout *layout = new QGridLayout(widget);
    choice1_ = new QRadioButton(tr("3/4 signature"), this);
    layout->addWidget(choice1_, 0, 0);
    choice2_ = new QRadioButton(tr("4/4 signature"), this);
    layout->addWidget(choice2_, 1, 0);
    ok_button_ = new QPushButton(tr("Ok"), this);
    //ok_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
    layout->addWidget(ok_button_, 2, 0);
    cancel_button_ = new QPushButton(tr("Cancel"), this);
    //ok_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
    layout->addWidget(cancel_button_, 2, 1);
    widget->setLayout(layout);
    //setWindowModality(Qt::WindowModal);
    setWindowModality(Qt::ApplicationModal);
    createActions();
}

void WindowSetTimeSig::Execute()
{
    show();
}

void WindowSetTimeSig::createActions()
{
    connect(ok_button_, SIGNAL(released()), this, SLOT(ok()));
    connect(cancel_button_, SIGNAL(released()), this, SLOT(cancel()));
    //connect(ok_button, SIGNAL (released()), this, &WindowSetTimeSig::ok);
    //quitAct = new QAction(tr("&Quit"), this);
    //connect(quitAct, &QAction::triggered, this, &MainWindow::quit);
}

void WindowSetTimeSig::cancel() {
    QApplication::quit();
};

void WindowSetTimeSig::ok() {
    int top;
    int bottom;
    if (choice1_->isChecked()) {
        top = 3;
        bottom = 4;
    }
    else {
        top = 4;
        bottom = 4;
    }
    mainWindow_->setTimeSignature(top, bottom);
    close();
    //QApplication::quit();
};

void WindowSetTimeSig::choice1() {
    QApplication::quit();
};

void WindowSetTimeSig::choice2() {
    QApplication::quit();
};

