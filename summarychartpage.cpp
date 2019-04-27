#include "summarychartpage.h"
#include "ui_summarychartpage.h"
#include "summaryquerymodel.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QVPieModelMapper>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

SummaryChartPage::SummaryChartPage(SummaryQueryModel *pModel, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SummaryChartPage)
    , m_pModel(pModel)
    , m_pMapper(new QVPieModelMapper(this))
{
    ui->setupUi(this);

    QPieSeries *series = new QPieSeries();

    m_pMapper->setLabelsColumn(0);
    m_pMapper->setValuesColumn(1);
    m_pMapper->setSeries(series);
    m_pMapper->setModel(m_pModel);

    if (!series->isEmpty())
    {
        QPieSlice *slice = series->slices().at(0);
        slice->setExploded(true);
        slice->setLabelVisible(true);
        slice->setPen(QPen(Qt::darkGreen, 2));
        slice->setBrush(Qt::green);
    }

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

void SummaryChartPage::refresh(const QDate &fromDate, const QDate &toDate)
{
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
