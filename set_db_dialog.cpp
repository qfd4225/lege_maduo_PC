#include "set_db_dialog.h"
#include "ui_set_db_dialog.h"

setDBDialog::setDBDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::setDBDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [=](){
        QVector<int>  db_list ;
        int DBTemp = ui->spinBox_DBTemp->value();
        db_list.append(DBTemp);
        int DBLayout = ui->spinBox_DBLayout->value();
        db_list.append(DBLayout);
        emit confirmClicked(db_list);
        this->close();
    });

    connect(ui->buttonBox, &QDialogButtonBox::rejected, [=](){
        this->close();
    });

}

setDBDialog::~setDBDialog()
{
    delete ui;
}
