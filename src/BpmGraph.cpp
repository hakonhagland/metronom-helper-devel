#include "RenderArea.hpp"
#include "Pixel.hpp"
#include <iterator>

/* Public methods in alphabetical order */

RenderArea::BpmGraph::BpmGraph(RenderArea &renderArea, QPainter &painter,
                               GraphPoints &points) :
    renderArea_{renderArea}, painter_{painter}, points_{points}
{
    ytop_ = renderArea_.barTopOffset_ + renderArea_.barHeight_
        + renderArea_.topBPMgraphMargin_;
    xmin_ = renderArea_.barLeftMargin_;
}

void RenderArea::BpmGraph::drawGraph()
{
    QColor BPM_GRAPH_COLOR{255, 0, 0};
    QPen pen {BPM_GRAPH_COLOR, 2};
    //pen.setJoinStyle(Qt::DashLine);
    //pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    //points_.print();
    int idx = 0;
    for (auto itr = points_.cbegin(); itr != std::prev(points_.cend()); itr++) {
        const auto &point1 = *itr;
        const auto &point2 = *std::next(itr);

        //QPoint p1 {getScrolledX(point1), pixel::round(point1.y)};
        QPoint p1 {pixel::round(point1.x), pixel::round(point1.y)};
        //QPoint p2 {getScrolledX(point2), pixel::round(point2.y)};
        QPoint p2 {pixel::round(point2.x), pixel::round(point2.y)};
        this->painter_.setPen(pen);
        painter_.drawLine(p1, p2);
        int radius = renderArea_.bpmGraphCirclePointRadius_;
        //checkSelectedPoint(
        drawCircle_(p1, radius, point1.x, point1.bpm);
        if ((points_.size() == 2) || (idx > 0)) {
            drawCircle_(p2, radius, point2.x, point2.bpm);
        }
        idx++;
    }
}

/*
int RenderArea::BpmGraph::getScrolledX(GraphPoint &p)
{
    pixel::round(p.x-delta);

}
*/

void RenderArea::BpmGraph::drawRectangle()
{
    int height = renderArea_.bpmGraphHeight_;
    int width = renderArea_.barCount_ * renderArea_.barWidth_;
    int ybottom = this->ytop_ + height;
    int xmax = this->xmin_ + width;
    QColor AXES_COLOR{0, 0, 200};
    QPen pen {AXES_COLOR, 2};
    //pen.setJoinStyle(Qt::DashLine);
    //pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    painter_.setPen(pen);
    painter_.drawLine(this->xmin_, ybottom, xmax, ybottom);
    painter_.drawLine(this->xmin_, ytop_, xmax, this->ytop_);
    painter_.drawLine(this->xmin_, ytop_, this->xmin_, ybottom);
    painter_.drawLine(xmax, ytop_, xmax, ybottom);
    QFont font{"Arial", 10, QFont::Normal};
    painter_.setFont(font);
    drawYaxisTicks(height, font);
}

void RenderArea::BpmGraph::drawYaxisTicks(int height, QFont &font)
{
    int N = this->renderArea_.bpmGraphYaxisTicks_;
    int max = this->renderArea_.bpmMax();
    int min = this->renderArea_.bpmMin();
    int x1 = 0;
    double delta_pixel = height/(N-1);
    double bpm = max;
    double y = 0;
    double delta_bpm = ((double)(max-min))/(N-1);
    QFontMetrics fm{font};
    for (int i = 0; i < N; i++) {
        std::string str = std::to_string(pixel::round(bpm));
        QString txt {str.c_str()};
        QRect rect = fm.tightBoundingRect(txt);
        QColor AXES_TEXT_COLOR{0, 0, 0};
        QPen text_pen {AXES_TEXT_COLOR, 2};
        painter_.setPen(text_pen);
        this->painter_.drawText(
            x1, pixel::round(this->ytop_+y+(rect.y()/2.0)),
            rect.width()+1, rect.height()+1, Qt::AlignVCenter, txt);
        if ((i > 0) && (i < (N-1))) {
            drawYaxisTick(y);
        }
        y = y + delta_pixel;
        bpm = bpm - delta_bpm;
    }
}

void RenderArea::BpmGraph::drawYaxisTick(double y)
{
    QColor TICK_COLOR{0, 0, 200};
    QPen pen {TICK_COLOR, 2};
    //pen.setJoinStyle(Qt::DashLine);
    //pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    this->painter_.setPen(pen);
    int tick_length = 4;
    int ypixel = pixel::round(this->ytop_+y);
    painter_.drawLine( this->xmin_, ypixel, this->xmin_+tick_length, ypixel);
}

/* Private methods in alphabetical order */

bool RenderArea::BpmGraph::checkSelectedPoint_(double x)
{
    GraphPoint *point = this->renderArea_.getSelectedPoint();
    if (point == nullptr) {
        return false;
    }
    else {
        return x == point->x;
    }
}

void RenderArea::BpmGraph::drawCircle_(QPoint p, int radius, double x, int bpm)
{
    if (checkSelectedPoint_(x)) {
        this->painter_.setBrush(Qt::blue);
    }
    else {
        this->painter_.setBrush(Qt::green);
    }
    this->painter_.drawEllipse(p, radius, radius);
    drawCircleHelpText_(p, radius, bpm);
}

void RenderArea::BpmGraph::drawCircleHelpText_(QPoint p, int radius, int bpm)
{
    int deltaX = 3;
    int deltaY = 0;
    int x = p.x() + radius + deltaX;
    int y = p.y() + radius + deltaY;
    std::string str = std::to_string(bpm);
    QString txt {str.c_str()};
    QColor TEXT_COLOR{0, 0, 0};
    QPen text_pen {TEXT_COLOR, 2};
    this->painter_.setPen(text_pen);
    this->painter_.drawText(x, y, txt);
}

/*
void RenderArea::BpmGraph::drawCircleHelpText_(QPoint p, int radius, int bpm)
{
    int deltaX = 3;
    int deltaY = 0;
    int x = p.x() + radius + deltaX;
    int y = p.y() + radius + deltaY;
    int height = this->renderArea_.bpmGraphHeight_;
    int ypos = p.y() - this->ytop_;
    int maxBpm = this->renderArea_.bpmMax();
    int minBpm = this->renderArea_.bpmMin();
    double fraction = ((double) ypos/height);
    double bpm = (maxBpm - minBpm)*(1-fraction) + minBpm;
    std::string str = std::to_string(pixel::round(bpm));
    QString txt {str.c_str()};
    QColor TEXT_COLOR{0, 0, 0};
    QPen text_pen {TEXT_COLOR, 2};
    this->painter_.setPen(text_pen);
    this->painter_.drawText(x, y, txt);
}
*/
