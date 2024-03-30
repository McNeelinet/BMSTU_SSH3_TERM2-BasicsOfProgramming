#include <QPainter>
#include <QPicture>
#include "graphmanager.h"

std::vector<QPoint> ValuesToPoints(std::vector<YearMetricPair> values, GraphSettings& s)
{
    std::vector<QPoint> points;

    for (int i = 0; i <= values.size() - 1; i++)
        points.push_back(QPoint(s.X_START_POS + i * s.X_BAR_STEP, s.Y_START_POS - values[i].metric * s.Y_BAR_SCALE));

    return points;
}

void GraphSetUp(GraphSettings& s)  // , IOData& io)
{
    s.GRAPH_WIDTH = 1600;
    s.GRAPH_HEIGHT = 900;

    s.X_START_POS = 75;
    s.Y_START_POS = s.GRAPH_HEIGHT - 75;

    s.X_BAR_ELEMENTS = 5;  // Количество элементов по x. Должно зависеть от входных данных
    s.Y_BAR_ELEMENTS = 11;  // Количество элементов по y. Должно зависеть от входных данных

    s.X_BAR_STEP = (s.GRAPH_WIDTH - s.X_START_POS) / (s.X_BAR_ELEMENTS - 1);
    s.Y_BAR_STEP = s.Y_START_POS / (s.Y_BAR_ELEMENTS - 1);

    s.Y_BAR_MAX = 1;
    while (s.Y_BAR_MAX < 99)  // 99 - максимальный элемент по y. Должен зависеть от входных данных
        s.Y_BAR_MAX *= 10;

    s.Y_BAR_SCALE = s.Y_START_POS / s.Y_BAR_MAX;
}

void DrawGrid(QPainter& painter, GraphSettings& s)
{
    const int DashL = 5;

    painter.setPen(QPen(Qt::gray));
    for (int i = 1; i <= s.X_BAR_ELEMENTS - 2; i++)
        for (int CurYPos = s.Y_START_POS; CurYPos >= s.Y_START_POS - ((s.Y_BAR_ELEMENTS - 1) * s.Y_BAR_STEP) + DashL; CurYPos -= DashL*2)
            painter.drawLine(s.X_START_POS + (i * s.X_BAR_STEP), CurYPos,
                             s.X_START_POS + (i * s.X_BAR_STEP), CurYPos - DashL);

    for (int i = 1; i <= s.Y_BAR_ELEMENTS - 2; i++)
        for (int CurXPos = s.X_START_POS; CurXPos <= s.X_START_POS + ((s.X_BAR_ELEMENTS - 1) * s.X_BAR_STEP) - DashL; CurXPos += DashL*2)
            painter.drawLine(CurXPos, s.Y_START_POS - (i * s.Y_BAR_STEP),
                             CurXPos + DashL, s.Y_START_POS - (i * s.Y_BAR_STEP));
}

void DrawAxis(QPainter& painter, GraphSettings& s)
{
    painter.setPen(QPen(Qt::white));
    painter.drawLine(s.X_START_POS, s.Y_START_POS,
                     s.X_START_POS, s.Y_START_POS - ((s.Y_BAR_ELEMENTS - 1) * s.Y_BAR_STEP));
    painter.drawLine(s.X_START_POS, s.Y_START_POS,
                     s.X_START_POS + ((s.X_BAR_ELEMENTS - 1) * s.X_BAR_STEP), s.Y_START_POS);
}

void DrawLine(QPainter& painter, std::vector<QPoint> points)
{
    if (points.size() >= 2) {
        painter.setPen(QPen(Qt::green));
        painter.drawPolyline(&points[0], points.size());
    }
}

void DrawPoints(QPainter& painter, std::vector<QPoint> points)
{
    const int ellipseR = 5;

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::red));
    for (int i = 0; i <= points.size() - 1; i++)
        painter.drawEllipse(points[i].x() - ellipseR, points[i].y() - ellipseR, 2 * ellipseR, 2 * ellipseR);
}

void GraphDraw(QPicture& graph, std::vector<YearMetricPair> values)
{
    QPainter painter;
    GraphSettings s;

    GraphSetUp(s);
    std::vector<QPoint> points = ValuesToPoints(values, s);
    painter.begin(&graph);

    DrawGrid(painter, s);
    DrawAxis(painter, s);
    DrawLine(painter, points);
    DrawPoints(painter, points);

    painter.end();
}
