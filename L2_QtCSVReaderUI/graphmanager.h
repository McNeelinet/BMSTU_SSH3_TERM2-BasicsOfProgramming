#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H
#include <QPicture>

struct YearMetricPair
{
    int year;
    double metric;
};

struct GraphSettings {
    int GRAPH_WIDTH;
    int GRAPH_HEIGHT;

    int X_START_POS;
    int Y_START_POS;

    int X_BAR_ELEMENTS;
    int Y_BAR_ELEMENTS;

    int X_BAR_STEP;
    int Y_BAR_STEP;

    int X_BAR_MAX;
    int Y_BAR_MAX;

    int X_BAR_SCALE;
    int Y_BAR_SCALE;
};

void GraphDraw(QPicture& graph);

#endif // GRAPHMANAGER_H
