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
    int calculateBarXmin(int pos);
    void calculateMouseBarPos(int x, int *barNo, int *gridPos);
    bool checkBarMouseY( int y );
    void drawBar(QPainter &painter, QPainterPath &path, int pos);
    void drawBarCircles(QPainter &painter, int xpos, int ypos, int barNum );
    void drawGrid(QPainter &painter, int xpos, int ypos, int width, int height);
    void drawSingleBarCircleWithAnimation(
        QPainter &painter, int pos, int ypos, int radius, int barNum, int circleNum);
    void drawBPMgraph(QPainter &painter);
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

    class BpmGraph {
    public:
        BpmGraph(RenderArea &renderArea, QPainter &painter);
        void drawRectangle();
    private:
        RenderArea &renderArea_;
        QPainter &painter_;
    };
};

#endif // RENDERAREA_H
