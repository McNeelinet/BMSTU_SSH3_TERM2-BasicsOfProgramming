#ifndef CSVREADER_H
#define CSVREADER_H
#include <fstream>
#include <vector>

struct FileInfo
{
    std::string filename;
    std::ifstream file;
    int columns;
    int colRegNumber;
    int colYearNumber;
};

struct YearMetricPair
{
    int year;
    double metric;

    bool hasyear = false;
    bool hasmetric = false;
};

struct IOData
{
    std::vector<std::string> rowValues;
    std::vector<YearMetricPair> metricData;
    double metricResult;
};

enum ACTIONS {
    METRICS_MAXIMUM,
    METRICS_MINIMUM,
    METRICS_MEDIAN,
    METRICS_FILTER,
    READER_ENABLE,
    READER_RESET,
    READER_DISABLE,
    READER_GETROW
};

bool CSVHelperFrontController(int action, FileInfo* fi = nullptr, IOData* io = nullptr);

#endif // CSVREADER_H
