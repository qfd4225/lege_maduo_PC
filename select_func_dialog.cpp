#include "select_func_dialog.h"

SelectFuncDialog::SelectFuncDialog(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::SelectFuncDialogClass())
{
	ui->setupUi(this);

}

SelectFuncDialog::~SelectFuncDialog()
{
	delete ui;
}
