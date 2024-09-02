#include "sql_show_widget.h"
#include "ui_sql_show_widget.h"



sqlShowWidget::sqlShowWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::sqlShowWidget)
{
	ui->setupUi(this);
	// 登出
	connect(ui->pushButton_logout, &QPushButton::clicked, this, &sqlShowWidget::returnToMain);
	// 返回模式选择
	connect(ui->pushButton_return, &QPushButton::clicked, this, &sqlShowWidget::returnToModeChoose);

    box_sql_widget = new SqlResultShowWidget(this);
    QVBoxLayout* layout1 = new QVBoxLayout(ui->widget_sql_result);
    layout1->addWidget(box_sql_widget);
    ui->widget_sql_result->setLayout(layout1);
    // box_sql_widget->hide();

    box_show_widget = new SqlBoxShowWidget(this);
    QVBoxLayout* layout2 = new QVBoxLayout(ui->widget_box_size);
    layout2->addWidget(box_show_widget);
    ui->widget_box_size->setLayout(layout2);
    // box_show_widget->hide();

    // 刷新数据
    connect(ui->pushButton_refresh, &QPushButton::clicked, this, [=](){
        if(box_show_widget && box_sql_widget)
        {
            box_show_widget->refreshSql();
            box_sql_widget->refreshSql();
            int result_size = box_sql_widget->getSize();
            int box_size = box_show_widget->getSize();
            qDebug()<<"result_size为:"<<result_size;
            qDebug()<<"box_size为:"<<box_size;
        }
    });
    // 清空数据
    connect(ui->pushButton_clearAll, &QPushButton::clicked, this, [=](){
        if(box_show_widget && box_sql_widget)
        {
            box_show_widget->clearTable();
            box_sql_widget->clearTable();
        }
    });

    ui->pushButton_refresh->show();
}
sqlShowWidget::~sqlShowWidget()
{
	delete ui;
}

void sqlShowWidget::pluginDataSlot(QVector<QString> skuCodes, QVector<SingleSKUBpp::LayoutResultMsql> layoutResult, QVector<SingleSKUBpp::TrayMsql> trayResult)
{
	assert(layoutResult.size() == trayResult.size() && trayResult.size() == skuCodes.size());
    if(box_show_widget && box_sql_widget)
    {
        box_sql_widget->updateSql(skuCodes, layoutResult, trayResult);
        box_show_widget->updateSql(skuCodes,layoutResult,trayResult);
	}
}



void sqlShowWidget::getSelectedData(QString Sku_Code,int Tray_Length,int Tray_Width,int Tray_MaxHeight,int Tray_MaxWeight,int Tray_MaxNumber,QList<QStringList> &result_list, QList<QStringList> &box_list)
{
    if(box_show_widget && box_sql_widget)
    {
        result_list =  box_sql_widget->getSelectedData(Sku_Code,Tray_Length,Tray_Width,Tray_MaxHeight,Tray_MaxWeight,Tray_MaxNumber);
        box_list = box_show_widget->getSelectedData(Sku_Code);
    }
}
