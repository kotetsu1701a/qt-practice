#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

double getJulianDay(int year, int month, int day)
// マイコン天文学Ⅰ（恒星社厚生閣 1983 中野主一著）
// 20-21頁 式 2-1-1,2-1-2,2-1-3
{
    double julian = 0.0;
    double branch;
    int sgn;

    branch = year + (month - 1.0) / 12.0 + day / 365.25;

    if (month < 3.0) {
        year -= 1;
        month += 12;
    }

    sgn = (year > 0.0) - (year < 0);
    if (branch >= 1582.78) {
        julian = floor(year * 365.25) + floor(year / 400.0) - floor(year / 100.0) + floor(30.59 * (month - 2.0)) + day + 1721088.5;
     } else {
        if (branch >= 0.0) {
            julian = floor(year * 365.25) + floor(30.59 * (month - 2.0)) + day + 1721086.5;
        } else {
            if (year < 0.0) {
                julian = sgn * floor(abs(year) * 365.25) + floor(30.59 * (month - 2.0)) + day + 1721085.5;
            }
        }
    }
    return julian;
}

void getJulanToDateTime(int dtime[], double julian, bool opt)
//　マイコン宇宙講座（廣済堂 1980 中野主一著）
// 35頁 サブルーチンJDATE リスト2-2
{
    int month_date[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    double temp_julian, julian_day;
    int year, month, day;
    double r1, r2;
    int m;

    // ユリウス日を修正ユリウス日にする
    temp_julian = julian - 2400000.5;

    // 年数を求め、月を1、日を0として年の最初のユリウス日を求める
    year = trunc(0.0027379093 * temp_julian + 1858.877);
    month = 1;
    day = 0;
    julian_day = getJulianDay(year, month, day);

    // 日本標準時の場合は世界時にするため、-9時間とする
    if (opt == 0) {
      julian_day -= 0.375;
    }

    r2 = temp_julian - (julian_day - 2400000.5);

    // 閏年のチェック - 2月の日数を29日にする
    if (((int)year % 4 == 0 && (int)year % 100 != 0) || (int)year % 400 == 0) {
      month_date[2] = 29;
    }

    // 月を求める
    r1 = 0.0;
    m = 1;
    while (m < 13) {
        if ((floor(r2) - r1 - month_date[m]) <= 0) {
            break;
        }
        r1 += month_date[m];
        m++;
    }
    month = m;

    // 日は時刻を含めるケースがあるので整数にする
    day = floor(r2 - r1);

    // 閏年の際に変更した2月の日数を元に戻す
    month_date[2] = 28;

    // 月が1月、2月時の変更を元に戻す
    if (month == 13) {
      year += 1;
      month -= 12;
    }

    dtime[0] = year;
    dtime[1] = month;
    dtime[2] = day;
}


QString setFormatString(int x)
{
    return QString("%1").arg(x, 2, 10, QChar('0'));
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // tableWidgetのヘッダー幅の設定
    ui->tableWidget->setColumnWidth(0, 100);    // 年月日
    ui->tableWidget->setColumnWidth(1, 100);    // 時刻
    ui->tableWidget->setColumnWidth(2, 130);    // ユリウス日
    ui->tableWidget->setColumnWidth(3, 115);    // 修正ユリウス日

    // 行の高さをセット　1行毎に設定する
    // ui->tableWidget->setRowHeight( 0, 20 );

    // 現在の時刻
    ui->nowRadioButton->setChecked(true);

    // 現在の日付と時刻をセット
    setDateTime_Now();

    // タイムゾーンを日本標準時にセット
    ui->timeZoneTComboBox->setCurrentIndex(0);

    // フォーカスセット
    ui->yearEdit->setFocus();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setjulianDay(int row, int year, int month, int day, int hour, int minute, int second, double jd, double mjd)
{
    QString jdStr, mjdStr;
    QString strDate, strTime;

    // 文字列に変換
    strDate = setFormatString(year) + '/' + setFormatString(month) + '/' + setFormatString(day);
    strTime = setFormatString(hour) + ':' + setFormatString(minute) + ':' + setFormatString(second);
    jdStr = QString::number(jd, 'f', 5);
    mjdStr = QString::number(mjd, 'f', 5);

    // TableWidgetに値をセットする
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(strDate));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(strTime));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(jdStr));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(mjdStr));

    // セル内の文字列の配置を設定
    ui->tableWidget->item(row, 0)->setTextAlignment(Qt::AlignCenter | Qt::AlignCenter);
    ui->tableWidget->item(row, 1)->setTextAlignment(Qt::AlignCenter | Qt::AlignCenter);
    ui->tableWidget->item(row, 2)->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
    ui->tableWidget->item(row, 3)->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
}

void MainWindow::setDateTime_Now()
{
    // 現在の日付と時刻をセット
    time_t t = time(NULL);
    struct tm *local = localtime(&t);

    ui->yearEdit->setText(QString::number(local->tm_year + 1900));
    ui->monthBox->setValue(local->tm_mon + 1);
    ui->dayBox->setValue(local->tm_mday);
    ui->hourBox->setValue(local->tm_hour);
    ui->minuteBox->setValue(local->tm_min);
    ui->secondBox->setValue(local->tm_sec);
}


void MainWindow::setDateTime_Zero()
{
    // 時刻を0時にする
    ui->hourBox->setValue(0);
    ui->minuteBox->setValue(0);
    ui->secondBox->setValue(0);
}


void MainWindow::on_closeButton_clicked()
{
    // 終了
    exit(EXIT_FAILURE);
}


void MainWindow::on_clearButto_clicked()
{
    // tableWidgetをクリア
    ui->tableWidget->clearSelection();
    ui->tableWidget->disconnect();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(1);
}


void MainWindow::on_calcButton_clicked()
{
    double julian, modifyJulian;
    int dtime[] = {0, 0, 0};
    int year, month, day;
    int hour, minute, second;
    bool opt;

    // 日付と時刻を取得
    year = ui->yearEdit->text().toInt();
    month = ui->monthBox->value();
    day = ui->dayBox->value();
    hour = ui->hourBox->value();
    minute = ui->minuteBox->value();
    second = ui->secondBox->value();

    // ユリウス日を求める
    julian = getJulianDay(year, month, day);
    julian += (hour / 24.0 + minute / 1440.0 + second / 86400.0);

    // 日本標準時なら-0.375を引いて世界時
    opt = ui->timeZoneTComboBox->currentIndex();
    if (opt== 0) {
        julian -= 0.375;
    }

    // 修正ユリウス日の計算
    modifyJulian = julian - 2400000.5;

    // １ヶ月間の計算
    if (ui->monthCheckBox->isChecked()) {
        for (int i = 0; i < 31; i++) {
            ui->tableWidget->setRowCount(i + 1);
            setjulianDay(i, year, month, day, hour, minute, second, julian, modifyJulian);
            julian += 1.0;
            modifyJulian = julian - 2400000.5;
            getJulanToDateTime(dtime, julian, opt);
            year = dtime[0];
            month = dtime[1];
            day = dtime[2];
        }
    } else {
        // 指定した日のみ計算
        setjulianDay(0, year, month, day, hour, minute, second, julian, modifyJulian);
    }
}


void MainWindow::on_zeroRadioButton_clicked()
{
    // 時刻を０時にする
    setDateTime_Zero();
}


void MainWindow::on_nowRadioButton_clicked()
{
    // 時刻を現在の時刻にする
    setDateTime_Now();
}

void MainWindow::on_monthCheckBox_clicked()
{
    // チェックがDisableであればtableWidgetをクリア
    if (!ui->monthCheckBox->isChecked()) {
        ui->tableWidget->clearSelection();
        ui->tableWidget->disconnect();
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(1);
    }
}

