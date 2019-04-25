#include "eventcalendarwidget.h"
#include <QSqlQuery>
#include <QVariant>
#include <QPainter>

EventCalendarWidget::EventCalendarWidget(QWidget *parent)
    : QCalendarWidget(parent)
    , m_eventImage(QImage(":/images/eventindicator.png"))
{
}

int eventsForDate(const QDate &date)
{
    const QString queryStr = QString::fromLatin1(
                "SELECT COUNT(1) AS count "
                "FROM Events "
                "WHERE '%1' >= myDate AND '%1' <= myDate").arg(date.toString("yyyy-MM-dd"));

    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");

    int count = 0;
    if (query.next())
    {
        count = query.value("count").toInt();
    }

    return count;
}

void EventCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    if (eventsForDate(date) > 0)
    {
        painter->save();
        painter->drawImage(rect.topLeft(), m_eventImage);
        painter->drawText(rect, Qt::AlignCenter|Qt::TextSingleLine, QString::number(date.day()));
        painter->restore();
    }
    else
    {
        QCalendarWidget::paintCell(painter, rect, date);
    }
}
