#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();                       // 実行時lineEditにフォーカスをセット
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_closeButton_clicked()
{
    // 終了
    exit(0);
}


void MainWindow::on_clearButton_clicked()
{
    ui->lineEdit->setFocus();                       // 実行時lineEditにフォーカスをセット
}


void MainWindow::on_deleteButton_clicked()
{
    int idx;

    // 選択した項目の削除
    idx = ui->listWidget->currentRow();             // 選択したアイテムのインデックスを取得
    ui->listWidget->takeItem(idx);                  // 選択したインデックスのアイテムを削除
}


void MainWindow::on_addButton_clicked()
{
    QString str;

    // 追加ボタンによる追加処理
    str = ui->lineEdit->text();
    if (!str.isEmpty()){                            // 空文字をLListWidgetへ追加しないための判断
        ui->listWidget->addItem(str);               // ListWidgetへアイテムを追加
        ui->lineEdit->clear();                      // LineEditの内容をクリア
        ui->lineEdit->setFocus();                   // 実行時lineEditにフォーカスをセット
    }
}


void MainWindow::on_lineEdit_returnPressed()
{
    QString str;

    // Enterキーによる追加処理
    str = ui->lineEdit->text();
    if (!str.isEmpty()) {                           // 空文字をLListWidgetへ追加しないための判断
        ui->listWidget->addItem(str);               // ListWidgetへアイテムを追加
        ui->lineEdit->clear();                      // LineEditの内容をクリア
        ui->lineEdit->setFocus();                   // 実行時lineEditにフォーカスをセット
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    bool sort;

    // ソートチェック
    sort = ui->checkBox->checkState();              // 現在のチェック状態を取得

    // ソート状態であれば項目をソート
    if (sort) {
        ui->listWidget->setSortingEnabled(true);    // Enable
        ui->listWidget->sortItems();                // Sort
    } else {
        // ソート状態を解除
        ui->listWidget->setSortingEnabled(false);   // Disable
    }
    ui->lineEdit->setFocus();                       // 実行時lineEditにフォーカスをセット
}
