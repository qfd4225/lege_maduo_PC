#ifndef SQL_SHOW_WIDGET_H
#define SQL_SHOW_WIDGET_H
#include <QWidget>
#include "sql_result_show_widget.h"
#include "sql_box_show_widget.h"
namespace Ui {
	class sqlShowWidget;
}

class sqlShowWidget : public QWidget
{
	Q_OBJECT

public:
	explicit sqlShowWidget(QWidget* parent = nullptr);
	~sqlShowWidget();
    SqlBoxShowWidget* box_show_widget = nullptr;
    SqlResultShowWidget* box_sql_widget = nullptr;
    void getSelectedData(QString Sku_Code,int Tray_Length,int Tray_Width,int Tray_MaxHeight,int Tray_MaxWeight,int Tray_MaxNumber,QList<QStringList> &result_list, QList<QStringList> &box_list);
signals:
	void returnToMain();
	void returnToModeChoose();

public slots:
    void pluginDataSlot(QVector<QString> skuCodes , QVector<SingleSKUBpp::LayoutResultMsql> layoutResult, QVector<SingleSKUBpp::TrayMsql> trayResult);
private:
	Ui::sqlShowWidget* ui;


};
#endif // SQL_SHOW_WIDGET_H
