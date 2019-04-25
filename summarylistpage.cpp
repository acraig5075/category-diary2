#include "summarylistpage.h"
#include "ui_summarylistpage.h"
#include "summaryquerymodel.h"

SummaryListPage::SummaryListPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SummaryListPage)
    , m_pModel(new SummaryQueryModel(this))
{
    ui->setupUi(this);
    ui->tableView->setModel(m_pModel);
}

SummaryListPage::~SummaryListPage()
{
    delete ui;
}

void SummaryListPage::setModel(const QDate &fromDate, const QDate &toDate, int totalSum)
{
    m_pModel->setFromDate(fromDate);
    m_pModel->setToDate(toDate);
    m_pModel->setTotalSum(totalSum);
    m_pModel->update();

    ui->tableView->resizeColumnsToContents();

    QString label = QString("Summary for %1 to %2")
            .arg(fromDate.toString("d MMMM yyyy"))
            .arg(toDate.toString("d MMMM yyyy"));
    ui->rangeLabel->setText(label);
}
