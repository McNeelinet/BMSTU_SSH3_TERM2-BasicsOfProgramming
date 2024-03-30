#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H
#include <QTableWidget>
#include "csvreader.h"

void TableClear(QTableWidget& table);
void TableFill(QTableWidget& table, FileInfo& fi, std::string region);
void TableGetColumnData(QTableWidget& table, IOData& inputoutput, QString colMetric, int colYearNumber);

#endif // TABLEMANAGER_H
