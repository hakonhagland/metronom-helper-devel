#include "MainWindow.hpp"
#include "WindowSetVolume.hpp"

WindowSetVolume::WindowSetVolume(MainWindow *mainWindow) {
    mainWindow_ = mainWindow;
    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);
    QGridLayout *layout = new QGridLayout(widget);
    slider_ = new QSlider(Qt::Horizontal);
    slider_->setMinimum(0);
    slider_->setMaximum(10000);
    slider_->setValue(5000);
    layout->addWidget(slider_, 0, 0, 1, 2);
    ok_button_ = new QPushButton(tr("Ok"), this);
    //ok_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
    layout->addWidget(ok_button_, 1, 0);
    cancel_button_ = new QPushButton(tr("Cancel"), this);
    //ok_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
    layout->addWidget(cancel_button_, 1, 1);
    widget->setLayout(layout);
    //setWindowModality(Qt::WindowModal);
    setWindowModality(Qt::ApplicationModal);
    createActions();
}
    
void WindowSetVolume::Execute()
{
    show();
}

void WindowSetVolume::createActions()
{
    connect(ok_button_, SIGNAL(released()), this, SLOT(ok()));
    connect(cancel_button_, SIGNAL(released()), this, SLOT(cancel()));
    //connect(ok_button, SIGNAL (released()), this, &WindowSetTimeSig::ok);
    //quitAct = new QAction(tr("&Quit"), this);
    //connect(quitAct, &QAction::triggered, this, &MainWindow::quit);
}

void WindowSetVolume::cancel() {
    close();
};

void WindowSetVolume::ok() {
    qInfo() << "Min = " << slider_->minimum();
    qInfo() << "Max = " << slider_->maximum();
    qInfo() << "Value = " << slider_->value();
    int min = slider_->minimum();
    int max = slider_->maximum();
    int value = slider_->value();
    float f = ((float) value) / max;
    qInfo() << "Volume = " << f;
    mainWindow_->setFloatVolume(f);
    close();
    //QApplication::quit();
};


