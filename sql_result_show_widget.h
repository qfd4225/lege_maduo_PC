#ifndef SQL_RESULT_SHOW_WIDGET_H
#define SQL_RESULT_SHOW_WIDGET_H

#include <QWidget>
#include <QtCore/QCoreApplication>
#include <QInputDialog>
#include<QMessageBox>
#include "sql_edit_dialog.h"
#include "single_SKU.h"

namespace Ui {
class SqlResultShowWidget;
}

class SqlResultShowWidget : public QWidget
{
    Q_OBJECT
public:
    struct TrayInfo
    {
        QString Sku_Code;
        int Tray_Length;
        int Tray_Width;
        int Tray_MaxHeight;
        int Tray_MaxWeight;
        int Tray_MaxNumber;
    };


    explicit SqlResultShowWidget(QWidget *parent = nullptr);
    SqlResultShowWidget(Ui::SqlResultShowWidget *ui, QSqlQuery myQuery)
        : ui(ui), myQuery(std::move(myQuery)) {}
    ~SqlResultShowWidget();
    void updateSql(QVector<QString> skuCodes , QVector<SingleSKUBpp::LayoutResultMsql> layoutResult, QVector<SingleSKUBpp::TrayMsql> trayResult);
    void refreshSql();
    int getSize();
    void update2DResult(TrayInfo tray,QVector<SingleSKUBpp::LayoutResultMsql> results);

    QList<QStringList> getSelectedData(QString Sku_Code, int Tray_Length,int Tray_Width,
                                       int Tray_MaxHeight,int Tray_MaxWeight,int Tray_MaxNumber);
    QList<QStringList> queryDataList(QSqlQuery &query, const TrayInfo &sku_plan);
    void clearTable();
private:
    Ui::SqlResultShowWidget *ui;
    QSqlQuery myQuery;

    void linkTable();
    void setup_tableSql();
    void createTable(QSqlQuery& query);
    void insertData(QSqlQuery& query, const QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql> data_pair) ;
    void updateData(QSqlQuery& query, const QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql> data_pair);
    bool deleteData(QSqlQuery& query, const TrayInfo& sku_plan);
    void queryData(QSqlQuery& query, const TrayInfo& sku_plan);
    void refreshData(QSqlQuery& query);
    int getRecordCount(QSqlQuery& query, const QString& tableName);
    void getPairFromComboInfo(QStringList combo_info, QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql>& data_pair);
    bool deleteDataFloor(QSqlQuery &query, const TrayInfo &sku_plan,int floor_num);
};

#endif // SQL_RESULT_SHOW_WIDGET_H
