#include <QHeaderView>
#include "tablemanager.h"

void TableClear(QTableWidget& table)
{
    table.setColumnCount(0);
    table.setRowCount(0);
    table.horizontalHeader()->hide();
}

void TableFill(QTableWidget& table, FileInfo& fi, std::string region)
{
    IOData io;

    CSVHelperFrontController(READER_RESET, &fi);
    CSVHelperFrontController(READER_GETROW, &fi, &io);

    table.setColumnCount(fi.columns);
    for (int i = 0; i <= fi.columns - 1; i++)
        table.setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(i), 0));
    table.horizontalHeader()->show();

    while (CSVHelperFrontController(READER_GETROW, &fi, &io)) {
        if (io.rowValues[fi.colRegNumber] != region)
            continue;

        table.insertRow(table.rowCount());
        for (int i = 0; i < fi.columns; i++)
            table.setItem(table.rowCount() - 1, i, new QTableWidgetItem(QString::fromStdString(io.rowValues[i])));
    }
}

void TableGetColumnData(QTableWidget& table, IOData& inputoutput, QString colMetric, int colYearNumber)
{
    bool isInt;
    bool isDouble;

    int colMetricNumber = colMetric.toInt(&isInt);
    if (!isInt)
        throw std::runtime_error("Номер колонки - не число");

    int columnCount = table.columnCount();
    if (!(colMetricNumber >= 0 && colMetricNumber < columnCount))
        throw std::runtime_error("Выход за пределы диапазона");
    int rowCount = table.rowCount();

    for (int i = 0; i <= rowCount - 1; i++) {
        double metric = table.item(i, colMetricNumber)->text().toDouble(&isDouble);
        int year = table.item(i, colYearNumber)->text().toInt(&isInt);

        YearMetricPair pair = {.year = year, .metric = metric};
        if (isInt)
            pair.hasyear = true;
        if (isDouble)
            pair.hasmetric = true;

        inputoutput.metricData.push_back(pair);
    }
}
