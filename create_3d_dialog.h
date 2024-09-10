#ifndef CREATE_3D_DIALOG_H
#define CREATE_3D_DIALOG_H

#include <QDialog>

namespace Ui {
class create3dDialog;
}

class create3dDialog : public QDialog
{
    Q_OBJECT

public:
    explicit create3dDialog(QWidget *parent = nullptr);
    ~create3dDialog();
signals:
       void confirmSignal(QStringList combo);
private:
    Ui::create3dDialog *ui;
};

#endif // CREATE_3D_DIALOG_H
