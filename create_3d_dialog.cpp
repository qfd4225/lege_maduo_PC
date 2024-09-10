#include "create_3d_dialog.h"
#include "ui_create_3d_dialog.h"
#include<QMessageBox>
create3dDialog::create3dDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::create3dDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=]()
            {
                auto tray_length = ui->lengthLineEdit->text();
                auto tray_width = ui->widthLineEdit->text();
                auto tray_height = ui->heightLineEdit->text();
                auto tray_Widght = ui->totalweightLineEdit->text();
                auto goods_length = ui->goods_length_LineEdit->text();
                auto goods_width = ui->goods_width_LineEdit->text();
                auto goods_height = ui->goods_height_LineEdit->text();
                auto goods_Widght = ui->weightLineEdit->text();
                auto outRatio = ui->outRatioLineEdit->text();

                if (tray_length.isEmpty() || tray_width.isEmpty() || tray_height.isEmpty() || tray_Widght.isEmpty() || goods_length.isEmpty() ||
                    goods_width.isEmpty() || goods_height.isEmpty() || goods_Widght.isEmpty() || outRatio.isEmpty()) {
                    QMessageBox::warning(NULL, "提示！", "尚未填写完成!",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                    return;
                }

                QStringList combo_info;
                combo_info << tray_length << tray_width << tray_height << tray_Widght <<
                    goods_length << goods_width << goods_height << goods_Widght << outRatio;
                emit confirmSignal(combo_info);
            });
}

create3dDialog::~create3dDialog()
{
    delete ui;
}
