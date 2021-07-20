#include "BpmCalculator.hpp"
#include "RenderArea.hpp"
#include "Pixel.hpp"

BpmCalculator::BpmCalculator(MainWindow *mainWindow) : mainWindow_(mainWindow)
{

}

double BpmCalculator::calculateLastPoint()
{
    RenderArea *renderArea = mainWindow_->getRenderArea();
    RenderArea::GraphPoints &points = renderArea->getBpmGraphPoints();
    return points.rbegin()->bpm;
}

double BpmCalculator::calculate(int metroPos)
{
    RenderArea *renderArea = mainWindow_->getRenderArea();
    //int bpmGraphTopOffset = renderArea->getBpmGraphTopOffset();
    double offset2 = renderArea->getBarCircleBarLeftMargin();
    double deltaX = offset2 * 2;
    double barLeftMargin = renderArea->getBarLeftMargin();
    double x1 = metroPos * deltaX;
    RenderArea::GraphPoints &points = renderArea->getBpmGraphPoints();
    double xsPrev = 0;
    double xl1 = 0;
    double bl1 = points.begin()->bpm;
    double bsPrev = bl1;
    double br1 = points.rbegin()->bpm;
    double xr1 = points.rbegin()->x - (barLeftMargin + offset2);
    auto last_point = std::prev(points.end());
    for (auto itr = std::next(points.begin()); itr != last_point; itr++) {
        auto point = *itr;
        double xs = point.x - (barLeftMargin + offset2);
        double bs = point.bpm;
        if ( pixel::compare_greater(xs, x1) ) {
            // xs = x-coordinate of the sub point
            // x1 = x-coordinate of metroPos
            // Found the sub points closest to the metropos click circle
            xl1 = xsPrev;  //
            xr1 = xs;
            bl1 = bsPrev;
            br1 = bs;
            break;
        }
        else if (itr == std::prev(last_point) ) {
            xl1 = xs;  //
            auto point = *last_point;
            xr1 = point.x - (barLeftMargin + offset2);
            br1 = point.bpm;
            bl1 = bs;
            break;
        }
        xsPrev = xs;
        bsPrev = bs;
    }
    double F = (x1 - xl1) / (xr1 - xl1);
    double b1 = F * ( br1 - bl1 ) + bl1;
    return b1;
}
