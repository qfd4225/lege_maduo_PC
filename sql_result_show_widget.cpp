#include "sql_result_show_widget.h"
#include "ui_sql_result_show_widget.h"
#include<QScrollBar>
SqlResultShowWidget::SqlResultShowWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::SqlResultShowWidget)
{
    ui->setupUi(this);
	linkTable();
	createTable(myQuery);

	// 增
	connect(ui->pushButton_insert, &QPushButton::clicked, this, [=]() {
        bool ok1, ok2, ok3, ok4,ok5,ok6;
        QString text1 = QInputDialog::getText(nullptr, "请输入", "要增添的sku码:", QLineEdit::Normal, "", &ok1);
        QString text2 = QInputDialog::getText(nullptr, "请输入", "要增添的栈板长度:", QLineEdit::Normal, "", &ok2);
        QString text3 = QInputDialog::getText(nullptr, "请输入", "要增添的栈板宽度:", QLineEdit::Normal, "", &ok3);
        QString text4 = QInputDialog::getText(nullptr, "请输入", "要增添的栈板最大高度:", QLineEdit::Normal, "", &ok4);
        QString text5 = QInputDialog::getText(nullptr, "请输入", "要增添的栈板最大载重:", QLineEdit::Normal, "", &ok5);
        QString text6 = QInputDialog::getText(nullptr, "请输入", "要增添的栈板最大载货数量:", QLineEdit::Normal, "", &ok6);

        if (ok1 && ok2 && ok3 && ok4 &&ok5&&ok6&& !text1.isEmpty() && !text2.isEmpty() && !text3.isEmpty() && !text4.isEmpty()&& !text5.isEmpty()&& !text6.isEmpty()) {

            QString textA1 = QInputDialog::getText(nullptr, "请输入", "floor_num:", QLineEdit::Normal, "", &ok1);
            QString textA2 = QInputDialog::getText(nullptr, "请输入", "x:", QLineEdit::Normal, "", &ok2);
            QString textA3 = QInputDialog::getText(nullptr, "请输入", "y:", QLineEdit::Normal, "", &ok3);
            QString textA4 = QInputDialog::getText(nullptr, "请输入", "z:", QLineEdit::Normal, "", &ok4);
            QString textA5 = QInputDialog::getText(nullptr, "请输入", "box_ori:", QLineEdit::Normal, "", &ok5);
            QString textA6 = QInputDialog::getText(nullptr, "请输入", "enter_dir:", QLineEdit::Normal, "", &ok6);
            if (ok1 && ok2 && ok3 && ok4 &&ok5&&ok6&& !textA1.isEmpty() && !textA2.isEmpty() && !textA3.isEmpty() && !textA4.isEmpty()&& !textA5.isEmpty()&& !textA6.isEmpty()) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "确认", "请确认操作!",
                                              QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    TrayInfo addkey = TrayInfo{ text1,
                                               text2.toInt(),
                                               text3.toInt(),
                                               text4.toInt(),
                                               text5.toInt(),
                                               text6.toInt()};
                    SingleSKUBpp::LayoutResultMsql addValue= SingleSKUBpp::LayoutResultMsql{
                        textA1.toInt(),
                        textA2.toDouble(),
                        textA3.toDouble(),
                        textA4.toDouble(),
                        textA5.toInt(),
                        textA6.toInt()
                    };
                    QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql> data_pair1 = qMakePair(addkey,addValue);
                    insertData(myQuery, data_pair1);
                           refreshData(myQuery);        // 更新
                }
                else{return;}
            }
            else{return;}
        }

		});
	// 查
    connect(ui->pushButton_read, &QPushButton::clicked, this, [=]() {
        bool ok1, ok2, ok3, ok4,ok5,ok6;
        QString text1 = QInputDialog::getText(nullptr, "请输入", "要读取的sku码:", QLineEdit::Normal, "", &ok1);
        QString text2 = QInputDialog::getText(nullptr, "请输入", "要读取的栈板长度:", QLineEdit::Normal, "", &ok2);
        QString text3 = QInputDialog::getText(nullptr, "请输入", "要读取的栈板宽度:", QLineEdit::Normal, "", &ok3);
        QString text4 = QInputDialog::getText(nullptr, "请输入", "要读取的栈板最大高度:", QLineEdit::Normal, "", &ok4);
        QString text5 = QInputDialog::getText(nullptr, "请输入", "要读取的栈板最大载重:", QLineEdit::Normal, "", &ok5);
        QString text6 = QInputDialog::getText(nullptr, "请输入", "要读取的栈板最大载货数量:", QLineEdit::Normal, "", &ok6);


        if (ok1 && ok2 && ok3 && ok4 &&ok5&&ok6&& !text1.isEmpty() && !text2.isEmpty() && !text3.isEmpty() && !text4.isEmpty()&& !text5.isEmpty()&& !text6.isEmpty()) {
        QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "确认", "请确认操作!",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
            TrayInfo findkey = TrayInfo{ text1,
                                          text2.toInt(),
                                          text3.toInt(),
                                          text4.toInt(),
                                          text5.toInt(),
                                          text6.toInt()};

            queryData(myQuery, findkey);
		}
		else {
			return;
		}
	}
		});
	// 改
    connect(ui->pushButton_edit, &QPushButton::clicked, this, [=]() {
        bool ok1, ok2, ok3, ok4,ok5,ok6;
        QString text1 = QInputDialog::getText(nullptr, "请输入", "要修改的sku码:", QLineEdit::Normal, "", &ok1);
        QString text2 = QInputDialog::getText(nullptr, "请输入", "要修改的栈板长度:", QLineEdit::Normal, "", &ok2);
        QString text3 = QInputDialog::getText(nullptr, "请输入", "要修改的栈板宽度:", QLineEdit::Normal, "", &ok3);
        QString text4 = QInputDialog::getText(nullptr, "请输入", "要修改的栈板最大高度:", QLineEdit::Normal, "", &ok4);
        QString text5 = QInputDialog::getText(nullptr, "请输入", "要修改的栈板最大载重:", QLineEdit::Normal, "", &ok5);
        QString text6 = QInputDialog::getText(nullptr, "请输入", "要修改的栈板最大载货数量:", QLineEdit::Normal, "", &ok6);

        if (ok1 && ok2 && ok3 && ok4 &&ok5&&ok6&& !text1.isEmpty() && !text2.isEmpty() && !text3.isEmpty() && !text4.isEmpty()&& !text5.isEmpty()&& !text6.isEmpty()) {

            QString textA1 = QInputDialog::getText(nullptr, "请输入", "floor_num:", QLineEdit::Normal, "", &ok1);
            QString textA2 = QInputDialog::getText(nullptr, "请输入", "x:", QLineEdit::Normal, "", &ok2);
            QString textA3 = QInputDialog::getText(nullptr, "请输入", "y:", QLineEdit::Normal, "", &ok3);
            QString textA4 = QInputDialog::getText(nullptr, "请输入", "z:", QLineEdit::Normal, "", &ok4);
            QString textA5 = QInputDialog::getText(nullptr, "请输入", "box_ori:", QLineEdit::Normal, "", &ok5);
            QString textA6 = QInputDialog::getText(nullptr, "请输入", "enter_dir:", QLineEdit::Normal, "", &ok6);
            if (ok1 && ok2 && ok3 && ok4 &&ok5&&ok6&& !textA1.isEmpty() && !textA2.isEmpty() && !textA3.isEmpty() && !textA4.isEmpty()&& !textA5.isEmpty()&& !textA6.isEmpty()) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "确认", "请确认操作!",
                                              QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    TrayInfo addkey = TrayInfo{ text1,
                                               text2.toInt(),
                                               text3.toInt(),
                                               text4.toInt(),
                                               text5.toInt(),
                                               text6.toInt()};
                    SingleSKUBpp::LayoutResultMsql addValue= SingleSKUBpp::LayoutResultMsql{
                        textA1.toInt(),
                        textA2.toDouble(),
                        textA3.toDouble(),
                        textA4.toDouble(),
                        textA5.toInt(),
                        textA6.toInt()
                    };
                    QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql> data_pair1 = qMakePair(addkey,addValue);
                    updateData(myQuery, data_pair1);
                    refreshData(myQuery);        // 更新
                }
                else{return;}
            }
            else{return;}
        }
		});
	//删
	connect(ui->pushButton_remove, &QPushButton::clicked, this, [=]() {
        bool ok1, ok2, ok3, ok4,ok5,ok6;
		QString text1 = QInputDialog::getText(nullptr, "请输入", "要删除的sku码:", QLineEdit::Normal, "", &ok1);
		QString text2 = QInputDialog::getText(nullptr, "请输入", "要删除的栈板长度:", QLineEdit::Normal, "", &ok2);
		QString text3 = QInputDialog::getText(nullptr, "请输入", "要删除的栈板宽度:", QLineEdit::Normal, "", &ok3);
        QString text4 = QInputDialog::getText(nullptr, "请输入", "要删除的栈板最大高度:", QLineEdit::Normal, "", &ok4);
        QString text5 = QInputDialog::getText(nullptr, "请输入", "要删除的栈板最大载重:", QLineEdit::Normal, "", &ok5);
        QString text6 = QInputDialog::getText(nullptr, "请输入", "要删除的栈板最大载货数量:", QLineEdit::Normal, "", &ok6);


        if (ok1 && ok2 && ok3 && ok4 &&ok5&&ok6&& !text1.isEmpty() && !text2.isEmpty() && !text3.isEmpty() && !text4.isEmpty()&& !text5.isEmpty()&& !text6.isEmpty()) {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, "确认", "请确认操作!",
				QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) {
				TrayInfo deleteKey = TrayInfo{ text1,
                                                text2.toInt(),
                                                text3.toInt(),
                                                text4.toInt(),
                                              text5.toInt(),
                                              text6.toInt()};

				bool delete_ret = deleteData(myQuery, deleteKey);
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

SqlResultShowWidget::~SqlResultShowWidget()
{
	delete ui;
}

void SqlResultShowWidget::updateSql(QVector<QString> skuCodes, QVector<SingleSKUBpp::LayoutResultMsql> layoutResult, QVector<SingleSKUBpp::TrayMsql> trayResult)
{
	for (int i = 0; i < skuCodes.size(); i++)
	{
		QString p_codestr = skuCodes[i];
		auto p_tray = trayResult[i];
		auto p_layout = layoutResult[i];

        float tray_len = p_tray.tray_length;
        float tray_wid = p_tray.tray_width;
        float tray_Maxhei = p_tray.tray_MaxHeight;
        float tray_MaxWei = p_tray.tray_MaxWeight;
        float tray_MaxNum = p_tray.tray_MaxNumber;

        TrayInfo tray_info = { p_codestr, (int)tray_len,(int)tray_wid,(int)tray_Maxhei,(int)tray_MaxWei,(int)tray_MaxNum};
		QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql> data_pair = qMakePair(tray_info, p_layout);
		insertData(myQuery, data_pair);
	}
}

void SqlResultShowWidget::refreshSql()
{
	refreshData(myQuery);
}

int SqlResultShowWidget::getSize()
{
    int table_rows = getRecordCount(myQuery, "layout_results");
    return table_rows;
}

void SqlResultShowWidget::update2DResult(TrayInfo tray, QVector<SingleSKUBpp::LayoutResultMsql> results)
{
    TrayInfo deleteKey = tray;
    deleteDataFloor(myQuery, deleteKey,results[0].floor_num);

    TrayInfo updatekey = tray;
    for(auto result :results)
    {
        QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql> data_pair = qMakePair(updatekey,result);
        insertData(myQuery,data_pair);
    }
    refreshData(myQuery);
}

void SqlResultShowWidget::getPairFromComboInfo(QStringList combo_info, QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql>& data_pair)
{
	assert(combo_info.size() == 10);
	QString sku_code_str = combo_info.at(0);        // sku码
	QString TrayLength_str = combo_info.at(1);       // 层数
	QString TrayWidth_str = combo_info.at(2);           // X坐标
	QString TrayHeight_str = combo_info.at(3);           // Y坐标
	QString floor_num_str = combo_info.at(4);       // 层数
	QString x_pos_str = combo_info.at(5);           // X坐标
	QString y_pos_str = combo_info.at(6);           // Y坐标
	QString z_pos_str = combo_info.at(7);           // Z坐标
	QString box_angle_str = combo_info.at(8);       // 箱子朝向
	QString box_direction_str = combo_info.at(9);   // 箱子进入方向

	QString Sku_Code = sku_code_str;
	int  Tray_Length = TrayLength_str.toInt();
	int  Tray_Width = TrayWidth_str.toInt();
	int  Tray_Height = TrayHeight_str.toDouble();

	TrayInfo sku_code = { Sku_Code,Tray_Length,Tray_Width,Tray_Height };  // sku码
	int floor_num = floor_num_str.toInt();  // 层数
	double x_pos = x_pos_str.toDouble();   // X坐标
	double y_pos = y_pos_str.toDouble();   // Y坐标
	double z_pos = z_pos_str.toDouble(); // Z坐标
	int box_ori = box_angle_str.toInt(); // 箱子朝向
	int enter_dir = box_direction_str.toInt();   // 箱子进入方向

	SingleSKUBpp::LayoutResultMsql sku_node = { floor_num, x_pos, y_pos, z_pos, box_ori, enter_dir };
	data_pair = qMakePair(sku_code, sku_node);
}

void SqlResultShowWidget::setup_tableSql()
{
    ui->tableWidget_results->setColumnCount(12);  // 你可以根据需要调整行数
	QList<QString> fileList;
    fileList << "SKU码" << "托盘长度" << "托盘宽度" << "托盘最大高度"<<"托盘最大载重"<<"托盘最大数量" << "层数" << "X" << "Y" << "Z" << "Box_Ori" << "Enter_Dir";
    ui->tableWidget_results->setHorizontalHeaderLabels(fileList);
	ui->tableWidget_results->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableWidget_results->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget_results->setSelectionMode(QAbstractItemView::SingleSelection);

	for (int i = 0; i < ui->tableWidget_results->columnCount(); ++i) {
		ui->tableWidget_results->setColumnWidth(i, 60);
	}
}

void SqlResultShowWidget::linkTable()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "QtSql_Results");//数据库名称 QtSql是数据库的名称
	db.setDatabaseName("QtSql_Results.db");//数据库文件

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

void SqlResultShowWidget::createTable(QSqlQuery& query) {
	bool my_ok = query.exec("CREATE TABLE IF NOT EXISTS layout_results ("
		"sku_code TEXT, "
		"tray_length INTEGER,"
		"tray_width INTEGER,"
        "tray_MaxHeight INTEGER,"
        "tray_MaxWeight INTEGER,"
        "tray_MaxNumber INTEGER,"
		"floor_num INTEGER, "
		"x REAL, "
		"y REAL, "
		"z REAL, "
		"box_ori INTEGER, "
		"enter_dir INTEGER)");
	if (!my_ok) {
		qDebug() << "Error creating table:" << query.lastError();
	}
}

// 插入数据
void SqlResultShowWidget::insertData(QSqlQuery& query, const QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql> data_pair)
    {
        const TrayInfo& sku_plan = data_pair.first;
        const SingleSKUBpp::LayoutResultMsql& data = data_pair.second;
        query.prepare("INSERT INTO layout_results (sku_code,tray_length,tray_width,tray_MaxHeight,tray_MaxWeight,tray_MaxNumber,floor_num, x, y, z, box_ori, enter_dir) "
                      "VALUES (?, ?, ?, ?, ?, ?, ? , ? , ? , ?,?,?)");
        query.addBindValue(sku_plan.Sku_Code);
        query.addBindValue(sku_plan.Tray_Length);
        query.addBindValue(sku_plan.Tray_Width);
        query.addBindValue(sku_plan.Tray_MaxHeight);
        query.addBindValue(sku_plan.Tray_MaxWeight);
        query.addBindValue(sku_plan.Tray_MaxNumber);
        query.addBindValue(data.floor_num);
        query.addBindValue(data.x);
        query.addBindValue(data.y);
        query.addBindValue(data.z);
        query.addBindValue(data.box_ori);
        query.addBindValue(data.enter_dir);
        if (!query.exec()) {
            qDebug() << "4124";
            qDebug() << "Error inserting data:" << query.lastError();
        }
    }

void SqlResultShowWidget::updateData(QSqlQuery& query, const QPair<TrayInfo, SingleSKUBpp::LayoutResultMsql> data_pair) {
	TrayInfo sku_plan = data_pair.first;
	QString sku_code = sku_plan.Sku_Code;
    int tray_MaxHeight = sku_plan.Tray_MaxHeight;
	int tray_Length = sku_plan.Tray_Length;
    int tray_Width = sku_plan.Tray_Width;
    int tray_MaxNumber = sku_plan.Tray_MaxNumber;
    int tray_MaxWeight = sku_plan.Tray_MaxWeight;
	const SingleSKUBpp::LayoutResultMsql& data = data_pair.second;
	query.prepare("UPDATE layout_results SET floor_num = ?, x = ?, y = ?, z = ?, box_ori = ?, enter_dir = ? "
        "WHERE sku_code = ? AND tray_length = ? AND tray_width = ? AND tray_MaxHeight = ?  AND tray_MaxWeight = ?  AND tray_MaxNumber = ?");
	query.addBindValue(data.floor_num);
	query.addBindValue(data.x);
	query.addBindValue(data.y);
	query.addBindValue(data.z);
	query.addBindValue(data.box_ori);
	query.addBindValue(data.enter_dir);
	query.addBindValue(sku_code);
	query.addBindValue(tray_Length);
	query.addBindValue(tray_Width);
    query.addBindValue(tray_MaxHeight);
    query.addBindValue(tray_MaxWeight);
    query.addBindValue(tray_MaxNumber);
	if (!query.exec()) {
		qDebug() << "Error updating data:" << query.lastError();
	}
}

// 删除数据
bool SqlResultShowWidget::deleteDataFloor(QSqlQuery& query, const TrayInfo& sku_plan,int floor_num) {
    // 准备 SQL DELETE 语句，根据 TrayInfo 的多个字段进行删除
    query.prepare("DELETE FROM layout_results WHERE sku_code = ? AND tray_length = ? AND tray_width = ? AND tray_MaxHeight = ?  AND tray_MaxWeight = ?  AND tray_MaxNumber = ? AND floor_num = ?");

    // 绑定 TrayInfo 的值
    query.addBindValue(sku_plan.Sku_Code);
    query.addBindValue(sku_plan.Tray_Length);
    query.addBindValue(sku_plan.Tray_Width);
    query.addBindValue(sku_plan.Tray_MaxHeight);
    query.addBindValue(sku_plan.Tray_MaxWeight);
    query.addBindValue(sku_plan.Tray_MaxNumber);
    query.addBindValue(floor_num);
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


// 删除数据
bool SqlResultShowWidget::deleteData(QSqlQuery& query, const TrayInfo& sku_plan) {
	// 准备 SQL DELETE 语句，根据 TrayInfo 的多个字段进行删除
    query.prepare("DELETE FROM layout_results WHERE sku_code = ? AND tray_length = ? AND tray_width = ? AND tray_MaxHeight = ?  AND tray_MaxWeight = ?  AND tray_MaxNumber = ?");

	// 绑定 TrayInfo 的值
	query.addBindValue(sku_plan.Sku_Code);
	query.addBindValue(sku_plan.Tray_Length);
    query.addBindValue(sku_plan.Tray_Width);
    query.addBindValue(sku_plan.Tray_MaxHeight);
    query.addBindValue(sku_plan.Tray_MaxWeight);
    query.addBindValue(sku_plan.Tray_MaxNumber);

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

int SqlResultShowWidget::getRecordCount(QSqlQuery& query, const QString& tableName) {
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
void SqlResultShowWidget::queryData(QSqlQuery& query, const TrayInfo& sku_plan) {
	// 准备 SQL SELECT 语句，根据 TrayInfo 的多个字段进行查询
    query.prepare("SELECT sku_code, tray_length, tray_width, tray_MaxHeight,tray_MaxWeight,tray_MaxNumber, floor_num, x, y, z, box_ori, enter_dir "
		"FROM layout_results "
        "WHERE sku_code = ? AND tray_length = ? AND tray_width = ? AND tray_MaxHeight = ?  AND tray_MaxWeight = ?  AND tray_MaxNumber = ?");

	// 绑定 TrayInfo 的值
	query.addBindValue(sku_plan.Sku_Code);
    query.addBindValue(sku_plan.Tray_Length);
    query.addBindValue(sku_plan.Tray_Width);
    query.addBindValue(sku_plan.Tray_MaxHeight);
    query.addBindValue(sku_plan.Tray_MaxWeight);
    query.addBindValue(sku_plan.Tray_MaxNumber);

	// 执行查询操作
	if (!query.exec()) {
		qDebug() << "Error querying data:" << query.lastError();
		return;
	}

	// 获取查询结果
	while (query.next()) {
		qDebug() << "Sku_Code:" << query.value("sku_code").toString()
			<< "Tray_Length:" << query.value("tray_length").toInt()
			<< "Tray_Width:" << query.value("tray_width").toInt()
            << "tray_MaxHeight:" << query.value("tray_MaxHeight").toInt()
             << "tray_MaxWeight:" << query.value("tray_MaxWeight").toInt()
             << "tray_MaxNumber:" << query.value("tray_MaxNumber").toInt()
			<< "Floor_Num:" << query.value("floor_num").toInt()
			<< "X:" << query.value("x").toReal()
			<< "Y:" << query.value("y").toReal()
			<< "Z:" << query.value("z").toReal()
			<< "Box_Ori:" << query.value("box_ori").toInt()
			<< "Enter_Dir:" << query.value("enter_dir").toInt();
	}
}


// 查询某键值对应的数据并返回
QList<QStringList> SqlResultShowWidget::queryDataList(QSqlQuery& query, const TrayInfo& sku_plan) {
    QList<QStringList> result_list;
    // 准备 SQL SELECT 语句，根据 TrayInfo 的多个字段进行查询
    query.prepare("SELECT sku_code, tray_length, tray_width, tray_MaxHeight,tray_MaxWeight,tray_MaxNumber, floor_num, x, y, z, box_ori, enter_dir "
                  "FROM layout_results "
                  "WHERE sku_code = ? AND tray_length = ? AND tray_width = ? AND tray_MaxHeight = ?  AND tray_MaxWeight = ?  AND tray_MaxNumber = ?");

    // 绑定 TrayInfo 的值
    query.addBindValue(sku_plan.Sku_Code);
    query.addBindValue(sku_plan.Tray_Length);
    query.addBindValue(sku_plan.Tray_Width);
    query.addBindValue(sku_plan.Tray_MaxHeight);
    query.addBindValue(sku_plan.Tray_MaxWeight);
    query.addBindValue(sku_plan.Tray_MaxNumber);

    // 执行查询操作
    if (!query.exec()) {
        qDebug() << "Error querying data:" << query.lastError();
        return result_list;
    }

    // 获取查询结果
    while (query.next()) {
    QStringList resultL;
    resultL << query.value("sku_code").toString()
            << query.value("tray_length").toString()
                << query.value("tray_width").toString()
                << query.value("tray_MaxHeight").toString()
                << query.value("tray_MaxWeight").toString()
                << query.value("tray_MaxNumber").toString()
                << query.value("floor_num").toString()
                <<query.value("x").toString()
                <<query.value("y").toString()
                <<query.value("z").toString()
                << query.value("box_ori").toString()
                << query.value("enter_dir").toString();
    result_list.append(resultL);
    }
    return result_list;
}


void SqlResultShowWidget::clearTable() {
    bool my_ok = myQuery.exec("DELETE FROM layout_results");
    if (!my_ok) {
        qDebug() << "Error clearing table:" << myQuery.lastError();
    } else {
        qDebug() << "Table cleared successfully.";
    }
}


// 查询所有数据并显示
void SqlResultShowWidget::refreshData(QSqlQuery& query) {
	int table_rows = getRecordCount(query, "layout_results");
	ui->tableWidget_results->setRowCount(table_rows);
	if (!query.exec("SELECT * FROM layout_results")) {
		qDebug() << "Error selecting data:" << query.lastError();
	}
	else {
		int row_Number = 0;
		while (query.next()) {
			QString sku_code = query.value(0).toString();
			int tray_Lenght = query.value(1).toInt();
			int tray_With = query.value(2).toInt();
            int Tray_MaxHeight = query.value(3).toInt();
            int Tray_MaxWeight = query.value(4).toInt();
            int Tray_MaxNumber = query.value(5).toInt();
            int floor_num = query.value(6).toInt();
            double x = query.value(7).toDouble();
            double y = query.value(8).toDouble();
            double z = query.value(9).toDouble();
            int box_ori = query.value(10).toInt();
            int enter_dir = query.value(11).toInt();

			QTableWidgetItem* item0 = new QTableWidgetItem();
			item0->setText(sku_code);
			ui->tableWidget_results->setItem(row_Number, 0, item0);

			QTableWidgetItem* item1 = new QTableWidgetItem();
			item1->setText(QString::number(tray_Lenght));
			ui->tableWidget_results->setItem(row_Number, 1, item1);

			QTableWidgetItem* item2 = new QTableWidgetItem();
			item2->setText(QString::number(tray_With));
			ui->tableWidget_results->setItem(row_Number, 2, item2);

			QTableWidgetItem* item3 = new QTableWidgetItem();
            item3->setText(QString::number(Tray_MaxHeight));
			ui->tableWidget_results->setItem(row_Number, 3, item3);

            QTableWidgetItem* item31 = new QTableWidgetItem();
            item31->setText(QString::number(Tray_MaxWeight));
            ui->tableWidget_results->setItem(row_Number, 4, item31);

            QTableWidgetItem* item32 = new QTableWidgetItem();
            item32->setText(QString::number(Tray_MaxNumber));
            ui->tableWidget_results->setItem(row_Number, 5, item32);

			QTableWidgetItem* item4 = new QTableWidgetItem();
			item4->setText(QString::number(floor_num));
            ui->tableWidget_results->setItem(row_Number, 6, item4);

			QTableWidgetItem* item5 = new QTableWidgetItem();
			item5->setText(QString::number(x));
            ui->tableWidget_results->setItem(row_Number, 7, item5);

			QTableWidgetItem* item6 = new QTableWidgetItem();
			item6->setText(QString::number(y));
            ui->tableWidget_results->setItem(row_Number, 8, item6);

			QTableWidgetItem* item7 = new QTableWidgetItem();
			item7->setText(QString::number(z));
            ui->tableWidget_results->setItem(row_Number, 9, item7);

			QTableWidgetItem* item8 = new QTableWidgetItem();
			item8->setText(QString::number(box_ori));
            ui->tableWidget_results->setItem(row_Number, 10, item8);

			QTableWidgetItem* item9 = new QTableWidgetItem();
			item9->setText(QString::number(enter_dir));
            ui->tableWidget_results->setItem(row_Number, 11, item9);

			row_Number++;
		}
	}
	QScrollBar* bar = ui->tableWidget_results->verticalScrollBar();
	bar->setValue(bar->maximum());
}

QList<QStringList> SqlResultShowWidget::getSelectedData(QString Sku_Code, int Tray_Length,int Tray_Width,
                                           int Tray_MaxHeight,int Tray_MaxWeight,int Tray_MaxNumber)
{

    TrayInfo findKey = TrayInfo{Sku_Code,Tray_Length,Tray_Width, Tray_MaxHeight, Tray_MaxWeight, Tray_MaxNumber};

    QList<QStringList> result_list = queryDataList(myQuery,findKey);
    return result_list;
}



