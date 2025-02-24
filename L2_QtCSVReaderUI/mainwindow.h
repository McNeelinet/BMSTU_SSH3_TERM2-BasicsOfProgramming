#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPicture>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFileDialog();
    void loadData();
    void calcMetrics();
    void clearMetrics();

private:
    int colRegNumber;
    int colYearNumber;
    QPicture graph;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
