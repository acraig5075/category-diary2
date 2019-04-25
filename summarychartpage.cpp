#include "summarychartpage.h"
#include "ui_summarychartpage.h"
#include "summaryquerymodel.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QVPieModelMapper>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

SummaryChartPage::SummaryChartPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SummaryChartPage)
    , m_pModel(new SummaryQueryModel(this))
    , m_pMapper(new QVPieModelMapper(this))
{
    ui->setupUi(this);

    QPieSeries *series = new QPieSeries();

    m_pMapper->setLabelsColumn(0);
    m_pMapper->setValuesColumn(1);
    m_pMapper->setSeries(series);
    m_pMapper->setModel(m_pModel);

    QPieSlice *slice = series->slices().at(0);
    slice->setExploded(true);
    slice->setLabelVisible(true);
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    //chart->setTheme(QChart::ChartThemeDark);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartLayout->addWidget(chartView);
}

SummaryChartPage::~SummaryChartPage()
{
    delete ui;
}

void SummaryChartPage::setModel(const QDate &fromDate, const QDate &toDate, int totalSum)
{
    m_pModel->setFromDate(fromDate);
    m_pModel->setToDate(toDate);
    m_pModel->setTotalSum(totalSum);
    m_pModel->update();

    QString label = QString("Summary for %1 to %2")
            .arg(fromDate.toString("d MMMM yyyy"))
            .arg(toDate.toString("d MMMM yyyy"));
    ui->rangeLabel->setText(label);

    QPieSeries *series = m_pMapper->series();
    series->setLabelsPosition(QPieSlice::LabelOutside);

    for (int i = 0; i < qMin(3, series->count()); ++i)
    {
        QPieSlice *slice = series->slices().at(i);
        slice->setLabelVisible(true);
    }
}
