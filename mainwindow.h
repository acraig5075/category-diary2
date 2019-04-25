#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Database;
class EventsQueryModel;
class SummaryListPage;
class SummaryChartPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Database &db, QWidget *parent = nullptr);
    ~MainWindow();

private:
    void AddEvent(const QDate &date);
    void EventsChanged();

private slots:
    void on_calendarWidget_clicked(const QDate &date);
    void on_categoriesButton_clicked();
    void on_addEventButton_clicked();
    void on_calendarWidget_activated(const QDate &date);
    void on_queryButton_clicked();
    void onDeleteEventMenu();

private:
    Ui::MainWindow *ui;
    Database &m_db;
    EventsQueryModel *m_pEventsModel = nullptr;
    SummaryListPage *m_pSummaryListPage = nullptr;
    SummaryChartPage *m_pSummaryChartPage = nullptr;
};

#endif // MAINWINDOW_H