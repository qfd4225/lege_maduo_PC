#include "sql_edit_dialog.h"
#include "ui_sql_edit_dialog.h"
#include <QMessageBox>

SqlEditDialog::SqlEditDialog(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::SqlEditDialog)
{
	ui->setupUi(this);
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=]()
        {
			auto sku_code_str = ui->lineEdit_SkuCode->text();
			auto floor_num_str = ui->spinBox_FloorNum->text();
			auto x_pos_str = ui->doubleSpinBox_XPos->text();
			auto y_pos_str = ui->doubleSpinBox_YPos->text();
			auto z_pos_str = ui->doubleSpinBox_ZPos->text();
			auto box_angle_str = ui->spinBox_Angel->text();
			auto box_direction_str = ui->spinBox_Direction->text();
			auto TrayLength = ui->spinBox_TrayLength->text();
			auto TrayWidth = ui->spinBox_TrayWidth->text();
			auto TrayHeight = ui->spinBox_TrayHeight->text();

			if (sku_code_str.isEmpty() || floor_num_str.isEmpty() || x_pos_str.isEmpty() || y_pos_str.isEmpty() || z_pos_str.isEmpty() ||
				box_angle_str.isEmpty() || box_direction_str.isEmpty() || TrayLength.isEmpty() || TrayWidth.isEmpty() || TrayHeight.isEmpty()) {
				QMessageBox::warning(NULL, "提示！", "尚未填写完成!",
					QMessageBox::Yes | QMessageBox::No,
					QMessageBox::Yes);
				return;
			}

			QStringList combo_info;
			combo_info << sku_code_str << TrayLength << TrayWidth << TrayHeight << floor_num_str << x_pos_str << y_pos_str << z_pos_str << box_angle_str << box_direction_str;
			emit confirmSignal(opera_mode, combo_info);
		});
}
void SqlEditDialog::setAll(QStringList combo_info)
{
	ui->lineEdit_SkuCode->setEnabled(false);
	ui->spinBox_TrayLength->setEnabled(false);
	ui->spinBox_TrayWidth->setEnabled(false);
	ui->spinBox_TrayHeight->setEnabled(false);

	ui->lineEdit_SkuCode->setText(combo_info[0]);

	ui->spinBox_TrayLength->setValue(combo_info[1].toInt());
	ui->spinBox_TrayWidth->setValue(combo_info[2].toInt());
	ui->spinBox_TrayHeight->setValue(combo_info[3].toInt());

	ui->spinBox_FloorNum->setValue(combo_info[1 + 3].toInt());
	ui->doubleSpinBox_XPos->setValue(combo_info[2 + 3].toDouble());
	ui->doubleSpinBox_YPos->setValue(combo_info[3 + 3].toDouble());
	ui->doubleSpinBox_ZPos->setValue(combo_info[4 + 3].toDouble());
	ui->spinBox_Angel->setValue(combo_info[5 + 3].toInt());
	ui->spinBox_Direction->setValue(combo_info[6 + 3].toInt());
}

void SqlEditDialog::clearAll()
{
	ui->lineEdit_SkuCode->setEnabled(true);
	ui->spinBox_TrayLength->setEnabled(true);
	ui->spinBox_TrayWidth->setEnabled(true);
	ui->spinBox_TrayHeight->setEnabled(true);
	ui->spinBox_TrayLength->setValue(0);
	ui->spinBox_TrayWidth->setValue(0);
	ui->spinBox_TrayHeight->setValue(0);
	ui->lineEdit_SkuCode->setText("");
	ui->spinBox_FloorNum->setValue(0);
	ui->doubleSpinBox_XPos->setValue(0.00);
	ui->doubleSpinBox_YPos->setValue(0.00);
	ui->doubleSpinBox_ZPos->setValue(0.00);
	ui->spinBox_Angel->setValue(0);
	ui->spinBox_Direction->setValue(0);
}


SqlEditDialog::~SqlEditDialog()
{
	delete ui;
}

