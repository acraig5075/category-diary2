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
    explicit SummaryListPage(QWidget *parent = nullptr);
    ~SummaryListPage();

    void setModel(const QDate &fromDate, const QDate &toDate, int totalSum);

private:
    Ui::SummaryListPage *ui;
    SummaryQueryModel *m_pModel = nullptr;
};

#endif // SUMMARYLISTPAGE_H
