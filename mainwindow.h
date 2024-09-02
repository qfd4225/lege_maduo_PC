#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "offline_show_dialog.h"
#include "sql_show_widget.h"
#include"set_ip_dialog.h"
#include"set_db_dialog.h"
#include "viewer_boxes_3d.h"
#include "plc_com.h"
QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();




private:
	Ui::MainWindow* ui;
    setDBDialog* set_db_dialog = nullptr;
    SetIpDialog* set_ip_dialog = nullptr;
	OfflineShowDialog* set_stack_widget = nullptr;
	sqlShowWidget* sql_show_widget = nullptr;
    viewerBoxes3D* view_boxes_3d = nullptr;
    sqlThread* sql_thread = nullptr;
    int max_attempts = 10; // 最大尝试次数
    int try_times = 0;
    QString readFromIni(const QString &key);
    QString current_ip = "";
    void writeToIni(const char *key, QString value);

    void connectPlc(QString ip_key);
    bool isPLCConnected = false;    //plc 是否已经连接上

     QTimer *statusTimer = nullptr;
private slots:
	void on_logIn_clicked();
	void stackToMain();
	void stackToModeChoose();
    void view3dToMain();
    void view3dToModeChoose();
	void sqlToMain();
	void sqlToModeChoose();

};
#endif // MAINWINDOW_H
