#include "mainwindow.h"
#include "database.h"
#include <QApplication>

static const QString styleSheet =
        "QMainWindow, QDialog { "
        "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 2, stop: 0 #C0C0C0, stop: 1 #FFFFFF); "
        "}"

        "QLabel { font: bold 12px; }"

        "QGroupBox { "
        "  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 2, stop: 0 #99CCFF, stop: 1 #FFFFFF); "
        "  border: 2px solid #3399FF; "
        "  border-radius: 5px; "
        "  margin-top: 2ex"
        "}"

        "QTabWidget::pane { "
        "  border: 2px solid #8f8f91; "
        "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 2, stop: 0 #C0C0C0, stop: 1 #FFFFFF); "
        "  border-radius: 4px; "
        "  margin-left: 8px; "
        "  margin-right: 8px; "
        "  margin-bottom: 8px; "
        "}"

        "QTabWidget::tab-bar { "
        "  alignment: center; "
        "}"

        "QLineEdit, QComboBox, QDateEdit { "
        "  border: 1px solid #3399FF; "
        "  border-radius: 3px; "
        "  min-height: 20px;"
        "}"

        "QSplitter::handle {"
        "  background: solid lightgrey; "
        "  border: 5px; "
        "}"

        "QPushButton { "
        "  border: 2px solid #8f8f91; "
        "  border-radius: 6px; "
        "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); "
        "  min-width: 80px;"
        "  min-height: 26px;"
        "}"

        "QTableView { "
        "  background: lightGray; "
        "  border: 1px solid #3399FF; "
        "  border-radius: 3px; "
        "}"

        "QTableView::item { "
        "  background-color: white; "
        "}"

        "QTableView::item:selected { "
        "  background-color: palette(highlight); "
        "}"

        "QHeaderView::section {"
        "  padding: 2px; "
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
