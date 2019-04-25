#include <QSqlRecord>
#include "summaryquerymodel.h"

SummaryQueryModel::SummaryQueryModel(QObject *parent)
    : QSqlQueryModel(parent)
    , m_fromDate(QDate::currentDate())
    , m_toDate(QDate::currentDate())
{
    update();
}

void SummaryQueryModel::update()
{
    QString queryStr = QString(QueryFormatStr)
            .arg(m_totalSum)
            .arg(m_fromDate.toString("yyyy-MM-dd"))
            .arg(m_toDate.toString("yyyy-MM-dd"));

    setQuery(queryStr);
}

QVariant SummaryQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if (index.isValid())
    {
        if (role < Qt::UserRole)
        {
            value = QSqlQueryModel::data(index, role);
        }
        else
        {
            int columnIdx = role - Qt::UserRole - 1;
            QModelIndex modelIndex = this->index(index.row(), columnIdx);
            value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        }
    }
    return value;
}

void SummaryQueryModel::setFromDate(const QDate &date)
{
    m_fromDate = date;
}

void SummaryQueryModel::setToDate(const QDate &date)
{
    m_toDate = date;
}

void SummaryQueryModel::setTotalSum(int totalSum)
{
    m_totalSum = totalSum;
}
