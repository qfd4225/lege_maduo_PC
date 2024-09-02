#include "sql_box_edit_dialog.h"
#include "ui_sql_box_edit_dialog.h"
#include<QMessageBox>
SqlBoxEditDialog::SqlBoxEditDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SqlBoxEditDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=]()
    {
        auto sku_code_str = ui->lineEdit_SkuCode->text();
        auto height = ui->doubleSpinBox_height->text();
        auto length = ui->doubleSpinBox_length->text();
        auto weight = ui->doubleSpinBox_weight->text();
        auto width = ui->doubleSpinBox_width->text();

        if (sku_code_str.isEmpty() || height.isEmpty() || length.isEmpty() || weight.isEmpty() || width.isEmpty()) {
            QMessageBox::warning(NULL, "提示！", "尚未填写完成!",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
            return;
        }

        QStringList combo_info;
        combo_info << sku_code_str << height << length << weight << width;
        emit confirmSignal(opera_mode, combo_info);
    });
}

SqlBoxEditDialog::~SqlBoxEditDialog()
{
    delete ui;
}

void SqlBoxEditDialog::setAll(QStringList combo_info)
{

}

void SqlBoxEditDialog::clearAll()
{
    ui->lineEdit_SkuCode->setEnabled(true);
    ui->lineEdit_SkuCode->setText("");
    ui->doubleSpinBox_height->setValue(0.00);
    ui->doubleSpinBox_length->setValue(0.00);
    ui->doubleSpinBox_weight->setValue(0.00);
    ui->doubleSpinBox_width->setValue(0.00);
}
