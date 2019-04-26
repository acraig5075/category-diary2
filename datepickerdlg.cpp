#include "datepickerdlg.h"
#include "ui_datepickerdlg.h"

DatePickerDlg::DatePickerDlg(QDate &date, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatePickerDlg)
    , m_date(date)
{
    ui->setupUi(this);
    ui->calendarWidget->showToday();
}

DatePickerDlg::~DatePickerDlg()
{
    delete ui;
}

void DatePickerDlg::on_DatePickerDlg_accepted()
{
    m_date = ui->calendarWidget->selectedDate();
}
