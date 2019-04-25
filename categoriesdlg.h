#ifndef CATEGORIESDLG_H
#define CATEGORIESDLG_H

#include <QDialog>

namespace Ui {
class CategoriesDlg;
}

class Database;
class CategoryQueryModel;

class CategoriesDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CategoriesDlg(Database &db, QWidget *parent = nullptr);
    ~CategoriesDlg();

private:
    int getCurrentSelection();

private slots:
    void on_addButton_clicked();
    void onDeleteMenu();
    void onRenameMenu();

private:
    Ui::CategoriesDlg *ui;
    Database &m_db;
    CategoryQueryModel *m_pModel = nullptr;
};

#endif // CATEGORIESDLG_H
