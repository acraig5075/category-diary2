#include "categoriesdlg.h"
#include "ui_categoriesdlg.h"
#include "categoryquerymodel.h"
#include "database.h"
#include <QInputDialog>
#include <QAction>
#include <QMessageBox>

CategoriesDlg::CategoriesDlg(Database &db, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CategoriesDlg)
    , m_db(db)
    , m_pModel(new CategoryQueryModel(this))
{
    ui->setupUi(this);

    ui->tableView->setModel(m_pModel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *deleteAction = new QAction("Delete", this);
    QAction *renameAction = new QAction("Rename...", this);

    ui->tableView->addAction(deleteAction);
    ui->tableView->addAction(renameAction);

    QObject::connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(onDeleteMenu()));
    QObject::connect(renameAction, SIGNAL(triggered(bool)), this, SLOT(onRenameMenu()));
}

CategoriesDlg::~CategoriesDlg()
{
    delete ui;
}

void CategoriesDlg::on_addButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add Category", "Name", QLineEdit::Normal, "New category", &ok);
    if (ok)
    {
        if (m_db.addCategory(name))
            m_pModel->update();
    }
}

int CategoriesDlg::getCurrentSelection()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();
    if (selection && selection->hasSelection())
    {
        QModelIndexList indices = selection->selectedIndexes();
        if (indices.size() > 0)
        {
            return indices[0].row();
        }
    }

    return -1;
}

void CategoriesDlg::onDeleteMenu()
{
    int index = getCurrentSelection();
    if (index >= 0)
    {
        int id = m_pModel->getId(index);
        QString name = m_pModel->getName(index);

        if (QMessageBox::No == QMessageBox::question(this, "Delete", QString("Are you sure you want to delete category %1").arg(name)))
            return;

        bool ok = m_db.deleteCategory(id);
        if (ok)
            m_pModel->update();
        else
            QMessageBox::warning(this, "Delete", QString("Failed to delete %1. (Is it unused?)").arg(name));
    }
}

void CategoriesDlg::onRenameMenu()
{
    int index = getCurrentSelection();
    if (index >= 0)
    {
        int id = m_pModel->getId(index);
        QString oldName = m_pModel->getName(index);

        bool ok;
        QString newName = QInputDialog::getText(this, "Rename", "Rename category", QLineEdit::Normal, oldName, &ok);

        if (ok)
            ok = m_db.renameCategory(id, newName);
        if (ok)
            m_pModel->update();
    }
}
