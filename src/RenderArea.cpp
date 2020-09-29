#include "RenderArea.hpp"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QPainterPath>

#include "MainWindow.hpp"

void RenderArea::addBar()
{
    barCount_ += 1;
}


void RenderArea::calculateMouseBarPos(int x, int *barNo, int *gridPos) {
    int maxBarNo = barCount_;
    int minBarNo = 1;
    int xmin = calculateBarXmin(minBarNo-1);
    int xmax = calculateBarXmin(maxBarNo-1)+ barWidth_;
    if (x >= xmin && x <= xmax) {
        *barNo = ((int) (x - xmin) / barWidth_) + 1;
        int x2 = (x - xmin) % barWidth_;
        int numCircles = mainWindow_->getTimeSignature().getTop();
        int numGridPos = numCircles*gridResolution_;
        int gridCellWidth =  barWidth_ / numGridPos;
        *gridPos = (x2 / gridCellWidth) + 1;
    }
    else {
        *barNo = 0;
    }
    return;
}

bool RenderArea::checkBarMouseY(int y) {
    int y1 = barTopOffset_;
    int y2 = y1 + barHeight_;
    return y >= y1 && y <= y2;
}

int RenderArea::calculateBarXmin(int pos)
{
    return pos*(barWidth_ + barHorizontalSpacing_) + barLeftMargin_;
}

// INPUT:
//  - pos = current bar number (0, 1, 2, ...)
void RenderArea::drawBar(QPainter &painter, QPainterPath &path, int pos)
{
    QColor BLACK_COLOR{0, 0, 0};
    painter.setPen(QPen(BLACK_COLOR, 6));
    painter.setBrush(Qt::NoBrush);
    // xpos = left edge of rectangle
    int xpos = calculateBarXmin(pos);
    int ypos = barTopOffset_;
    path.setFillRule( Qt::WindingFill );
    path.addRoundedRect( QRect(xpos,ypos,barWidth_,barHeight_),
        20, 20, Qt::RelativeSize );
    painter.drawPath(path);
    drawBarCircles(painter, xpos, ypos, pos );
    drawGrid(painter, xpos, ypos, barWidth_, barHeight_);
}

void RenderArea::drawBPMgraph(QPainter &painter)
{

    if (barCount_ < 1) {
        return;
    }
    BpmGraph graph { *this, painter };

    graph.drawRectangle();
    graph.drawGraph();

}

void RenderArea::drawGrid(QPainter &painter, int xpos, int ypos, int width, int height )
{
    int numCircles = mainWindow_->getTimeSignature().getTop();
    QColor GRAY_COLOR{50, 50, 50};
    QPen pen {GRAY_COLOR, 1};
    pen.setStyle(Qt::DashLine);
    //pen.setJoinStyle(Qt::DashLine);
    //pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    int numLines = numCircles*gridResolution_+1;
    //qInfo() << "num lines = " << numLines;
    //qInfo() << "width = " << width;
    double delta = ((double) width) / (numLines-1);
    for (int i = 0; i < numLines; i++) {
        int xpos2 = (int) (xpos + delta*i);
        painter.drawLine(xpos2, ypos, xpos2, ypos+height);
    }
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
    int delta = (int) (barWidth_ / numCircles);
    int pos0 = delta/2;

    int pos = xpos + pos0;
    //qInfo() << "BAR_CIRCLE_MARGIN = " <<BAR_CIRCLE_MARGIN;
    //qInfo() << "pos0 = " << pos0;
    //qInfo() << "pos = " << pos;
    //qInfo() << "xpos = " << xpos;
    //qInfo() << "barWidth_ = " << barWidth_;
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

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint lastPoint = event->pos();
        int x = lastPoint.x();
        int y = lastPoint.y();
        if (checkBarMouseY(y)) {
            int barNo = 0;
            int gridPos = 0;
            calculateMouseBarPos(x, &barNo, &gridPos);
            if (barNo > 0) {
                qInfo() << "barNo = " << barNo;
                qInfo() << "gridPos = " << gridPos;
                mainWindow_->openBpmDialog(barNo, gridPos);
            }
        }
    }
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
    drawBPMgraph(painter);
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

RenderArea::RenderArea(MainWindow *mainWindow)
    : QWidget(mainWindow),
      mainWindow_(mainWindow),
      playing_{false},
      metroPos_{0},
      barLeftMargin_{10},
      barWidth_{160},
      barHorizontalSpacing_{0},
      barCount_{0},
      barTopOffset_{10},
      barHeight_{100},
      gridResolution_{2},
      topBPMgraphMargin_{40},
      bpmGraphHeight_{200}
{

}


void RenderArea::setPlaying(bool value)
{
    playing_ = value;
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
