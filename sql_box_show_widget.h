#ifndef SQL_BOX_SHOW_WIDGET_H
#define SQL_BOX_SHOW_WIDGET_H

#include <QWidget>
#include <QtCore/QCoreApplication>
#include <QInputDialog>
#include<QMessageBox>
#include "sql_box_edit_dialog.h"
#include "single_SKU.h"

namespace Ui {
class SqlBoxShowWidget;
}

class SqlBoxShowWidget : public QWidget
{
    Q_OBJECT

    struct BoxInfo {
        float length;
        float width;
        float height;
        float weight;
    };
public:
    explicit SqlBoxShowWidget(QWidget *parent = nullptr);
    SqlBoxShowWidget(Ui::SqlBoxShowWidget *ui, QSqlQuery myQuery)
        : ui(ui), myQuery(std::move(myQuery)) {}
    ~SqlBoxShowWidget();

    void refreshSql();
    void updateSql(QVector<QString> skuCodes , QVector<SingleSKUBpp::LayoutResultMsql> layoutResult, QVector<SingleSKUBpp::TrayMsql> trayResult);
    int getSize();
    QList<QStringList> getAllData();
    QList<QStringList> getSelectedData(const QString& sku_code);
    void clearTable();
private:
    Ui::SqlBoxShowWidget *ui;
    QSqlQuery myQuery;
    SqlBoxEditDialog* sql_edit_dialog_ = nullptr;

    void linkTable();
    void setup_tableSql();
    void createTable(QSqlQuery& query);
    void insertData(QSqlQuery& query, const QPair<QString, BoxInfo> data_pair);
    void updateData(QSqlQuery& query, const QPair<QString, BoxInfo> data_pair);
    bool deleteData(QSqlQuery& query, const QString& sku_plan);
    void queryData(QSqlQuery& query, const QString& sku_code);
    void refreshData(QSqlQuery& query);
    int getRecordCount(QSqlQuery& query, const QString& tableName);
    void getPairFromComboInfo(QStringList combo_info, QPair<QString, BoxInfo>& data_pair);

    QList<QStringList> queryDataList(QSqlQuery &query, const QString &sku_code);
};

#endif // SQL_BOX_SHOW_WIDGET_H
