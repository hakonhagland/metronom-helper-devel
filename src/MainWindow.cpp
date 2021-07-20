#include "MainWindow.hpp"
#include "WindowSetTimeSig.hpp"
#include "WindowSetVolume.hpp"
#include "WindowSetBPM.hpp"
#include "BeatCircleTimer.hpp"
#include "BpmCalculator.hpp"
#include <QPushButton>

MainWindow::MainWindow() : timeSignature_{3,4} {
    createActions();
    createMenus();
    QString message = tr("Use the pull down menu to quit the application");
    statusBar()->showMessage(message);
    createTimer();
    createBeatCircleTimer();
    floatVolume_ = 1.0f;
    currentBpm_ = 60;
    setBpmDialogLastBpm_ = currentBpm_;
    bpmCalculator_ = std::make_unique<BpmCalculator>(this);
    scrollBar_ = new QScrollBar(Qt::Horizontal);
    setScrollBar(scrollBar_);
    connect(scrollBar_,SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
}

void MainWindow::sliderChanged(int value) {
    qInfo() << "slider change: " << value;
}

void MainWindow::createTimer()
{
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(timerEvent()));
}

void MainWindow::Execute()
{
    show();
}

void MainWindow::setTimeSignature(int top, int bottom) {
    timeSignature_ = TimeSignature {top, bottom};
}

RenderArea *MainWindow::createRenderArea() {
    renderArea_ = new RenderArea(this);
    return renderArea_;
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));
    actionsMenu = menuBar()->addMenu(tr("&Actions"));
    fileMenu->addAction(quitAct);
    fileMenu->addAction(saveAct);
    editMenu->addAction(addBarAct);
    editMenu->addAction(setTimeSigAct);
    editMenu->addAction(setBpmAct);
    actionsMenu->addAction(playAct);
    actionsMenu->addAction(stopAct);
    actionsMenu->addAction(volumeAct);
}

void MainWindow::createActions()
{
    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcut(QKeySequence(tr("Ctrl+Q")));
    connect(quitAct, &QAction::triggered, this, &MainWindow::quit);
    saveAct = new QAction(tr("&Save"), this);
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    addBarAct = new QAction(tr("&Add bar"), this);
    addBarAct->setShortcut(QKeySequence(tr("Ctrl+B")));
    connect(addBarAct, &QAction::triggered, this, &MainWindow::addBar);
    setTimeSigAct = new QAction(tr("Set &time signature"), this);
    connect(setTimeSigAct, &QAction::triggered, this,
        &MainWindow::openTimeSignatureDialog);
    setBpmAct = new QAction(tr("Set &BPM"), this);
    setBpmAct->setShortcut(QKeySequence(tr("Ctrl+T")));
    connect(setBpmAct, &QAction::triggered, this, &MainWindow::setBPM);
    //newAct->setShortcuts(QKeySequence::New);
    quitAct->setStatusTip(tr("Quit the application"));
    playAct = new QAction(tr("&Play"), this);
    playAct->setShortcut(QKeySequence(tr("Ctrl+P")));
    connect(playAct, &QAction::triggered, this, &MainWindow::play);
    stopAct = new QAction(tr("St&op"), this);
    stopAct->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(stopAct, &QAction::triggered, this, &MainWindow::stop);
    volumeAct = new QAction(tr("Set &volume"), this);
    connect(volumeAct, &QAction::triggered, this, &MainWindow::setVolume);
}

void MainWindow::quit() {
    QApplication::quit();
}

void MainWindow::addBar() {
    renderArea_->addBar();
    update();
}

void MainWindow::save() {
    QApplication::quit();
}

void MainWindow::updateBpm(
       int bpm, [[maybe_unused]] int gridPos, [[maybe_unused]] int barPos)
{
    double delay = convertBpmToMilliSec_(bpm);
    currentBpm_ = bpm;

    getTimer()->setInterval((int) delay);
}

double MainWindow::convertBpmToMilliSec_(int bpm)
{
    return (60.0/bpm) * 1000;
}

void MainWindow::play() {
    renderArea_->setPlaying(true);
    timer_->setSingleShot(true);
    this->playSoundAndAnimate();
}

void MainWindow::beatCircleTimerEvent()
{
    beatCircleTimer_->timerEvent();
}

void MainWindow::playSoundAndAnimate()
{
    int beatsPerBar = this->getTimeSignature().getTop();
    int max = renderArea_->getBarCount() * beatsPerBar;
    int metroPos = renderArea_->getMetroPos();
    if (metroPos >= max) {
        return;
    }
    effect_ = new QSoundEffect(this);
    effect_->setSource(QUrl::fromLocalFile("../res/metro4_amp.wav"));
    effect_->setLoopCount(1);
    effect_->setVolume(floatVolume_);
    effect_->play();
    beatCircleTimer_->startTimer();
    double delay;
    if (metroPos < (max - 1)) {
        double bpm1 = bpmCalculator_->calculate(metroPos);
        double bpm2 = bpmCalculator_->calculate(metroPos+1);
        delay = 120 / (bpm1 + bpm2);
    }
    else {
        double bpm = bpmCalculator_->calculateLastPoint();
        delay = 60 / bpm;
    }
    renderArea_->updateMetroPos();
    timer_->start(delay * 1000);
    update();
}

void MainWindow::timerEvent()
{
    this->playSoundAndAnimate();
}

void MainWindow::createBeatCircleTimer()
{
    beatCircleTimer_ = new BeatCircleTimer(this);
    connect(beatCircleTimer_, SIGNAL(timeout()), this, SLOT(beatCircleTimerEvent()));
}

void MainWindow::stop() {
    renderArea_->setPlaying(false);
    timer_->stop();
    //update();
}

void MainWindow::positionWindow_(QMainWindow *dialog, int dw = 400, int dh = 300)
{
    QRect qrect = this->frameGeometry();
    int x = qrect.x();
    int y = qrect.y();
    int w = qrect.width();
    int h = qrect.height();

    int xx = x + (w-dw)/2;
    int yy = y + (h-dh)/2;
    dialog->resize(dw, dh);
    dialog->move(xx,yy);
}


void MainWindow::setFloatVolume(float v)
{
    floatVolume_ = v;
}

void MainWindow::setVolume()
{
    volumeDialog_ = new WindowSetVolume(this);
    positionWindow_(volumeDialog_);
    volumeDialog_->setWindowTitle("Set volume");
    volumeDialog_->Execute();
}

void MainWindow::openBpmDialog(int barPos, int gridPos) {
    bpmDialog_ = new WindowSetBPM {this, barPos, gridPos, this->setBpmDialogLastBpm_};
    positionWindow_(bpmDialog_, 400, 200);
    bpmDialog_->setWindowTitle("Select a value for BPM");
    bpmDialog_->Execute();
}

void MainWindow::setBPM() {
    int barPos = 1;
    int gridPos = 1;
    openBpmDialog(barPos, gridPos);
}

void MainWindow::openTimeSignatureDialog() {
    timeSigDialog_ = new WindowSetTimeSig(this);
    positionWindow_(timeSigDialog_);
    timeSigDialog_->setWindowTitle("Choose time signature");
    timeSigDialog_->Execute();
}

QTimer *MainWindow::getTimer()
{
    return timer_;
}
