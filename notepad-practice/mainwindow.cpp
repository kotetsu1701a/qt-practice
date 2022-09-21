#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFontDialog>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->textEdit);  // TextEditの周囲の隙間をなくす
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::saveFlag()
{
    if (!ui->textEdit->document()->isModified()) {
        return true;
    }
    QMessageBox::StandardButton ret = QMessageBox::warning(this, QCoreApplication::applicationName(),
                         tr("ドキュメントが変更されています。\n"
                            "変更を保存しますか？\n\n"
                            "保存しない場合は変更した内容が破棄されます。"),
                         QMessageBox::Save | QMessageBox::Cancel);

    if (ret == QMessageBox::Save) {
        on_actionSave_triggered();
        return true;
    }
    if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;

}


void MainWindow::on_actionNew_triggered()
{
    if (saveFlag()) {
        file_path_ = "";                // 受け渡すパス名をリセット
        ui->textEdit->setText("");      // 内容を削除する
    }
}


void MainWindow::on_actionOpen_triggered()
{
    // ファイルダイアログを開く
    QString file_name = QFileDialog::getOpenFileName(this, "ファイルを開く");

    // ファイル名が空でなければ開く処理する
    // キャンセルが押された場合は何もしない
    if (!file_name.isEmpty()) {
        QFile file(file_name);
        file_path_ = file_name;

        // ファイルの属性と種類をチェック
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, QCoreApplication::applicationName(), tr("警告", "ファイルを開くことができません"));
            return;
        }

        // 読み込み処理
        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setText(text);
        file.close();
    }

    // カーソルをテキストの最初にセットする
    ui->textEdit->moveCursor(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
}


void MainWindow::on_actionSave_triggered()
{
    // ファイル名なしで保存しようとする場合はファイルダイアログを開く
    if (file_path_.isEmpty()) {
        QString file_name = QFileDialog::getSaveFileName(this, "別名で保存");
        // キャセルが押された場合は何もしない
        // ファイル名が入力されたらパスにセットする
        if (file_name.isEmpty()) {
            return;
        } else {
            file_path_ = file_name;
        }
    }
    QFile file(file_path_);

    // ファイルの属性と種類をチェック
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, QCoreApplication::applicationName(), tr("警告", "ファイルを保存することができません"));
        return;
    }

    // 書き込み処理
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}


void MainWindow::on_actionSaveAs_triggered()
{
    // ファイルダイアログを開く
    QString file_name = QFileDialog::getSaveFileName(this, "別名で保存");

    // ファイル名が空でなければ開く処理する
    // キャンセルが押された場合は何もしない
    if (!file_name.isEmpty()) {
        QFile file(file_name);
        file_path_ = file_name;

        // ファイルの属性と種類をチェック
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, QCoreApplication::applicationName(), tr("警告", "ファイルを保存することができません"));
            return;
        }

        // 書き込み処理
        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.flush();
        file.close();
    }
}


void MainWindow::on_actionQuit_triggered()
{
    if (saveFlag()) {
        close();
    }
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionDelete_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.removeSelectedText();
}


void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok) {
        ui->textEdit->setFont(font);
    } else {
        return;
    }
}


void MainWindow::on_actionNotePad_triggered()
{
    QString about_text;
    about_text = "NotePad version 0.02\n\n";
    about_text += "Base on QT Creator 6.3.2 (GCC 11.2.0-2ubuntu1~22.04)\n\n";
    about_text += "Build date: 2022/09/21 16:50\n\n";
    about_text += "Lisence: GPL and LGPL\n\n";
    about_text += "This application is written under the GPL v3 and LGPL v3 open source licenses provided by Qt Creator.";

    QMessageBox::information(this, "NotePadについて", about_text);
}
