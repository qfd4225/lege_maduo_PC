#ifndef SQL_KEY_EDIT_DIALOG_H
#define SQL_KEY_EDIT_DIALOG_H

#include <QDialog>

namespace Ui {
class sqlKeyEditDialog;
}

class sqlKeyEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sqlKeyEditDialog(QWidget *parent = nullptr);
    ~sqlKeyEditDialog();
    void clearForm();
private:
    Ui::sqlKeyEditDialog *ui;


signals:
    void confirmSignal(QStringList combo_info);
};

#endif // SQL_KEY_EDIT_DIALOG_H





