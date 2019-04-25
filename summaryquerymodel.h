#ifndef SUMMARYQUERYMODEL_H
#define SUMMARYQUERYMODEL_H

#include <QSqlQueryModel>
#include <QDate>

class SummaryQueryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    const QString QueryFormatStr =
            "SELECT Categories.description AS Description, SUM(Events.percentage) * 100.0 / %1 AS Percent "
            "FROM Events "
            "INNER JOIN Categories "
            "ON Events.categoryId = Categories.id "
            "WHERE '%2' <= myDate "
            "AND '%3' >= myDate "
            "GROUP BY categoryId "
            "ORDER BY percent DESC ";

    enum Roles { DateRole = Qt::UserRole + 1, PercentRole, NameRole, IdRole };

    SummaryQueryModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    void setFromDate(const QDate &date);
    void setToDate(const QDate &date);
    void setTotalSum(int totalSum);

public slots:
    void update();

private:
    int m_totalSum = 1;
    QDate m_fromDate;
    QDate m_toDate;
};

#endif
