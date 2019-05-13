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

    ui->listView->setModel(m_pModel);
    ui->listView->setModelColumn(1);
}

AddEventDlg::~AddEventDlg()
{
    delete ui;
}

void AddEventDlg::on_buttonBox_accepted()
{
    QItemSelectionModel *selection = ui->listView->selectionModel();
    if (selection && selection->hasSelection())
    {
        QModelIndexList indexes = selection->selectedIndexes();
        for (int i = 0; i < indexes.size(); ++i)
        {
            int id = m_pModel->getId(indexes[i].row());
            m_db.addEvent(m_date, id);
        }
    }
}
