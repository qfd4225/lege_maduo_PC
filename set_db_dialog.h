#ifndef SET_DB_DIALOG_H
#define SET_DB_DIALOG_H

#include <QDialog>

namespace Ui {
class setDBDialog;
}

class setDBDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setDBDialog(QWidget *parent = nullptr);
    ~setDBDialog();
signals:
    void confirmClicked(QVector<int> db_list);
private:
    Ui::setDBDialog *ui;
};

#endif // SET_DB_DIALOG_H
