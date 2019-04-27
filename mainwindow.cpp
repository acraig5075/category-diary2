#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "eventsquerymodel.h"
#include "categoriesdlg.h"
#include "addeventdlg.h"
#include "summarylistpage.h"
#include "summarychartpage.h"
#include "summaryquerymodel.h"
#include <datepickerdlg.h>
#include <QMessageBox>

#define VERSION "1.0"

MainWindow::MainWindow(Database &db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_db(db)
    , m_pEventsModel(new EventsQueryModel(this))
    , m_pSummaryModel(new SummaryQueryModel(this))
    , m_pSummaryListPage(new SummaryListPage(m_pSummaryModel, this))
    , m_pSummaryChartPage(new SummaryChartPage(m_pSummaryModel, this))
{
    ui->setupUi(this);

    setWindowTitle(QString("Category Diary %1").arg(VERSION));

    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setFirstDayOfWeek(Qt::Monday);

    ui->tableView->setModel(m_pEventsModel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(3);

    ui->startDateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->endDateEdit->setDisplayFormat("dd/MM/yyyy");

    if (QDate::currentDate().dayOfWeek() == 0)
        on_lastWeekRangeMenu();
    else
        on_thisWeekRangeMenu();

    ui->page1Layout->addWidget(m_pSummaryListPage);
    ui->page2Layout->addWidget(m_pSummaryChartPage);
    ui->tabWidget->setCurrentIndex(1);

    QAction *deleteAction = new QAction("Delete", this);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->addAction(deleteAction);
    QObject::connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(onDeleteEventMenu()));

    QMenu *setRangeMenu = new QMenu(this);
    QAction *range1 = new QAction("This week", this);
    QAction *range2 = new QAction("Last week", this);
    QAction *range3 = new QAction("Past month", this);
    QAction *range4 = new QAction("Past year", this);
    QAction *range5 = new QAction("All time", this);
    setRangeMenu->addAction(range1);
    setRangeMenu->addAction(range2);
    setRangeMenu->addAction(range3);
    setRangeMenu->addAction(range4);
    setRangeMenu->addAction(range5);
    ui->setRangeButton->setMenu(setRangeMenu);
    connect(range1, SIGNAL(triggered()), this, SLOT(on_thisWeekRangeMenu()));
    connect(range2, SIGNAL(triggered()), this, SLOT(on_lastWeekRangeMenu()));
    connect(range3, SIGNAL(triggered()), this, SLOT(on_pastMonthRangeMenu()));
    connect(range4, SIGNAL(triggered()), this, SLOT(on_pastYearRangeMenu()));
    connect(range5, SIGNAL(triggered()), this, SLOT(on_allTimeRangeMenu()));

    on_calendarWidget_clicked(ui->calendarWidget->selectedDate());

    on_queryButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    QString label = QString("Events for %1")
            .arg(date.toString("dddd, d MMMM yyyy"));

    ui->dateLabel->setText(label);

    m_pEventsModel->setDate(date);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_categoriesButton_clicked()
{
    CategoriesDlg dlg(m_db, this);
    dlg.exec();
}

void MainWindow::on_addEventButton_clicked()
{
    QDate date = ui->calendarWidget->selectedDate();
    AddEvent(date);
}

void MainWindow::AddEvent(const QDate &date)
{
    AddEventDlg dlg(date, m_db, this);

    if (dlg.exec() == QDialog::Accepted)
    {
        m_pEventsModel->update();
        ui->tableView->resizeColumnsToContents();

        if (ui->startDateEdit->date() <= date && date <= ui->endDateEdit->date())
            on_queryButton_clicked();
    }
}

void MainWindow::on_calendarWidget_activated(const QDate &date)
{
    AddEvent(date);
}

void MainWindow::onDeleteEventMenu()
{
    int index = -1;
    QItemSelectionModel *selection = ui->tableView->selectionModel();
    if (selection && selection->hasSelection())
    {
        QModelIndexList indices = selection->selectedIndexes();
        if (indices.size() > 0)
        {
            index = indices[0].row();
        }
    }

    if (index >= 0)
    {
        if (QMessageBox::Yes == QMessageBox::question(this, "Confirm", "Are you sure?"))
        {
            int id = m_pEventsModel->getId(index);
            if (m_db.deleteEvent(id))
            {
                m_pEventsModel->update();
                ui->tableView->resizeColumnsToContents();
            }
        }
    }
}

void MainWindow::on_queryButton_clicked()
{
    QDate fromDate = ui->startDateEdit->date();
    QDate toDate = ui->endDateEdit->date();

    if (fromDate > toDate)
        qSwap(fromDate, toDate);

    bool ok;
    int totalSum = m_db.sumRangePercentage(fromDate, toDate, ok);
    if (ok)
    {
        SetSummaryModel(fromDate, toDate, totalSum);
        m_pSummaryListPage->refresh(fromDate, toDate);
        m_pSummaryChartPage->refresh(fromDate, toDate);
    }
}

void MainWindow::on_startDateButton_clicked()
{
    QDate date;
    DatePickerDlg dlg(date, this);
    if (QDialog::Accepted == dlg.exec())
        ui->startDateEdit->setDate(date);
}

void MainWindow::on_endDateButton_clicked()
{
    QDate date;
    DatePickerDlg dlg(date, this);
    if (QDialog::Accepted == dlg.exec())
        ui->endDateEdit->setDate(date);
}


void MainWindow::on_thisWeekRangeMenu()
{
    QDate today = QDate::currentDate();
    int weekDay = today.dayOfWeek();
    QDate weekStart = today.addDays(-weekDay);
    QDate weekEnd = weekStart.addDays(7);
    ui->startDateEdit->setDate(weekStart);
    ui->endDateEdit->setDate(weekEnd);
}

void MainWindow::on_lastWeekRangeMenu()
{
    QDate today = QDate::currentDate();
    int weekDay = today.dayOfWeek();
    QDate weekStart = today.addDays(-weekDay);
    QDate weekEnd = weekStart.addDays(7);
    ui->startDateEdit->setDate(weekStart.addDays(-7));
    ui->endDateEdit->setDate(weekEnd.addDays(-7));
}

void MainWindow::on_pastMonthRangeMenu()
{
    QDate today = QDate::currentDate();
    ui->startDateEdit->setDate(today.addMonths(-1));
    ui->endDateEdit->setDate(today);
}

void MainWindow::on_pastYearRangeMenu()
{
    QDate today = QDate::currentDate();
    ui->startDateEdit->setDate(today.addYears(-1));
    ui->endDateEdit->setDate(today);
}

void MainWindow::on_allTimeRangeMenu()
{
    QDate today = QDate::currentDate();
    ui->startDateEdit->setDate(QDate(2000, 1, 1));
    ui->endDateEdit->setDate(today);
}

void MainWindow::SetSummaryModel(const QDate &fromDate, const QDate &toDate, int totalSum)
{
    m_pSummaryModel->setFromDate(fromDate);
    m_pSummaryModel->setToDate(toDate);
    m_pSummaryModel->setTotalSum(totalSum);
    m_pSummaryModel->update();
}
