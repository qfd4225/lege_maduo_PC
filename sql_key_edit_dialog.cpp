#include "sql_key_edit_dialog.h"
#include "ui_sql_key_edit_dialog.h"
#include<QMessageBox>

sqlKeyEditDialog::sqlKeyEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::sqlKeyEditDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_cancle, &QPushButton::clicked, this, [=]()
            {
            clearForm();
            this->hide();
    });

    connect(ui->pushButton_confirm, &QPushButton::clicked, this, [=]()
            {
                QString skuCode = ui->lineEdit_skuCode->text();
                QString TrayLength = ui->lineEdit_TrayLength->text();
                QString TrayWidth = ui->lineEdit_TrayWidth->text();
                QString TrayMaxHeight = ui->lineEdit_TrayMaxHeight->text();
                QString TrayMaxWeight = ui->lineEdit_TrayMaxWeight->text();
                QString TrayMaxNumber = ui->lineEdit_TrayMaxNumber->text();

                if (skuCode.isEmpty() || TrayLength.isEmpty() || TrayWidth.isEmpty() ||
                    TrayMaxHeight.isEmpty() || TrayMaxWeight.isEmpty() || TrayMaxNumber.isEmpty())
                {
                    QMessageBox::warning(NULL, "提示！", "请完成信息填写!",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                    return;
                }

                QStringList combo_info;
                combo_info << skuCode << TrayLength << TrayWidth << TrayMaxHeight << TrayMaxWeight << TrayMaxNumber;
                emit confirmSignal(combo_info);
            });
}

sqlKeyEditDialog::~sqlKeyEditDialog()
{
    delete ui;
}

void sqlKeyEditDialog::clearForm()
{
    ui->lineEdit_skuCode->clear();
    ui->lineEdit_TrayLength->clear();
    ui->lineEdit_TrayWidth->clear();
     ui->lineEdit_TrayMaxHeight->clear();
     ui->lineEdit_TrayMaxWeight->clear();
     ui->lineEdit_TrayMaxNumber->clear();
}

