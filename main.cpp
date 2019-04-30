#include "mainwindow.h"
#include "database.h"
#include <QApplication>

static const QString styleSheet =
        "QMainWindow, QDialog { "
        "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 2, stop: 0 #C0C0C0, stop: 1 #FFFFFF); "
        "}"

        "QLabel { font: bold 12px; }"

        "QGroupBox { "
        "  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 2, stop: 0 #D1D7B3, stop: 1 #FFFFFF); "
        "  border: 1px solid #808000; "
        "  border-radius: 5px; "
        "  margin-top: 2ex"
        "}"

        "QTableView { "
        "  background: lightGray; "
        "}"

        "QTableView::item { "
        "  background-color: white; "
        "}"

        "QTableView::item:selected { "
        "  background-color: palette(highlight); "
        "}"

        "QHeaderView::section {"
        "  border: 1px solid lightGray; "
        "}"
        ;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(styleSheet);

    Database db;
    db.connect();

    MainWindow w(db);
    w.show();

    return a.exec();
}
