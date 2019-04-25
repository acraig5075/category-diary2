#include <QDebug>
#include <QSqlQuery>
#include <QFile>
#include <QSqlError>
#include <QStandardPaths>
#include <QDate>
#include "database.h"

Database::Database(QObject *parent)
    : QObject(parent)
{
}

bool Database::connect()
{
    QStringList dataLocations = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);

    int i;
    for (i = 0; i < dataLocations.size(); ++i)
    {
        if (connectToDatabase(dataLocations[i] + "/db.sqlite"))
            return true;

        qDebug() << "... failed";\
    }

    if (i == dataLocations.size())
        qDebug() << "No database to work with, aborting...";
    return false;
}

bool Database::connectToDatabase(const QString &name)
{
    qDebug() << QString("Attempting to connect to ") + name;

    if (!QFile(name).exists())
        return create(name);
    else
        return open(name);
}

bool Database::open(const QString &name)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(name);
    if (!m_db.open())
    {
        qDebug() << "Failed to open database: " << m_db.lastError();
        return false;
    }
    return true;
}

void Database::close()
{
    m_db.close();
}

bool Database::create(const QString &name)
{
    if (!open(name))
    {
        qDebug() << "Failed to create the database";
        return false;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS Categories (id INTEGER PRIMARY KEY, description TEXT NOT NULL)");
    query.exec("CREATE TABLE IF NOT EXISTS Events (id INTEGER PRIMARY KEY, myDate DATE NOT NULL, categoryId INT NOT NULL, percentage INT NOT NULL)");
    query.exec("INSERT INTO Categories (description) VALUES ('Default category')");
    return true;
}

bool Database::addCategory(const QString &name)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Categories(description) VALUES(:Name)");
    query.bindValue(":Name", name);

    return query.exec();
}

bool Database::deleteCategory(int id)
{
    QSqlQuery query;
    query.prepare("SELECT count(1) FROM Events WHERE categoryId = :Id");
    query.bindValue(":Id", id);

    if (!query.exec() || !query.next())
    {
        qDebug() << "deleteCategory Failed(1)" << query.lastError();
        return false;
    }

    bool ok;
    int count = query.value(0).toInt(&ok);
    if (!ok || count != 0)
    {
        qDebug() << "deleteCategory Failed(3)" << query.lastError();
        return false;
    }

    query.prepare("DELETE FROM Categories WHERE id = :Id");
    query.bindValue(":Id", id);
    return query.exec();
}

bool Database::renameCategory(int id, const QString &name)
{
    QSqlQuery query;
    query.prepare("UPDATE Categories SET description = :Name WHERE id = :Id");
    query.bindValue(":Name", name);
    query.bindValue(":Id", id);

    return query.exec();
}

bool Database::addEvent(const QDate &date, int categoryId)
{
    QString dateStr = date.toString("yyyy-MM-dd");
    QSqlQuery query;

    query.prepare("SELECT id FROM Categories WHERE id = :CategoryId");
    query.bindValue(":CategoryId", categoryId);

    if (!query.exec() || !query.next())
    {
        qDebug() << "addEvent: Fail (1)" << query.lastError();
        return false;
    }

    query.prepare("INSERT INTO Events (myDate, categoryId, percentage) VALUES (:Date, :CategoryId, 100)");
    query.bindValue(":Date", dateStr);
    query.bindValue(":CategoryId", categoryId);

    if (!query.exec())
    {
        qDebug() << "addEvent: Fail (2)" << query.lastError();
        return false;
    }

    bool ret = ProportionEventsEqually(date);
    return ret;
}

bool Database::ProportionEventsEqually(const QDate &date)
{
    QString dateStr = date.toString("yyyy-MM-dd");
    QSqlQuery query;
    query.prepare("SELECT count(1) FROM Events WHERE myDate = :Date");
    query.bindValue(":Date", dateStr);

    if (!query.exec() || !query.next())
    {
        qDebug() << "ProportionEvents: Fail (1)" << query.lastError();
        return false;
    }

    bool ok;
    int count = query.value(0).toInt(&ok);
    if (!ok || count == 0)
    {
        qDebug() << "ProportionEvents: Fail (2)" << query.lastError();
        return false;
    }

    query.prepare("UPDATE Events SET percentage = :Percent WHERE myDate = :Date");
    query.bindValue(":Percent", 100 / count);
    query.bindValue(":Date", dateStr);

    if (!query.exec())
    {
        qDebug() << "ProportionEvents: Fail (3)" << query.lastError();
        return false;
    }

    return true;
}

int Database::eventsForDate(const QDate &date)
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

int Database::sumRangePercentage(const QDate &fromDate, const QDate &toDate, bool &ok)
{
    QString fromDateStr = fromDate.toString("yyyy-MM-dd");
    QString toDateStr = toDate.toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare("SELECT SUM(percentage) "
                  "FROM Events "
                  "WHERE :FromDate <= myDate "
                  "AND :ToDate >= myDate");
    query.bindValue(":FromDate", fromDateStr);
    query.bindValue(":ToDate", toDateStr);

    if (!query.exec() || !query.next())
    {
        qDebug() << "sumRangePercentage::Fail (1)" << query.lastError();
        ok = false;
        return 0;
    }

    int total = query.value(0).toInt(&ok);
    if (!ok)
    {
        qDebug() << "sumRangePercentage::Fail (2)" << query.lastError();
        return 0;
    }

    if (total == 0)
    {
        qDebug() << "sumRangePercentage 0 rows returned";
        return 0;
    }

    return total;
}

bool Database::deleteEvent(int id)
{
    QSqlQuery query;
    query.prepare("SELECT myDate FROM Events WHERE id = :Id");
    query.bindValue(":Id", id);

    if (!query.exec() || !query.next())
    {
        qDebug() << "deleteEvent::Fail (1)" << query.lastError();
        return false;
    }

    QDate date = query.value(0).toDate();
    if (!date.isValid())
    {
        qDebug() << "deleteEvent::Fail (2) Invalid date";
        return false;
    }

    query.prepare("DELETE FROM Events WHERE id = :Id");
    query.bindValue(":Id", id);

    if (!query.exec())
    {
        qDebug() << "deleteEvent::Fail (3)" << query.lastError();
        return false;
    }

    bool ret = ProportionEventsEqually(date);
    return ret;
}
