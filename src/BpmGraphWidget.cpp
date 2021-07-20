#include <QDebug>
#include <QPainterPath>
#include <QFont>
#include <QFontMetrics>
#include "BpmGraphWidget.hpp"

BpmGraphWidget::BpmGraphWidget(QWidget *parent, int width) : QWidget(parent) {
    widgetWidth_ = width;
}

void TestWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
}
