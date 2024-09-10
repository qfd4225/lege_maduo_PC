#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "goods.h"
#include <QSettings>
#include "single_SKU.h"

#include <QSettings>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->logIn, SIGNAL(clicked()), this, SLOT(on_logIn_clicked));
    set_stack_widget = new OfflineShowDialog();
    set_stack_widget->hide();
    sql_show_widget = new sqlShowWidget();
    sql_show_widget->hide();
    view_boxes_3d = new viewerBoxes3D();
    view_boxes_3d->hide();
    set_ip_dialog = new SetIpDialog();
    set_ip_dialog->hide();
    set_db_dialog = new setDBDialog();
    set_db_dialog->hide();
    sql_thread = new sqlThread;
    connect(sql_thread, &sqlThread::finished, sql_thread, &QObject::deleteLater);
    connect(sql_thread, &sqlThread::sqlSendSignal, sql_show_widget,&sqlShowWidget::pluginDataSlot);
    connect(sql_thread, &sqlThread::threeDSendSignal, view_boxes_3d, &viewerBoxes3D::showLayout);
	// 设置垛型模式
    connect(ui->toolButton_setStackMode, &QToolButton::clicked, [=]() {set_stack_widget->show();  });
	// 3D显示模式
    connect(ui->toolButton_set3DMode, &QToolButton::clicked, [=]() {view_boxes_3d->show();  });
	// SQl存取模式
	connect(ui->toolButton_setSqlMode, &QToolButton::clicked, [=]() {sql_show_widget->show(); this->hide(); });
	// 设置DB块
	connect(ui->toolButton_setDbBase, &QToolButton::clicked, [=]() {set_db_dialog->show(); });
	// 设置PLC
	connect(ui->toolButton_setPLC, &QToolButton::clicked, [=]() {set_ip_dialog->show(); });

    connect(set_stack_widget, &OfflineShowDialog::returnToMain, this, &MainWindow::stackToMain);
    connect(set_stack_widget, &OfflineShowDialog::returnToModeChoose, this, &MainWindow::stackToModeChoose);
    connect(set_stack_widget, &OfflineShowDialog::updateSql,[=](OfflineShowDialog::KeyStruct key,QList<OfflineShowDialog::ValueStruct> values){
        QString code = key.sku_code;
        int Tray_Length = key.tray_Length;
        int Tray_Width = key.tray_Width;
        int Tray_MaxHeight = key.tray_MaxHeight;
        int Tray_MaxWeight = key.tray_MaxWeight;
        int Tray_MaxNum = key.tray_MaxNum;
        int Floor_num = key.floor_num;
        QVector<SingleSKUBpp::LayoutResultMsql> result_msqls;
        for(auto each:values)
        {
            double ex = each.x;
            double ey = each.y;
            double ez = each.z;
            int  ebo = each.box_ori;
            int  eed = each.enter_dir;
            SingleSKUBpp::LayoutResultMsql result_msql = {Floor_num,ex,ey,ez,ebo,eed};
            result_msqls.append(result_msql);
        }
        SqlResultShowWidget::TrayInfo tray_info = {code,Tray_Length,Tray_Width,Tray_MaxHeight,Tray_MaxWeight,Tray_MaxNum};
        sql_show_widget->box_sql_widget->update2DResult(tray_info,result_msqls);
    });

	connect(view_boxes_3d, &viewerBoxes3D::returnHome, this, &MainWindow::view3dToMain);
	connect(view_boxes_3d, &viewerBoxes3D::returnChooseMode, this, &MainWindow::view3dToModeChoose);
    connect(view_boxes_3d, &viewerBoxes3D::disconnectPLCSignal,this,[=](){qDebug()<<"开始断开连接!";disconnectPLC();});

    connect(view_boxes_3d, &viewerBoxes3D::readSqlSignal, [=](){
        sql_show_widget->show();
        // box_sql_widget->setFixedSize(600,500);
        // box_show_widget->setFixedSize(600,500);
        // box_sql_widget->show();
        // box_show_widget->show();
    });
    connect(view_boxes_3d,&viewerBoxes3D::getDataSignal,[=](QString Sku_Code,int Tray_Length,int Tray_Width,int Tray_MaxHeight,int Tray_MaxWeight,int Tray_MaxNumber){
        if(sql_show_widget)
        {
            QList<QStringList> result_list;
            QList<QStringList> box_list;
            sql_show_widget->getSelectedData(Sku_Code,Tray_Length,Tray_Width,Tray_MaxHeight,Tray_MaxWeight,Tray_MaxNumber,result_list,box_list);

            view_boxes_3d->setSelectedData(result_list,box_list);
            // sql_show_widget->getAllData(result_list,box_list);
            // set_stack_widget->setAllData(result_list,box_list);
        }
    });
	connect(sql_show_widget, &sqlShowWidget::returnToMain, this, &MainWindow::sqlToMain);
	connect(sql_show_widget, &sqlShowWidget::returnToModeChoose, this, &MainWindow::sqlToModeChoose);
    connect(set_stack_widget, &OfflineShowDialog::getDataSignal,[=](QString Sku_Code,int Tray_Length,int Tray_Width,int Tray_MaxHeight,int Tray_MaxWeight,int Tray_MaxNumber){
        if(sql_show_widget)
        {
            QList<QStringList> result_list;
            QList<QStringList> box_list;
            sql_show_widget->getSelectedData(Sku_Code,Tray_Length,Tray_Width,Tray_MaxHeight,Tray_MaxWeight,Tray_MaxNumber,result_list,box_list);
            set_stack_widget->setSelectedData(result_list,box_list);

            // sql_show_widget->getAllData(result_list,box_list);
            // set_stack_widget->setAllData(result_list,box_list);
        }
    });

	connect(set_db_dialog, &setDBDialog::confirmClicked, [=](QVector<int> db_list) {
        if (db_list.size() != 2)
		{
			return;
		}
        DBTemp = db_list[0];
        DBLayout = db_list[1];

		});

    connect(set_ip_dialog, &SetIpDialog::confirmClicked, [=](QString ip_config) {
            current_ip = ip_config;
            if (set_db_dialog->exec() == QDialog::Accepted) {
                connectPlc(current_ip);
            }
		});


	// 返回登录界面
    connect(ui->toolButton_return, &QToolButton::clicked,  [=]() {ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage1); });
    connect(ui->toolButton_return_copy, &QToolButton::clicked,  [=]() {ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage1); });

    // 在线模式开启 = PLC配置 + DB块配置 + 3D显示
    connect(ui->pushButton_online,&QPushButton::clicked,[=](){
        if(!isPLCConnected)
        {
            set_ip_dialog->exec();
        }
        else
        {
            ui->toolButton_set3DMode->click();  // 连接成功后打开3D界面
        }
       }
    );
    connect(ui->toolButton_setStackMode, &QToolButton::clicked, [=]() {set_stack_widget->show();});

    // 离线模式开启 = 垛型配置
    connect(ui->pushButton_offline,&QPushButton::clicked,[=](){
        ui->toolButton_setStackMode->click();
            }
        );
	ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage1);

    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, [=]() mutable
            {
            int plc_connect_status = checkPLCStatus();
            qDebug()<<"plc连接状态:"<<plc_connect_status;
    });
    statusTimer->start(1000); // 5000 毫秒 = 5 秒
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::connectPlc(QString ip_key)
{
    int max_attempts = 5; // 最大尝试次数
    int try_times = 0;

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() mutable {

        // 连接PLC
        int rag = mSnap7GetDataFromS71200Db(current_ip);
        if (rag == 0) {
            // 连接成功
            DBTemp = readFromIni("DBTemp").toInt();
            DBLayout = readFromIni("DBLayout").toInt();
            timer->stop(); // 停止定时器
            timer->deleteLater(); // 清理定时器对象
            sql_thread->start();    // sql读取线程开启
            ui->toolButton_set3DMode->click();  // 连接成功后打开3D界面
            isPLCConnected = true; // plc 连接成功
            // plc 连接成功时把参数写入配置
            writeToIni("ip", current_ip);
            writeToIni("DBTemp", QString::number(DBTemp));
            writeToIni("DBLayout", QString::number(DBLayout));
        }
        else {
            // 连接失败，尝试重新连接
            qDebug() << "connect fail" << try_times << " times";
            try_times++;
            if (try_times >= max_attempts) {
                QMessageBox::warning(nullptr, "提示！", "plc无法连接上!",
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::Yes);
                timer->stop(); // 停止定时器
                timer->deleteLater(); // 清理定时器对象
                return;
            }
        }
    });

    timer->start(200); // 每200毫秒尝试连接一次
}


void MainWindow::writeToIni(const char* key, QString value) {
    QSettings settings("IPconfig.ini", QSettings::IniFormat);
	settings.setValue(key, QString(value));
}

QString MainWindow::readFromIni(const QString& key) {
    QSettings settings("IPconfig.ini", QSettings::IniFormat);

	// 获取键对应的值
	QVariant value = settings.value(key);

	// 检查值是否有效
	if (value.isValid()) {
		return value.toString();
	}
	else {
		return QString();  // 返回空 QString 表示没有找到数据
	}
}

// 登录
void MainWindow::on_logIn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage3);
}

void MainWindow::stackToMain()
{
	set_stack_widget->hide();
	ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage1);
	this->show();

}

void MainWindow::stackToModeChoose()
{
	set_stack_widget->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage3);
	this->show();
}

void MainWindow::view3dToMain()
{
	view_boxes_3d->hide();
	ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage1);
	this->show();
}

void MainWindow::view3dToModeChoose()
{
	view_boxes_3d->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage3);
	this->show();
}

void MainWindow::sqlToMain()
{
	sql_show_widget->hide();
	ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage1);
	this->show();
}

void MainWindow::sqlToModeChoose()
{
	sql_show_widget->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage3);
	this->show();

}
