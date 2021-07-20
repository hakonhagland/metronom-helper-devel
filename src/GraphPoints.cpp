#include "RenderArea.hpp"

void RenderArea::GraphPoints::addPoint(double x, double y, int bpm)
{
    points_.insert({x, y, bpm});
}

void RenderArea::GraphPoints::deletePoint(double x)
{
    auto itr = points_.find({x,0,0});
    if (itr != points_.end()) {
        points_.erase(itr);
    }
}

double RenderArea::GraphPoints::getLeftPointX()
{
    auto itr = points_.begin();
    assert( itr != points_.end());
    return itr->x;
}

double RenderArea::GraphPoints::getRightPointX()
{
    auto itr = points_.rbegin();
    assert( itr != points_.rend());
    return itr->x;
}

void RenderArea::GraphPoints::updateLastPoint(double x)
{
    if (!points_.empty()) {
        auto itr = points_.end();
        itr--;
        const GraphPoint &pc = *itr;
        GraphPoint &p = const_cast<GraphPoint &>(pc);
        p.x = x;
    }
}

void RenderArea::GraphPoints::print()
{
    for (auto p : points_) {
        qInfo() << "  (" << p.x << ", " << p.y << ")";
    }
}


/*
RenderArea::GraphPoint &RenderArea::GraphPoints::operator[](int index)
{
    return points_[index];
}
*/
