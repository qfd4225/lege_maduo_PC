#include "set_ip_dialog.h"
#include "ui_set_ip_dialog.h"
#include <QRegularExpression>
SetIpDialog::SetIpDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetIpDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_confirm, &QPushButton::clicked, [=](){
        QString ipAddress = ui->lineEdit_IP->text();
        QRegularExpression ipRegex(
            "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|["
            "01]?[0-9][0-9]?)$");
        QRegularExpressionMatch match = ipRegex.match(ipAddress);

        QString deviceIp;
        if (match.hasMatch() && (!ipAddress.isEmpty())) {
            deviceIp = ui->lineEdit_IP->text();
        } else {
            deviceIp = "";
        }

        // 发出确认按钮点击的信号
        if(!deviceIp.isEmpty())
        {
            emit confirmClicked(deviceIp);
            this->close();
        }
        else
        {
            qDebug()<<"配置IP无效!";
            return;
        }
    });

    connect(ui->pushButton_cancle, &QPushButton::clicked, [=](){
        this->close();
    });
}

SetIpDialog::~SetIpDialog()
{
    delete ui;
}
