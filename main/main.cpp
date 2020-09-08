#include <QApplication>
#include <QGridLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QString>
#include <QWidget>
#include "../include/MainWindow.hpp"
#include "../include/RenderArea.hpp"

int main(int argc, char *argv[ ])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle(QString::fromUtf8("MainWindow"));
    QWidget *widget = new QWidget(&window);
    QGridLayout *layout = new QGridLayout(widget);
    window.setCentralWidget(widget);
    widget->setLayout(layout);
    RenderArea *renderArea = window.createRenderArea();
    layout->addWidget(renderArea, 0, 0);
    window.resize(1200, 900);
    window.setWindowTitle("Simple example");
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int screenHeight = screenGeometry.height();
    int screenWidth = screenGeometry.width();
    int x = (screenWidth - window.width()) / 2;
    int y = (screenHeight - window.height()) / 2;
    window.move(x, y);
    window.Execute();
    return app.exec();

}
