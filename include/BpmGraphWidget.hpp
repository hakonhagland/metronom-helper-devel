#ifndef BPM_GRAPH_WIDGET_H
#define BPM_GRAPH_WIDGET_H

#include <QPainter>
#include <QPen>
#include <QString>
#include <QWidget>


class BpmGraphWidget : public QWidget
{
    Q_OBJECT

public:
    BpmGraphWidget(QWidget *parent, int width);
    ~BpmGraphWidget(){}

public slots:

private:
    int widgetWidth_;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
};

#endif // BPM_GRAPH_WIDGET_H
