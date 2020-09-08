#include "RenderArea.hpp"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QPainterPath>

#include "MainWindow.hpp"

RenderArea::RenderArea(MainWindow *mainWindow)
    : QWidget(mainWindow),
      mainWindow_(mainWindow),
      playing_{false},
      metroPos_{0},
      barLeftMargin_{10},
      barWidth_{120},
      barHorizontalSpacing_{10},
      barCount_{0},
      barTopOffset_{10},
      barHeight_{75}
{

}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint lastPoint = event->pos();
        qInfo() << "point = " << lastPoint;
    }
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

// horizontal position of current beat circle
void RenderArea::updateMetroPos()
{
    metroPos_ += 1;
}

void RenderArea::setPlaying(bool value)
{
    playing_ = value;
}


// INPUT:
//  - pos = current bar number (0, 1, 2, ...)
void RenderArea::drawBar(QPainter &painter, QPainterPath &path, int pos)
{
    QColor BLACK_COLOR{0, 0, 0};
    painter.setPen(QPen(BLACK_COLOR, 6));
    painter.setBrush(Qt::NoBrush);
    int xpos = pos*(barWidth_ + barHorizontalSpacing_) + barLeftMargin_;
    int ypos2 = 0;  // TODO: fix this
    int ypos = barTopOffset_ + ypos2;
    path.setFillRule( Qt::WindingFill );
    path.addRoundedRect( QRect(xpos,ypos,barWidth_,barHeight_),
        20, 20, Qt::RelativeSize );
    painter.drawPath(path);
    drawBarCircles(painter, xpos, ypos, pos );
}

// Draws the circles within a single
// INPUT:
//  - xpos : horizontal pixel position of the bar
//  - ypos : vertical pixel position of the bar
void RenderArea::drawBarCircles(QPainter &painter, int xpos, int ypos, int barNum )
{
    int radius = (int) ( barWidth_ * BAR_CIRCLE_RADIUS_SIZE);
    QColor green {0, 200, 0};
    painter.setPen(QPen(green, 1));
    painter.setBrush(QBrush(green, Qt::SolidPattern));
    int numCircles = mainWindow_->getTimeSignature().getTop();

    // horizontal margin for circles within the bar
    int pos0 = (int) (barWidth_ * BAR_CIRCLE_MARGIN);

    int pos = xpos + pos0;
    //qInfo() << "BAR_CIRCLE_MARGIN = " <<BAR_CIRCLE_MARGIN;
    //qInfo() << "pos0 = " << pos0;
    //qInfo() << "pos = " << pos;
    //qInfo() << "xpos = " << xpos;
    //qInfo() << "barWidth_ = " << barWidth_;
    int delta = (int) ((barWidth_ - (pos0 *2)) / (numCircles-1));
    //qInfo() << "delta = " << delta;
    for (int i = 1; i <= numCircles; i++) {
        drawSingleBarCircleWithAnimation(painter, pos, ypos, radius, barNum, i);
        pos += delta;
    }
}

void RenderArea::drawSingleBarCircleWithAnimation(
    QPainter &painter, int xpos, int bar_ypos, int radius, int barNum, int circleNum )
{
    int ypos = bar_ypos+barHeight_/2;
    int numCircles = mainWindow_->getTimeSignature().getTop();
    int currentPos = barNum*numCircles + circleNum;
    if (metroPos_ > 0) {
        if ( currentPos == metroPos_) {
            double radiusDelta = mainWindow_->getBeatCircleTimer()->getRadiusDelta();
            radius += radiusDelta;
        }
    }
    painter.drawEllipse(QPoint(xpos, ypos), radius, radius);

}

void RenderArea::addBar()
{
    barCount_ += 1;
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (barCount_ > 0) {
        QPainterPath path;
        for (int i = 0; i < barCount_; i++) {
            drawBar(painter, path, i);
        }
    }
    /*
    if (playing_) {
        QColor RED_COLOR{255, 0, 0};
        QPen pen {RED_COLOR, 3};
        pen.setJoinStyle(Qt::RoundJoin);
        //pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.drawLine(metroPos_, 10, metroPos_, 70);
        }*/

}
