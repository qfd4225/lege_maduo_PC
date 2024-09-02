#ifndef SET_IP_DIALOG_H
#define SET_IP_DIALOG_H

#include <QDialog>

namespace Ui {
class SetIpDialog;
}

class SetIpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetIpDialog(QWidget *parent = nullptr);
    ~SetIpDialog();
signals:
    void confirmClicked(QString ip_config);

private:
    Ui::SetIpDialog *ui;
};

#endif // SET_IP_DIALOG_H
