#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QWidget>

#include "MainWindow.hpp"

class MainWindow;

class RenderArea : public QWidget
{
    Q_OBJECT

    // fraction of bar width
    constexpr static double BAR_CIRCLE_MARGIN = 1.0/9;
    // fraction of bar width
    constexpr static double BAR_CIRCLE_RADIUS_SIZE = 5.0/100;

public:
    //RenderArea(QWidget *parent = 0, QImage *image);
    RenderArea(MainWindow *parent);
    void setPlaying(bool value);
    void updateMetroPos();
    QSize minimumSizeHint() const override;
    void mousePressEvent(QMouseEvent *event);
    QSize sizeHint() const override;
    void addBar();

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBar(QPainter &painter, QPainterPath &path, int pos);
    void drawBarCircles(QPainter &painter, int xpos, int ypos, int barNum );
    void drawSingleBarCircleWithAnimation(
        QPainter &painter, int pos, int ypos, int radius, int barNum, int circleNum);
    MainWindow *mainWindow_;
    bool playing_;
    int metroPos_;
    int barLeftMargin_;
    int barWidth_;
    int barHorizontalSpacing_;
    int barCount_;
    int barTopOffset_;
    int barHeight_;
};

#endif // RENDERAREA_H
