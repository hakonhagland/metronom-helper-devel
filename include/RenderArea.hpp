#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QFontMetrics>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QString>
#include <QWidget>
#include <set>
#include <string>
#include <vector>

#include "MainWindow.hpp"
#include "Pixel.hpp"

class MainWindow;

class RenderArea : public QWidget
{
    Q_OBJECT

    // fraction of bar width
    constexpr static double BAR_CIRCLE_RADIUS_SIZE = 5.0/100;
public:
    struct GraphPoint;
    class GraphPoints;
    //RenderArea(QWidget *parent = 0, QImage *image);
    RenderArea(MainWindow *parent);
    void addBar();
    int bpmMax() {return bpmMax_;}
    int bpmMin() {return bpmMin_;}
    double convertPixelToBpm(double y);
    double getBarCircleBarLeftMargin();
    int getBarCount() { return barCount_; }
    int getBarLeftMargin() {return barLeftMargin_; }
    int getBpmGraphTopOffset() { return barTopOffset_ + barHeight_ + topBPMgraphMargin_; }
    GraphPoint *getSelectedPoint() { return bpmSelectedPoint_; }
    int getMetroPos() {return metroPos_; }
    GraphPoints &getBpmGraphPoints() {return bpmGraphPoints_; }
    int getTotalHeight();
    void keyPressEvent(QKeyEvent *event);
    QSize minimumSizeHint() const override;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setPlaying(bool value);
    QSize sizeHint() const override;
    void updateMetroPos();

    struct GraphPoint {
        double x;
        double y;  // bpm value
        int bpm;
        GraphPoint() : x{0}, y{0}, bpm{0} {}
        GraphPoint(double x, double y, int bpm) :
            x{x}, y{y}, bpm{bpm} { }

        bool operator<(const GraphPoint& rhs) const {
            return x < rhs.x;
        }
    };

    class GraphPoints {
    public:
        GraphPoints() {}
        void addPoint(double x, double y, int bpm);
        void deletePoint(double x);
        double getLeftPointX();
        double getRightPointX();
        void updateLastPoint(double x);
        GraphPoint &operator[](int index);
        void print();
        auto begin() { return points_.begin(); }
        auto end() { return points_.end(); }
        auto cbegin() const { return points_.begin(); }
        auto rbegin() const { return points_.rbegin(); }
        auto cend() const { return points_.end(); }
        auto size() const { return points_.size(); }
        //auto begin() const { return points_.begin(); }
        //auto end() const { return points_.end(); }

    private:
        std::set<GraphPoint> points_;
    };

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    enum class ClickAlternative {onPoint, abovePoint, outside};

    class BpmGraph {
    public:
        BpmGraph(RenderArea &renderArea, QPainter &painter,
            GraphPoints &points);
        void drawRectangle();
        void drawGraph();
        void drawYaxisTicks(int height, QFont &font);
        void drawYaxisTick(double y);
    private:
        bool checkSelectedPoint_(double x);
        void drawCircle_(QPoint p, int radius, double x, int bpm);
        void drawCircleHelpText_(QPoint p, int radius, int bpm);
        RenderArea &renderArea_;
        QPainter &painter_;
        GraphPoints &points_;
        int ytop_;
        int xmin_;
    };

    GraphPoint addNewBpmGraphPoint(double x, int y);
    int bpmMaxValue() {return bpmMax_; }
    int bpmMinValue() {return bpmMin_; }
    int calculateBarXmin(int pos);
    void calculateMouseBarPos(int x, int *barNo, int *gridPos);
    bool checkBpmGraphMouseInside( int x, int y );
    ClickAlternative checkClickCircle(double x, int y);
    double convertBpmToPixel(int bpm);
    double discretizeBpmXcoord(int x);
    GraphPoint dragBpmGraphPoint(double x, int y);
    void drawBar(QPainter &painter, QPainterPath &path, int pos);
    void drawBarCircles(QPainter &painter, int xpos, int ypos, int barNum );
    void drawBarNumber(QPainter &painter, int xpos, int ypos, int barNum);
    void drawSingleBarCircleWithAnimation(
        QPainter &painter, double pos, double ypos,
        int radius, int barNum, int circleNum);
    void drawBPMgraph(QPainter &painter);
    void fixLastBpmGraphPoint();
    GraphPoint moveBpmGraphPoint(double x, int y);

    MainWindow *mainWindow_;
    bool playing_;
    int metroPos_;
    int barLeftMargin_;
    int barWidth_;
    int barHorizontalSpacing_;
    int barCount_;
    int barTopOffset_;
    int barHeight_;
    int gridResolution_;
    int topBPMgraphMargin_;
    int bpmGraphHeight_;
    double bpmGraphXresolution_;
    int  bpmGraphYaxisTicks_;
    int bpmGraphCirclePointRadius_;
    GraphPoints bpmGraphPoints_;
    bool bpmInDragState_;
    int bpmMin_;
    int bpmMax_;
    GraphPoint *bpmSelectedPoint_;
    double barCircleYoffset_;
    QFont barNumberFont_;
};

#endif // RENDERAREA_H
