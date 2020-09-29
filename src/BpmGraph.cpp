#include "RenderArea.hpp"

RenderArea::BpmGraph::BpmGraph(RenderArea &renderArea, QPainter &painter) :
    renderArea_{renderArea}, painter_{painter}
{

}

void RenderArea::BpmGraph::drawGraph()
{
    std::vector<GraphPoints> points;
    
}

void RenderArea::BpmGraph::drawRectangle()
{
    int ytop = renderArea_.barTopOffset_ + renderArea_.barHeight_
        + renderArea_.topBPMgraphMargin_;
    int height = renderArea_.bpmGraphHeight_;
    int xmin = renderArea_.barLeftMargin_;
    int width = renderArea_.barCount_ * renderArea_.barWidth_;
    int ybottom = ytop + height;
    int xmax = xmin + width;
    QColor AXES_COLOR{0, 0, 200};
    QPen pen {AXES_COLOR, 2};
    //pen.setJoinStyle(Qt::DashLine);
    //pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    painter_.setPen(pen);
    painter_.drawLine(xmin, ybottom, xmax, ybottom);
    painter_.drawLine(xmin, ytop, xmax, ytop);
    painter_.drawLine(xmin, ytop, xmin, ybottom);
    painter_.drawLine(xmax, ytop, xmax, ybottom);
}
