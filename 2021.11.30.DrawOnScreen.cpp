// 首先感谢各位热心朋友的帮助，不久前我在论坛里发帖提问过如何用Qt来写一个屏幕绘图的软件，在各位热心朋友的帮助下，我完成了这个小程序。现在将源码供出和大家一同分享，共同探讨。在此申明，这个软件只为学习Qt而写，所以并不完善，请各位多多指教。
// 首先介绍下，使用方法：
// 1、切换画笔：
// Ctrl+R：红色画笔
// Ctrl+G：绿色画笔
// Ctrl+B：蓝色画笔
// 2、形状切换：
// B：画直线
// C：画曲线
// R：画矩形
// E：画圆形
// 注意：以上键不区分大小写
// Ref: https://bbs.csdn.net/topics/390039968
--------------------------------------------------------------
// 以下是源码部分：


// 1、drawonscreen.h
#ifndef DRAWONSCREEN_H
#define DRAWONSCREEN_H

#include <QtGui/QMainWindow>
#include <QPen>

class DrawOnScreen : public QMainWindow
{
    Q_OBJECT

public:
    DrawOnScreen(QWidget *parent = 0);
    ~DrawOnScreen();

    void drawBaseLine(QPainter *painter);
    void drawCurve(QPainter *painter);
    void drawRectangle(QPainter *painter);
    void drawEllip(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    enum DrawShape
    {
        BASELINE,
        CURVE,
        RECT,
        ELLIPSE
    } drawShape;
    bool isDrawLine;
    bool isDrawCurve;
    bool isDrawRect;
    bool isDrawEllipse;
    QPixmap pixmap;
    QPixmap screenPixmap;
    QPointF startPoint;
    QPointF endPoint;
    QPen pen;
    QCursor cursor;
};

#endif // DRAWONSCREEN_H

// 2、drawonscreen.cpp
// #include <QtGui>
// #include "drawonscreen.h"

DrawOnScreen::DrawOnScreen(QWidget *parent)
    : QMainWindow(parent)
{
    drawShape = BASELINE;
    pen = QPen(Qt::green, 5);
    cursor = QCursor(QPixmap(":/sunsong/images/green.png"));
    setCursor(cursor);
    isDrawLine = false;
    isDrawCurve = false;
    isDrawRect = false;
    isDrawEllipse = false;
    showFullScreen();
}

DrawOnScreen::~DrawOnScreen()
{
}

void DrawOnScreen::paintEvent(QPaintEvent *event)
{
    pixmap = pixmap.grabWindow(QApplication::desktop()->winId(), 0, 0, -1, -1);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setPen(pen);

    switch (drawShape)
    {
    case BASELINE:
        drawBaseLine(&painter);
        break;
    case CURVE:
        drawCurve(&painter);
        break;
    case RECT:
        drawRectangle(&painter);
        break;
    case ELLIPSE:
        drawEllip(&painter);
        break;
    default:
        break;
    }

    QPainter p(this);
    p.drawPixmap(rect(), pixmap);
    isDrawLine = false;
    isDrawCurve = false;
    isDrawRect = false;
    isDrawEllipse = false;
}

void DrawOnScreen::resizeEvent(QResizeEvent *event)
{
    pixmap = QPixmap(size());
    screenPixmap = QPixmap(size());
}

void DrawOnScreen::drawBaseLine(QPainter *painter)
{
    if (isDrawLine)
    {
        painter->drawPixmap(rect(), screenPixmap);
        painter->drawLine(startPoint, endPoint);
    }
}

void DrawOnScreen::drawCurve(QPainter *painter)
{
    if (isDrawCurve)
        painter->drawLine(startPoint, endPoint);
}

void DrawOnScreen::drawRectangle(QPainter *painter)
{
    if (isDrawRect)
    {
        QRectF rectf = QRectF(startPoint, endPoint);
        painter->drawPixmap(rect(), screenPixmap);
        painter->drawRect(rectf);
    }
}

void DrawOnScreen::drawEllip(QPainter *painter)
{
    if (isDrawEllipse)
    {
        QRectF rectf = QRectF(startPoint, endPoint);
        painter->drawPixmap(rect(), screenPixmap);
        painter->drawEllipse(rectf);
    }
}

void DrawOnScreen::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        startPoint = event->posF();
        screenPixmap = pixmap;
    }
}

void DrawOnScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        endPoint = event->posF();

        switch (drawShape)
        {
        case BASELINE:
            isDrawLine = true;
            repaint();
            break;
        case CURVE:
            isDrawCurve = true;
            repaint();
            startPoint = endPoint;
            break;
        case RECT:
            isDrawRect = true;
            repaint();
            break;
        case ELLIPSE:
            isDrawEllipse = true;
            repaint();
            break;
        default:
            break;
        }
    }
}

void DrawOnScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        endPoint = event->posF();
        isDrawLine = true;
        isDrawCurve = true;
        isDrawRect = true;
        isDrawEllipse = true;
        repaint();
    }
}

void DrawOnScreen::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_B:
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            pen.setColor(Qt::blue);
            cursor = QCursor(QPixmap(":/sunsong/images/blue.png"));
            setCursor(cursor);
        }
        else
            drawShape = BASELINE;
    }
    break;
    case Qt::Key_C:
        drawShape = CURVE;
        break;
    case Qt::Key_R:
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            pen.setColor(Qt::red);
            cursor = QCursor(QPixmap(":/sunsong/images/red.png"));
            setCursor(cursor);
        }
        else
            drawShape = RECT;
    }
    break;
    case Qt::Key_E:
        drawShape = ELLIPSE;
        break;
    case Qt::Key_G:
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            pen.setColor(Qt::green);
            cursor = QCursor(QPixmap(":/sunsong/images/green.png"));
            setCursor(cursor);
        }
    }
    break;
    default:
        break;
    }
}

// 3、main.cpp
// #include <QtGui/QApplication>
// #include "drawonscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DrawOnScreen w;
    w.show();
    return a.exec();
}
