#ifndef DATEPICKERDLG_H
#define DATEPICKERDLG_H

#include <QDialog>

namespace Ui {
class DatePickerDlg;
}

class DatePickerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DatePickerDlg(QDate &date, QWidget *parent = nullptr);
    ~DatePickerDlg();

private slots:
    void on_DatePickerDlg_accepted();

private:
    Ui::DatePickerDlg *ui;
    QDate &m_date;
};

#endif // DATEPICKERDLG_H
