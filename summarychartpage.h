#ifndef SUMMARYCHARTPAGE_H
#define SUMMARYCHARTPAGE_H

#include <QWidget>

namespace Ui {
class SummaryChartPage;
}
namespace QtCharts {
class QVPieModelMapper;
}

class SummaryQueryModel;

class SummaryChartPage : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryChartPage(SummaryQueryModel *pModel, QWidget *parent = nullptr);
    ~SummaryChartPage();

    void refresh(const QDate &fromDate, const QDate &toDate);

private:
    Ui::SummaryChartPage *ui;
    SummaryQueryModel *m_pModel = nullptr;
    QtCharts::QVPieModelMapper *m_pMapper = nullptr;
};

#endif // SUMMARYCHARTPAGE_H
