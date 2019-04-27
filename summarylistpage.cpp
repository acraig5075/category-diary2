#include "summarylistpage.h"
#include "ui_summarylistpage.h"
#include "summaryquerymodel.h"

SummaryListPage::SummaryListPage(SummaryQueryModel *pModel, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SummaryListPage)
    , m_pModel(pModel)
{
    ui->setupUi(this);
    ui->tableView->setModel(m_pModel);
}

SummaryListPage::~SummaryListPage()
{
    delete ui;
}

void SummaryListPage::refresh(const QDate &fromDate, const QDate &toDate)
{
    ui->tableView->resizeColumnsToContents();

    QString label = QString("Summary for %1 to %2")
            .arg(fromDate.toString("d MMMM yyyy"))
            .arg(toDate.toString("d MMMM yyyy"));
    ui->rangeLabel->setText(label);
}
