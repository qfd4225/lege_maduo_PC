#pragma once

#include <QWidget>
#include "ui_select_func_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SelectFuncDialogClass; };
QT_END_NAMESPACE

class SelectFuncDialog : public QWidget
{
	Q_OBJECT

public:
	SelectFuncDialog(QWidget* parent = nullptr);
	~SelectFuncDialog();

private:
	Ui::SelectFuncDialogClass* ui;
};
