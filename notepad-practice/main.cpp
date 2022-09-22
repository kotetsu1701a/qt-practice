#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QTranslator translator;
    // const QStringList uiLanguages = QLocale::system().uiLanguages();
    // for (const QString &locale : uiLanguages) {
    //     const QString baseName = "notepad-practice_" + QLocale(locale).name();
    //     if (translator.load(":/i18n/" + baseName)) {
    //        a.installTranslator(&translator);
    //        break;
    //     }
    // }


    //ボタンおよびコンテキストメニューを日本語化
    QTranslator translator;
    if(translator.load("qt_ja", "/usr/share/qt5/translations")) {
        QCoreApplication::installTranslator(&translator);
    }


    MainWindow w;
    w.show();
    return a.exec();
}
