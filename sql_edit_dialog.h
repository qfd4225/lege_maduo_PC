#ifndef SQL_EDIT_DIALOG_H
#define SQL_EDIT_DIALOG_H

#include <QWidget>

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
namespace Ui {
	class SqlEditDialog;
}

enum OperaMode {
	ErrorMode = -1,
	AddMode = 0,
	EditMode = 1
};


class SqlEditDialog : public QWidget
{
	Q_OBJECT

public:
	explicit SqlEditDialog(QWidget* parent = nullptr);
	~SqlEditDialog();
	void setMode(OperaMode mode) { opera_mode = mode; }
	void setAll(QStringList combo_info);
	void clearAll();
signals:
	void confirmSignal(OperaMode mode, QStringList combo_info);
private:
	Ui::SqlEditDialog* ui;
	OperaMode opera_mode = ErrorMode;
};

#endif // SQL_EDIT_DIALOG_H
