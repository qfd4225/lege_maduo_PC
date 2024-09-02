#include "sql_box_show_widget.h"
#include "ui_sql_box_show_widget.h"
#include <QScrollBar>
SqlBoxShowWidget::SqlBoxShowWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SqlBoxShowWidget)
{
    ui->setupUi(this);
    sql_edit_dialog_ = new SqlBoxEditDialog();
    linkTable();
    createTable(myQuery);
    connect(sql_edit_dialog_, &SqlBoxEditDialog::confirmSignal, this,
            [=](SqlBoxEditDialog::OperaMode mode, QStringList combo_info) {
                QPair<QString, BoxInfo> sku_pair;
                getPairFromComboInfo(combo_info, sku_pair);
                if (mode == SqlBoxEditDialog::AddMode) {        //增
                    insertData(myQuery, sku_pair);
                    sql_edit_dialog_->hide();
                }
                else if (mode == SqlBoxEditDialog::EditMode) {        //改
                    updateData(myQuery, sku_pair);
                    sql_edit_dialog_->hide();
                }
                else {
                    qDebug() << "Error:未设置模式!!!";
                }
                sql_edit_dialog_->setMode(SqlBoxEditDialog::ErrorMode);
                refreshData(myQuery);     // 更新
            });
    // 增
    connect(ui->pushButton_insert, &QPushButton::clicked, this, [=]() {
        sql_edit_dialog_->clearAll();
        sql_edit_dialog_->setMode(SqlBoxEditDialog::AddMode);
        sql_edit_dialog_->show();
    });
    // 查
    connect(ui->pushButton_read, &QPushButton::clicked, this, [=]() {bool ok;
        QString text = QInputDialog::getText(nullptr, "请输入", "要查询的sku码:", QLineEdit::Normal, "", &ok);

        if (ok && !text.isEmpty()) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "确认", "请确认操作!",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {                
                queryData(myQuery, text);
            }
            else {
                return;
            }
        }
    });
    // 改
    connect(ui->pushButton_edit, &QPushButton::clicked, this, [=]() {
        QList<QTableWidgetItem*> selectedItems =
            ui->tableWidget_boxes->selectedItems();
        if (selectedItems.isEmpty()) {
            sql_edit_dialog_->clearAll();
            sql_edit_dialog_->setMode(SqlBoxEditDialog::EditMode);
            sql_edit_dialog_->show();
        }
        else {
            QStringList textList;
            for (auto item : selectedItems)
            {
                QString p_text = item->text();
                textList.append(p_text);
            }

            if (!selectedItems.first()->text().isEmpty()) {
                sql_edit_dialog_->setAll(textList);
                sql_edit_dialog_->setMode(SqlBoxEditDialog::EditMode);
                sql_edit_dialog_->show();
            }
        }
    });
    //删
    connect(ui->pushButton_remove, &QPushButton::clicked, this, [=]() {
        bool ok;
        QString text = QInputDialog::getText(nullptr, "请输入", "要删除的sku码:", QLineEdit::Normal, "", &ok);

        if (ok && !text.isEmpty()) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "确认", "请确认操作!",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                bool delete_ret = deleteData(myQuery, text);
                if (!delete_ret)
                {
                    QMessageBox::warning(NULL, "提示！", "没有型号SKU!",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                    return;
                }
            }
            else {
                return;
            }
        }
        refreshData(myQuery);        // 更新
    });
    //刷新
    connect(ui->pushButton_refresh, &QPushButton::clicked, this, [=] {refreshData(myQuery); });
    setup_tableSql();
}

SqlBoxShowWidget::~SqlBoxShowWidget()
{
    delete ui;
}

void SqlBoxShowWidget::refreshSql()
{
    refreshData(myQuery);
}

QList<QStringList> SqlBoxShowWidget::getAllData() {
    QList<QStringList> result_list;
    int table_rows = getRecordCount(myQuery, "layout_boxes");
    ui->tableWidget_boxes->setRowCount(table_rows);
    if (!myQuery.exec("SELECT * FROM layout_boxes")) {
        qDebug() << "Error selecting data:" << myQuery.lastError();
    }
    else {
        while (myQuery.next()) {
            QStringList result_str_list;
            QString sku_code = myQuery.value(0).toString();
            float box_length = myQuery.value(1).toFloat();
            float box_width = myQuery.value(2).toFloat();
            float box_height = myQuery.value(3).toFloat();
            float box_weight = myQuery.value(4).toFloat();
            result_str_list<<sku_code;
            result_str_list<<QString::number(box_length);
            result_str_list<<QString::number(box_width);
            result_str_list<<QString::number(box_height);
            result_str_list<<QString::number(box_weight);
            result_list.append(result_str_list);
        }
    }
    QScrollBar * bar = ui->tableWidget_boxes->verticalScrollBar();
    bar->setValue(bar->maximum());
    return result_list;
}

QList<QStringList> SqlBoxShowWidget::getSelectedData(const QString &sku_code)
{
    QList<QStringList> resultList = queryDataList(myQuery, sku_code);
    return resultList;
}

int SqlBoxShowWidget::getSize()
{
    int table_rows = getRecordCount(myQuery, "layout_boxes");
    return table_rows;
}


void SqlBoxShowWidget::updateSql(QVector<QString> skuCodes, QVector<SingleSKUBpp::LayoutResultMsql> layoutResult, QVector<SingleSKUBpp::TrayMsql> trayResult)
{
    for(int i = 0;i<skuCodes.size();i++)
    {
        QString p_codestr = skuCodes[i];
        auto p_tray = trayResult[i];

        float box_len=p_tray.box_length;
        float box_wid=p_tray.box_width;
        float box_hei=p_tray.box_height;
        float box_wei=p_tray.box_weight;

        BoxInfo box_info = {box_len,box_wid,box_hei,box_wei};
        QPair<QString, BoxInfo> data_pair = qMakePair(p_codestr,box_info);
        insertData(myQuery,data_pair);
    }
}




void SqlBoxShowWidget::getPairFromComboInfo(QStringList combo_info, QPair<QString, BoxInfo>& data_pair)
{
    assert(combo_info.size() == 5);
    QString sku_code_str = combo_info.at(0);        // sku码
    QString length_str = combo_info.at(1);       // 层数
    QString width_str = combo_info.at(2);           // X坐标
    QString height_str = combo_info.at(3);           // Y坐标
    QString weight_str = combo_info.at(4);       // 层数

    QString Sku_Code = sku_code_str;
    float  length = length_str.toFloat();
    float  width = width_str.toFloat();
    float  height = height_str.toFloat();
    float  weight = weight_str.toFloat();

    BoxInfo box_info  = { length,width,height,weight };  // sku码

    data_pair = qMakePair(Sku_Code, box_info);
}

void SqlBoxShowWidget::setup_tableSql()
{
    ui->tableWidget_boxes->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOn);
    ui->tableWidget_boxes->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget_boxes->setColumnCount(5);  // 你可以根据需要调整行数
    QList<QString> fileList;
    fileList << "SKU码" << "货物长度" << "货物宽度" << "货物高度" << "货物重量";
    ui->tableWidget_boxes->setHorizontalHeaderLabels(fileList);
    ui->tableWidget_boxes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_boxes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_boxes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_boxes->setSelectionMode(QAbstractItemView::SingleSelection);
}

void SqlBoxShowWidget::linkTable()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "QtSql_Boxes");//数据库名称 QtSql是数据库的名称
    db.setDatabaseName("QtSql_Boxes.db");//数据库文件

    bool my_ok = db.open();
    if (!my_ok) {
        qDebug() << "打开数据库失败";
        return;
    }
    else
    {
        qDebug() << "打开数据库成功";
        myQuery = QSqlQuery(db);
        return;
    }
}

void SqlBoxShowWidget::clearTable() {
    bool my_ok = myQuery.exec("DELETE FROM layout_boxes");
    if (!my_ok) {
        qDebug() << "Error clearing table:" << myQuery.lastError();
    } else {
        qDebug() << "Table cleared successfully.";
    }
}


void SqlBoxShowWidget::createTable(QSqlQuery& query) {
    bool my_ok = query.exec("CREATE TABLE IF NOT EXISTS layout_boxes ("
                            "sku_code TEXT, "
                            "box_length REAL,"
                            "box_width REAL,"
                            "box_height REAL,"
                            "box_weight REAL)");
    if (!my_ok) {
        qDebug() << "Error creating table:" << query.lastError();
    }
}

// 插入数据
void SqlBoxShowWidget::insertData(QSqlQuery& query, const QPair<QString, BoxInfo> data_pair) {
    QString sku_code  = data_pair.first;
    auto box_data = data_pair.second;
    query.prepare("INSERT INTO layout_boxes (sku_code,box_length,box_width,box_height, box_weight) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(sku_code);
    query.addBindValue(box_data.length);
    query.addBindValue(box_data.width);
    query.addBindValue(box_data.height);
    query.addBindValue(box_data.weight);
    if (!query.exec()) {
        qDebug() << "Error inserting data:" << query.lastError();
    }
}

void SqlBoxShowWidget::updateData(QSqlQuery& query, const QPair<QString, BoxInfo> data_pair) {
    QString sku_code  = data_pair.first;
    auto box_data = data_pair.second;
    query.prepare("UPDATE layout_boxes SET box_length = ?, box_width = ?, box_height = ?, box_weight = ? "
                  "WHERE sku_code = ?");
    query.addBindValue(box_data.length);
    query.addBindValue(box_data.width);
    query.addBindValue(box_data.height);
    query.addBindValue(box_data.weight);
    query.addBindValue(sku_code);
    if (!query.exec()) {
        qDebug() << "Error updating data:" << query.lastError();
    }
}



// 删除数据
bool SqlBoxShowWidget::deleteData(QSqlQuery& query, const QString& sku_plan) {
    // 准备 SQL DELETE 语句，根据 TrayInfo 的多个字段进行删除
    query.prepare("DELETE FROM layout_boxes WHERE sku_code = ? ");

    // 绑定 TrayInfo 的值
    query.addBindValue(sku_plan);
    if (!query.exec()) {
        qDebug() << "Error deleting data:" << query.lastError();
        return false;
    }
    // 检查是否有数据被删除
    int rowsAffected = query.numRowsAffected();
    if (rowsAffected == 0) {
        qDebug() << "Error deleting data";
        return false;
    }
    return true;
}


int SqlBoxShowWidget::getRecordCount(QSqlQuery& query, const QString& tableName) {
    query.prepare(QString("SELECT COUNT(*) FROM %1").arg(tableName));
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    return -1;  // 查询失败或没有结果
}

// 查询某键值对应的数据
void SqlBoxShowWidget::queryData(QSqlQuery& query, const QString& sku_code) {
    // 准备 SQL SELECT 语句，根据 TrayInfo 的多个字段进行查询
    query.prepare("SELECT sku_code, box_length, box_width, box_height, box_weight "
                  "FROM layout_boxes "
                  "WHERE sku_code = ?");

    // 绑定 TrayInfo 的值
    query.addBindValue(sku_code);

    // 执行查询操作
    if (!query.exec()) {
        qDebug() << "Error querying data:" << query.lastError();
        return;
    }

    // 获取查询结果
    while (query.next()) {
        qDebug() << "Sku_Code:" << query.value("sku_code").toString()
                 << "box_length:" << query.value("box_length").toInt()
                 << "box_width:" << query.value("box_width").toInt()
                 << "box_height:" << query.value("box_height").toInt()
                 << "box_weight:" << query.value("box_weight").toInt();
    }
}

// 查询某键值对应的数据并返回
QList<QStringList> SqlBoxShowWidget::queryDataList(QSqlQuery& query, const QString& sku_code) {
    QList<QStringList> result_list;
    // 准备 SQL SELECT 语句，根据 TrayInfo 的多个字段进行查询
    query.prepare("SELECT sku_code, box_length, box_width, box_height, box_weight "
                  "FROM layout_boxes "
                  "WHERE sku_code = ?");

    // 绑定 TrayInfo 的值
    query.addBindValue(sku_code);

    // 执行查询操作
    if (!query.exec()) {
        qDebug() << "Error querying data:" << query.lastError();
        return result_list;
    }

    // 获取查询结果
    while (query.next()) {
        QStringList result_L;
        result_L  << query.value("sku_code").toString();
        result_L  << query.value("box_length").toString();
        result_L  << query.value("box_width").toString();
        result_L  << query.value("box_height").toString();
        result_L  << query.value("box_weight").toString();
        result_list.append(result_L);
    }
    return result_list;
}



void SqlBoxShowWidget::refreshData(QSqlQuery& query) {
    int table_rows = getRecordCount(query, "layout_boxes");
    ui->tableWidget_boxes->setRowCount(table_rows);
    if (!query.exec("SELECT * FROM layout_boxes")) {
        qDebug() << "Error selecting data:" << query.lastError();
    }
    else {
        int row_Number = 0;
        while (query.next()) {
            QString sku_code = query.value(0).toString();
            float box_length = query.value(1).toFloat();
            float box_width = query.value(2).toFloat();
            float box_height = query.value(3).toFloat();
            float box_weight = query.value(4).toFloat();

            QTableWidgetItem* item0 = new QTableWidgetItem();
            item0->setText(sku_code);
            ui->tableWidget_boxes->setItem(row_Number, 0, item0);

            QTableWidgetItem* item1 = new QTableWidgetItem();
            item1->setText(QString::number(box_length));
            ui->tableWidget_boxes->setItem(row_Number, 1, item1);

            QTableWidgetItem* item2 = new QTableWidgetItem();
            item2->setText(QString::number(box_width));
            ui->tableWidget_boxes->setItem(row_Number, 2, item2);

            QTableWidgetItem* item3 = new QTableWidgetItem();
            item3->setText(QString::number(box_height));
            ui->tableWidget_boxes->setItem(row_Number, 3, item3);

            QTableWidgetItem* item4 = new QTableWidgetItem();
            item4->setText(QString::number(box_weight));
            ui->tableWidget_boxes->setItem(row_Number, 4, item4);

            row_Number++;
        }

    }
    QScrollBar * bar = ui->tableWidget_boxes->verticalScrollBar();
    bar->setValue(bar->maximum());
}
