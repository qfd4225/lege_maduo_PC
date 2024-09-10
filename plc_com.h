#ifndef PLC_COM_H
#define PLC_COM_H

#include <QVector>
#include <snap7.h>
#include <iostream>
#include <QCoreApplication>
#include <QDebug>
#include "single_SKU.h"
#include <QThread>
extern TS7Client* mClient;
// DB 数据交换块
extern int  DBTemp;
extern int  DB_length;
// DB 垛型块
extern int DBLayout;
extern int box_address_len;

// SKU码
const QString SKU_code;

// 链接函数

void disconnectPLC();
int mSnap7GetDataFromS71200Db(QString IP_config);
int checkPLCStatus();
float ReadReal(int address_data[]);
int ReadInt(int address_data[]);
std::string bytesToString(unsigned char* buffer, int actualLength);

QVector<int> ConvertReal(float real_data);
// 写入函数
void DBWriteBool(int DB_num, bool bool_data, int start_add);
void DBWriteReal(int DB_num, float real_data, int start_add);
void DBWriteInt(int DB_num, int int_data, int start_add);
void DBWriteString(int DB_num, std::string String_data, int start_add);
// 读取函数
bool DBReadBool(int start_add, unsigned char* Snap7Rcv);
float DBReadReal(int start_add, unsigned char* Snap7Rcv);
int DBReadInt(int start_add, unsigned char* Snap7Rcv);
std::string DBReadString(int start_add, unsigned char* Snap7Rcv);
// 发送函数
int SendInitData(int DB_num, int add, bool data_temp, float box_len, float box_width, float box_height);
int SendBoxData(int DB_num, int add, double x, double y, double z, double rz, int enter);
//一次性传输所有箱子数据函数
int SendInitDataTotal(int DB_num, int add, bool data_temp, std::string str, int max_box_num, float box_max_height, float box_len, float box_width, float box_height);
int SendBoxDataTotal(int DB_num, int add, QVector<SingleSKUBpp::LayoutResultTotal> layout_total_ori, int box_num);

int ChangeAddressInit(int address_init);
int ChangeAddressBox(int address_init);



// 开始读取DB信息函数
class sqlThread : public QThread
{
	Q_OBJECT
public:
	sqlThread();
private:
	virtual void run();//任务处理线程
	int count_times = 0;


private:
	// 传送码垛数据
	int ThreadSetBox(int DB_num, std::string str, float box_len, float box_width, float box_height, float box_weight, float tray_len, float tray_wid, float tray_wei, int tray_max_num, float tray_max_height, float tray_max_weight, float layout_gap);
	//void ThreadSetBox(int DB_num, float box_len, float box_width, float box_height);

	// 开始读取DB信息函数
    void ReadDBTempThread();
signals:
    void threeDSendSignal(QVector<SingleSKUBpp::LayoutResult> layout,double goods_weight,double goods_height);
	void sqlSendSignal(QVector<QString> skuCodes, QVector<SingleSKUBpp::LayoutResultMsql> layoutResult, QVector<SingleSKUBpp::TrayMsql> trayResult);
};


#endif // PLC_COM_H
