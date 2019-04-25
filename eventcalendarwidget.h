#ifndef EVENTCALENDARWIDGET_H
#define EVENTCALENDARWIDGET_H

#include <QObject>
#include <QCalendarWidget>

class Database;

class EventCalendarWidget : public QCalendarWidget
{
public:
    EventCalendarWidget(QWidget *parent = nullptr);

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const override;

private:
    QImage m_eventImage;
};

#endif // EVENTCALENDARWIDGET_H
