#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "csvreader.h"


//переписать?//
int findColumnsCount(std::string line)
{
    return std::count(line.cbegin(), line.cend(), ',') + 1;
}

int findColumnByPos(std::string line, size_t pos)
{
    return std::count(line.cbegin(), line.cbegin() + pos, ',');
}

int findColumn(std::string line, const char* what)
{
    int number;

    size_t pos = line.find(what);
    if (pos == std::string::npos)
        throw std::runtime_error("Нет нужной колонки");

    number = findColumnByPos(line, pos);
    return number;
}

std::string trimString(std::string string)
{
    std::string::iterator start = string.begin();
    std::string::iterator end = string.end();

    while (start != end && std::isspace(*start))
        start++;

    do
        end--;
    while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

void sortValues(std::vector<YearMetricPair>& vector, int size)
{
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (vector[j].metric > vector[j + 1].metric) {
                YearMetricPair temp = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = temp;
            }
}
////

bool CSVReaderEnable(FileInfo& fi)
{
    fi.file.open(fi.filename);
    if (!fi.file.is_open())
        throw std::runtime_error("Не удалось открыть файл.");

    std::string line;
    if (!std::getline(fi.file, line))
        throw std::runtime_error("Не удалось прочитать файл.");

    fi.columns = findColumnsCount(line);
    fi.colRegNumber = findColumn(line, "region");
    fi.colYearNumber = findColumn(line, "year");

    fi.file.seekg(0);
    return true;
}

bool CSVReaderReset(FileInfo& fi)
{
    fi.file.seekg(0);
    return true;
}

bool CSVReaderDisable(FileInfo& fi)
{
    if (fi.file.is_open())
        fi.file.close();
    return true;
}

bool CSVReaderGetRow(FileInfo& fi, IOData& io)
{
    if (!fi.file.is_open())
        throw std::runtime_error("Файл не был открыт");

    std::string line;
    if (std::getline(fi.file, line)) {
        if (findColumnsCount(line) != fi.columns)
            throw std::runtime_error("Поврежденный файл");

        std::stringstream lineStream(line);
        std::vector<std::string> lineArray(fi.columns);
        for (int i = 0; i <= fi.columns - 1; i++) {
            std::string temp;
            std::getline(lineStream, temp, ',');
            temp = trimString(temp);
            lineArray[i] = temp;
        }

        io.rowValues = lineArray;
        return true;
    }

    return false;
}

bool MetricsFilter(IOData& io)
{
    std::vector<YearMetricPair> filtered;
    for (int i = 0; i <= io.metricData.size() - 1; i++)
        if (io.metricData[i].hasmetric && io.metricData[i].hasyear)
            filtered.push_back(io.metricData[i]);

    io.metricData = filtered;
    return io.metricData.empty();
}

bool MetricsCalcMaximum(IOData& io)
{
    std::vector<YearMetricPair>& values = io.metricData;
    double& result = io.metricResult;

    if (!values.empty()) {
        result = values[0].metric;

        int size = values.size();
        for (int i = 0; i <= size - 1; i++)
            if (values[i].metric > result)
                result = values[i].metric;

        return true;
    }

    return false;
}

bool MetricsCalcMinimum(IOData& io)
{
    std::vector<YearMetricPair>& values = io.metricData;
    double& result = io.metricResult;

    if (!values.empty()) {
        result = values[0].metric;

        int size = values.size();
        for (int i = 0; i <= size - 1; i++)
            if (values[i].metric < result)
                result = values[i].metric;

        return true;
    }

    return false;
}

bool MetricsCalcMedian(IOData& io)
{
    std::vector<YearMetricPair>& values = io.metricData;
    double& result = io.metricResult;

    if (!values.empty()) {
        result = values[0].metric;
        sortValues(values, values.size());

        if (values.size() % 2 == 1)
            result = values[values.size() / 2].metric;
        else
            result = (values[values.size() / 2 - 1].metric + values[values.size() / 2].metric) / 2;

        return true;
    }

    return false;
}
////

bool CSVHelperFrontController(int action, FileInfo* fi, IOData* io)
{
    bool status = false;

    if (action == READER_ENABLE && !(fi == nullptr))
        status = CSVReaderEnable(*fi);
    else if (action == READER_RESET && !(fi == nullptr))
        status = CSVReaderReset(*fi);
    else if (action == READER_DISABLE && !(fi == nullptr))
        status = CSVReaderDisable(*fi);
    else if (action == READER_GETROW && !(fi == nullptr) && !(io == nullptr))
        status = CSVReaderGetRow(*fi, *io);
    else if (action == METRICS_MAXIMUM && !(io == nullptr))
        status = MetricsCalcMaximum(*io);
    else if (action == METRICS_MINIMUM && !(io == nullptr))
        status = MetricsCalcMinimum(*io);
    else if (action == METRICS_MEDIAN && !(io == nullptr))
        status = MetricsCalcMedian(*io);
    else if (action == METRICS_FILTER && !(io == nullptr))
        status = MetricsFilter(*io);
    else
        throw std::runtime_error("Не удается выполнить операцию.");

    return status;
}
