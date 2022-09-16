#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void setjulianDay(int, int, int, int, int, int, int, double, double);

    void setDateTime_Now();

    void setDateTime_Zero();

    void on_clearButto_clicked();

    void on_calcButton_clicked();

    void on_monthCheckBox_clicked();

    void on_nowButton_clicked();

    void on_zeroButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
