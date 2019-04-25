#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "eventsquerymodel.h"
#include "categoriesdlg.h"
#include "addeventdlg.h"
#include "summarylistpage.h"
#include "summarychartpage.h"
#include <QMessageBox>

#define VERSION "1.0"

MainWindow::MainWindow(Database &db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_db(db)
    , m_pEventsModel(new EventsQueryModel(this))
    , m_pSummaryListPage(new SummaryListPage(this))
    , m_pSummaryChartPage(new SummaryChartPage(this))
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
    ui->startDateEdit->setDate(QDate::currentDate().addDays(-7));
    ui->endDateEdit->setDate(QDate::currentDate());

    ui->page1Layout->addWidget(m_pSummaryListPage);
    ui->page2Layout->addWidget(m_pSummaryChartPage);
    ui->tabWidget->setCurrentIndex(1);

    QAction *deleteAction = new QAction("Delete", this);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->addAction(deleteAction);
    QObject::connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(onDeleteEventMenu()));

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
        m_pSummaryListPage->setModel(fromDate, toDate, totalSum);
        m_pSummaryChartPage->setModel(fromDate, toDate, totalSum);
    }
}
