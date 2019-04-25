#ifndef ADDEVENTDLG_H
#define ADDEVENTDLG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class AddEventDlg;
}

class CategoryQueryModel;
class Database;

class AddEventDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddEventDlg(const QDate &date, Database &db, QWidget *parent = nullptr);
    ~AddEventDlg();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddEventDlg *ui;
    CategoryQueryModel *m_pModel = nullptr;
    QDate m_date;
    Database &m_db;
};

#endif // ADDEVENTDLG_H
