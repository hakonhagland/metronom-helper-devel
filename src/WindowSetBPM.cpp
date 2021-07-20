#include "MainWindow.hpp"
#include "WindowSetBPM.hpp"
#include <QString>

WindowSetBPM::WindowSetBPM(
    MainWindow *main_window, int barPos, int gridPos, int lastBpm
) :
    mainWindow_(main_window), barPos_(barPos), gridPos_(gridPos)
{
    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);
    QGridLayout *layout = new QGridLayout(widget);
    slider_ = new QSlider(Qt::Horizontal, this);
    slider_->setMinimum(this->mainWindow_->renderArea()->bpmMin());
    slider_->setMaximum(this->mainWindow_->renderArea()->bpmMax());
    slider_->setValue(lastBpm);
    layout->addWidget(slider_, 0, 0, 1, 2, Qt::AlignVCenter);
    std::string str = std::to_string(lastBpm);
    label_ = new QLabel(str.c_str(), this);
    layout->addWidget(label_, 1, 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop);
    ok_button_ = new QPushButton(tr("Ok"), this);
    layout->addWidget(ok_button_, 2, 0, 1, 1);
    cancel_button_ = new QPushButton(tr("Cancel"), this);
    layout->addWidget(cancel_button_, 2, 1, 1,1);
    widget->setLayout(layout);
    setWindowModality(Qt::ApplicationModal);
    createActions();
}

void WindowSetBPM::Execute()
{
    show();
}

void WindowSetBPM::createActions()
{
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(updateBPM2(int)));
    connect(slider_, SIGNAL(sliderMoved(int)), this, SLOT(updateBPM(int)));
    connect(ok_button_, SIGNAL(released()), this, SLOT(ok()));
    connect(cancel_button_, SIGNAL(released()), this, SLOT(cancel()));
}

// updateBPM2() is called when the slider is clicked
void WindowSetBPM::updateBPM2(int value) {
    updateBPM(value);  // forwards the value to updateBPM()
};

// called when the slider is dragged
void WindowSetBPM::updateBPM(int value) {
    QString txt = QString::number(value);
    label_->setText(txt);
    mainWindow_->updateBpm(value, gridPos_, barPos_);
    this->mainWindow_->setBpmDialogLastBpm(value);
};

void WindowSetBPM::cancel() {
    close();
};

void WindowSetBPM::ok() {
    close();
};
