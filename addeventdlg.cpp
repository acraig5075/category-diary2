#include "addeventdlg.h"
#include "ui_addeventdlg.h"
#include "categoryquerymodel.h"
#include "database.h"

AddEventDlg::AddEventDlg(const QDate &date, Database &db, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddEventDlg)
    , m_pModel(new CategoryQueryModel(this))
    , m_date(date)
    , m_db(db)
{
    ui->setupUi(this);

    QString caption = QString("Add Event for %1")
            .arg(date.toString("d MMMM yyyy"));
    setWindowTitle(caption);

    ui->comboBox->setModel(m_pModel);
    ui->comboBox->setModelColumn(1);
}

AddEventDlg::~AddEventDlg()
{
    delete ui;
}

void AddEventDlg::on_buttonBox_accepted()
{
    int index = ui->comboBox->currentIndex();
    if (index != -1)
    {
        int id = m_pModel->getId(index);
        m_db.addEvent(m_date, id);
    }
}
