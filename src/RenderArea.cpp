#include "RenderArea.hpp"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QPainterPath>

#include "MainWindow.hpp"
#include "Pixel.hpp"

void RenderArea::addBar()
{
    barCount_ += 1;
    if (barCount_ == 1) {
        int width = barWidth_;
        int y1 = this->getBpmGraphTopOffset();
        int bpm = mainWindow_->getCurrentBpm();
        double bpm_ypos = convertBpmToPixel(bpm);
        int y = y1 + bpm_ypos;
        double pos0 = getBarCircleBarLeftMargin();
        bpmGraphPoints_.addPoint(barLeftMargin_+pos0, y, bpm);
        bpmGraphPoints_.addPoint(barLeftMargin_+width-pos0, y, bpm);
    }
    else {
        fixLastBpmGraphPoint();
    }
}

RenderArea::GraphPoint RenderArea::addNewBpmGraphPoint(
        [[maybe_unused]]double x, [[maybe_unused]] int y)
{
    double bpm = convertPixelToBpm(y);
    bpmGraphPoints_.addPoint(x, y, pixel::round(bpm));
    update();
    return {0, 0, 0};
}

int RenderArea::calculateBarXmin(int pos)
{
    return pos*(barWidth_ + barHorizontalSpacing_) + barLeftMargin_;
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

int RenderArea::getTotalHeight()
{
    return barTopOffset_ + barHeight_ + topBPMgraphMargin_ + bpmGraphHeight_;
}

// Check if mouse click was inside the blue area, and also x-coordinate
// is inside the valid click area.
bool RenderArea::checkBpmGraphMouseInside(int x, int y) {
    int y1 = barTopOffset_ + barHeight_ + topBPMgraphMargin_;
    int y2 = y1 + bpmGraphHeight_;
    double pos0 = getBarCircleBarLeftMargin();
    double x1 = barLeftMargin_ + pos0;
    int width = barCount_ * barWidth_;
    double x2 = barLeftMargin_ + width - pos0;
    return x >= x1 && x <= x2 && y >= y1 && y <= y2;
}

// Determine if mouse click coordinate (where x is discretized to valid grid points)
//     is either:
//  - on an existing click point, or
//  - above or below an existing click point, or
//  - to the left of or to the right of an existing click point.
RenderArea::ClickAlternative RenderArea::checkClickCircle(double x, int y)
{
    auto radius = bpmGraphCirclePointRadius_;
    // assume bpmGraphPoints_ are already sorted
    for(auto& existingPoint: bpmGraphPoints_) {
        if (pixel::compare(x, existingPoint.x)) {
            auto oldPoint = bpmSelectedPoint_;
            bpmSelectedPoint_ = &const_cast<GraphPoint &>(existingPoint);
            if ((y > existingPoint.y - radius) &&
                (y < existingPoint.y + radius)) {
                if (oldPoint != nullptr) {
                    if (pixel::compare(x, oldPoint->x)) {
                        bpmSelectedPoint_ = nullptr;
                    }
                }
                return ClickAlternative::onPoint;
            }
            else {
                bpmSelectedPoint_->y = y;
                bpmSelectedPoint_->bpm = pixel::round(convertPixelToBpm(y));
                return ClickAlternative::abovePoint;
            }
        }
    }
    return ClickAlternative::outside;
}

double RenderArea::convertBpmToPixel(int bpm)
{
    int maxBpm = this->bpmMax_;
    int minBpm = this->bpmMin_;
    int h = bpmGraphHeight_;
    double fraction = ((double) (bpm - minBpm) / (maxBpm - minBpm));
    double y = h*(1-fraction);
    return y;
}

double RenderArea::convertPixelToBpm(double y)
{
    y = y - getBpmGraphTopOffset();
    int maxBpm = this->bpmMax_;
    int minBpm = this->bpmMin_;
    int h = bpmGraphHeight_;
    double fraction = y / h;
    double bpm = (maxBpm - minBpm)*(1-fraction) + minBpm;
    return bpm;
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
    drawBarNumber(painter, xpos, ypos, pos);
}

void RenderArea::drawBarNumber(QPainter &painter, int xpos, int ypos, int barNum)
{
    std::string str = std::to_string(barNum+1);
    QFont font{"Arial", 10, QFont::Normal};
    QString txt {str.c_str()};
    QFontMetrics fm{barNumberFont_};
    QRect rect = fm.tightBoundingRect(txt);
    QColor TEXT_COLOR{0, 0, 255};
    QPen text_pen {TEXT_COLOR, 2};
    painter.setPen(text_pen);
    double x = xpos + barWidth_/2.0 - rect.width()/2.0;
    double y = ypos + barHeight_ - barHeight_/7.0 - barCircleYoffset_;
    painter.drawText(x, y, txt);
}

void RenderArea::drawBPMgraph(QPainter &painter)
{
    if (barCount_ < 1) {
        return;
    }

    BpmGraph graph { *this, painter, bpmGraphPoints_ };
    //QRect clip {40, 50, 400, 300};
    //painter.setClipRect(clip);
    graph.drawRectangle();
    graph.drawGraph();

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
    double pos0 = getBarCircleBarLeftMargin();
    double delta = pos0 * 2;
    double pos = xpos + pos0;
    for (int i = 1; i <= numCircles; i++) {
        drawSingleBarCircleWithAnimation(painter, pos, ypos, radius, barNum, i);
        pos += delta;
    }
}

RenderArea::GraphPoint RenderArea::dragBpmGraphPoint(
       [[maybe_unused]] double x, [[maybe_unused]] int y)
{
    bpmInDragState_ = true;
    update();
    return {0, 0, 0};
}

void RenderArea::drawSingleBarCircleWithAnimation(
    QPainter &painter, double xpos, double bar_ypos, int radius, int barNum, int circleNum )
{
    double ypos = bar_ypos+barHeight_/2 - barCircleYoffset_;
    int numCircles = mainWindow_->getTimeSignature().getTop();
    int currentPos = barNum*numCircles + circleNum;
    if (metroPos_ > 0) {
        if ( currentPos == metroPos_) {
            double radiusDelta = mainWindow_->getBeatCircleTimer()->getRadiusDelta();
            radius += radiusDelta;
        }
    }
    painter.drawEllipse(QPoint(pixel::round(xpos), pixel::round(ypos)),
        pixel::round(radius), pixel::round(radius));

}

void RenderArea::fixLastBpmGraphPoint()
{
    double pos0 = getBarCircleBarLeftMargin();
    int width = barCount_ * barWidth_;
    bpmGraphPoints_.updateLastPoint((double) (barLeftMargin_+width-pos0));
}


// Convert mouse click x-coordinate to xx
// - where xx is normalized to the bpmGraph area and restricted to valid
//  x positions.
//
// INPUT:
//  - x : pixel x-coordinate of mouse press event
//
double RenderArea::discretizeBpmXcoord(int x)
{
    double xpos2 = getBarCircleBarLeftMargin();
    double pos0 = xpos2 + barLeftMargin_;
    double delta = xpos2 * 2;  // distance between two bar circles
    // width = pixels (in floating point resolution) between two valid click
    //   positions.
    double width = delta / bpmGraphXresolution_;
    double xx = ((double) x) - pos0;
    // Round to closest valid position
    int f = pixel::round(xx / width);
    double xpos = f * width;
    return xpos + pos0;
}

// This distance is currently = 26
// The distance from the (centre of the) left edge of the bar box to
//   the center of the first bar circle.
double RenderArea::getBarCircleBarLeftMargin()
{
    int numCircles = mainWindow_->getTimeSignature().getTop();
    double delta = ((double) barWidth_) / numCircles;
    return delta/2;
}

void RenderArea::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        if (bpmSelectedPoint_) {
            auto xleft = bpmGraphPoints_.getLeftPointX();
            auto xright = bpmGraphPoints_.getRightPointX();
            if(!(pixel::compare(bpmSelectedPoint_->x, xleft)) &&
                !(pixel::compare(bpmSelectedPoint_->x, xright))) {
                bpmGraphPoints_.deletePoint(bpmSelectedPoint_->x);
                bpmSelectedPoint_ = nullptr;
                update();
            }
        }
    }
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    QPoint lastPoint = event->pos();
    int x = lastPoint.x();
    int y = lastPoint.y();
    if (checkBpmGraphMouseInside(x, y)) {
        if (bpmInDragState_) {
            if (bpmSelectedPoint_ != nullptr) {
                bpmSelectedPoint_->bpm = pixel::round(convertPixelToBpm(y));
                bpmSelectedPoint_->y = y;
                update();
            }
        }
    }
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint lastPoint = event->pos();
        int x = lastPoint.x();
        int y = lastPoint.y();
        if (checkBpmGraphMouseInside(x,y)) {
            double xx = discretizeBpmXcoord(x);
            ClickAlternative alt = checkClickCircle(xx, y);
            GraphPoint point;
            switch (alt) {
            case ClickAlternative::onPoint:
                point = dragBpmGraphPoint(xx, y);
                break;
            case ClickAlternative::abovePoint:
                point = moveBpmGraphPoint(xx, y);
                break;
            case ClickAlternative::outside:
                point = addNewBpmGraphPoint(xx, y);
                break;
            default:
                throw std::runtime_error("Unexpected click alternative");
            }
        }
    }
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint lastPoint = event->pos();
        int x = lastPoint.x();
        int y = lastPoint.y();
        bpmInDragState_ = false;
        if (checkBpmGraphMouseInside(x,y)) {
            qInfo() << "mouse released";
        }
    }
}

// We now know that bpmSelectedPoint_ has been updated in
//    checkClickCircle(point) called from mousePressEvent() so we only need
//   call update()
RenderArea::GraphPoint RenderArea::moveBpmGraphPoint(
        [[maybe_unused]] double xx, [[maybe_unused]] int y)
{
    update();
    return {0, 0, 0};
}

void RenderArea::paintEvent([[maybe_unused]] QPaintEvent *event)
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
      barLeftMargin_{28},
      barWidth_{160},
      barHorizontalSpacing_{0},
      barCount_{0},
      barTopOffset_{10},
      barHeight_{100},
      gridResolution_{2},
      topBPMgraphMargin_{40},
      bpmGraphHeight_{200},
      bpmGraphXresolution_{5},
      bpmGraphYaxisTicks_{6},
      bpmGraphCirclePointRadius_{7},
      bpmGraphPoints_{},
      bpmInDragState_{false},
      bpmMin_{1},
      bpmMax_{220},
      bpmSelectedPoint_{nullptr},
      barCircleYoffset_{8.0}
{
    bpmGraphHeight_ = bpmMax_ - bpmMin_;
    setFocusPolicy(Qt::StrongFocus);
    double pos0 = getBarCircleBarLeftMargin();
    double delta = pos0*2;
    double w = delta;
    double D = 2*bpmGraphCirclePointRadius_;
    double gamma = (w+D)/D;
    int gamma_min = (int) gamma;
    //double D_prime = (w+D)/gamma_min;
    bpmGraphXresolution_ = gamma_min;
    barNumberFont_ = QFont{"Arial", 16, QFont::Normal};
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
