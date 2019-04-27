#ifndef SUMMARYLISTPAGE_H
#define SUMMARYLISTPAGE_H

#include <QWidget>

namespace Ui {
class SummaryListPage;
}

class SummaryQueryModel;

class SummaryListPage : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryListPage(SummaryQueryModel *pModel, QWidget *parent = nullptr);
    ~SummaryListPage();

    void refresh(const QDate &fromDate, const QDate &toDate);

private:
    Ui::SummaryListPage *ui;
    SummaryQueryModel *m_pModel = nullptr;
};

#endif // SUMMARYLISTPAGE_H
