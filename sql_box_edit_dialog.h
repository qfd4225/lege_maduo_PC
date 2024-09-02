#ifndef SQL_BOX_EDIT_DIALOG_H
#define SQL_BOX_EDIT_DIALOG_H

#include <QWidget>

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class SqlBoxEditDialog;
}



class SqlBoxEditDialog : public QWidget
{
    Q_OBJECT
    public:
    enum OperaMode {
        ErrorMode = -1,
        AddMode = 0,
        EditMode = 1
    };


public:
    explicit SqlBoxEditDialog(QWidget *parent = nullptr);
    ~SqlBoxEditDialog();
    void setMode(OperaMode mode) { opera_mode = mode; }
    void setAll(QStringList combo_info);
    void clearAll();

private:
    Ui::SqlBoxEditDialog *ui;
    OperaMode opera_mode = ErrorMode;

signals:
    void confirmSignal(OperaMode mode, QStringList combo_info);

};

#endif // SQL_BOX_EDIT_DIALOG_H
