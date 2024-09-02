#include "plc_com.h"
#include <QDebug>
#include <random>
#include <iostream>
#include <QThread>
#include <chrono>
#include <algorithm>

TS7Client* mClient = nullptr;

// DB 数据交换块
int DBTemp = 2000;
int DB_length = 302;
// DB 垛型块
int DBLayout = 2001;
int box_address_len = 18;

// 大小端转换函数
void swapBytes(unsigned char* data, int size) {
	for (int i = 0; i < size / 2; ++i) {
		std::swap(data[i], data[size - 1 - i]);
	}
}

 // 检查 PLC 状态
int checkPLCStatus()
{
    if(!mClient)    return 0;
    int result = mClient->Connected();
    return result;
}

// 用于连接plc
int mSnap7GetDataFromS71200Db(QString IP_config)
{
    mClient = new TS7Client;
    int rag = mClient->ConnectTo(IP_config.toStdString().c_str(), 0, 0);

	//delete mClient;
	return rag;
}

// real转换过程函数
union AddressToFloat {
	uint32_t address;
	float floatValue;
};

float ReadReal(int address_data[]) {
	// 将4个int型数据合并成一个32位无符号整数
	uint32_t combinedAddress = ((uint32_t)address_data[0] << 24) | ((uint32_t)address_data[1] << 16) | ((uint32_t)address_data[2] << 8) | (uint32_t)address_data[3];

	// 创建联合体实例，以便通过地址和浮点值之间进行转换
	AddressToFloat converter;
	converter.address = combinedAddress;

	// 返回转换后的浮点值
	return converter.floatValue;
}

int ReadInt(int address_data[]) {
	int result = (address_data[0] << 8) | address_data[1];
	return result;
}

std::string bytesToString(unsigned char* buffer, int actualLength)
{
	return std::string(reinterpret_cast<char*>(buffer), actualLength);
}

std::string ReadString(int address_data[])
{
	return std::string();
}

QVector<int> ConvertReal(float real_data) {
	QVector<int> convert_data;
	AddressToFloat converter;
	converter.floatValue = real_data;
	uint32_t floatAsInt = converter.address;
	// 从32位整数中提取四个地址数据
	convert_data.append((floatAsInt >> 24) & 0xFF);
	convert_data.append((floatAsInt >> 16) & 0xFF);
	convert_data.append((floatAsInt >> 8) & 0xFF);
	convert_data.append((floatAsInt) & 0xFF);
	qDebug() << "convert_data = " << convert_data;
	return convert_data;
}

void DBWriteBool(int DB_num, bool bool_data, int start_add) {
	int convert_data;

	if (bool_data == false) {
		convert_data = 0;
	}
	else {
		convert_data = 1;
	}
	unsigned char byte_data[2] = { 0 };
	byte_data[0] = 0;
	byte_data[1] = convert_data;
	swapBytes(byte_data, sizeof(byte_data));
	mClient->DBWrite(DB_num, start_add, 2, byte_data);
}

void DBWriteReal(int DB_num, float real_data, int start_add) {
	unsigned char byte_data[4];
	std::memcpy(byte_data, &real_data, sizeof(real_data));
	swapBytes(byte_data, sizeof(byte_data));
	int reg = mClient->DBWrite(DB_num, start_add, 4, byte_data);
}

void DBWriteInt(int DB_num, int int_data, int start_add) {
	unsigned char byte_data[2];
	byte_data[0] = int_data & 0xFF;
	byte_data[1] = (int_data >> 8) & 0xFF;
	swapBytes(byte_data, sizeof(byte_data));
	int reg = mClient->DBWrite(DB_num, start_add, 2, byte_data);
}

void DBWriteString(int DB_num, std::string str, int start_add)
{
	int length = str.size();
	unsigned char byte_data[254];
	byte_data[0] = 254;
	byte_data[1] = length;
	for (int i = 0; i < length + 0; i++) {
		byte_data[i + 2] = str.data()[i];
	}
	int reg = mClient->DBWrite(DB_num, start_add, length + 2, byte_data);
}

bool DBReadBool(int start_add, unsigned char* Snap7Rcv)
{
	if ((int)Snap7Rcv[start_add] == 0) {
		return false;
	}
	else {
		return true;
	}
}

float DBReadReal(int start_add, unsigned char* Snap7Rcv)
{
	int real_temp[4];
	float real_data;
	for (int i = 0, temp_j = 0; i < 4; i++, temp_j++)
	{
		real_temp[temp_j] = (int)Snap7Rcv[start_add + i];
	}
	real_data = ReadReal(real_temp);
	return real_data;
}

int DBReadInt(int start_add, unsigned char* Snap7Rcv)
{
	int int_temp[2];
	int int_data;
	for (int i = 0, temp_j = 0; i < 2; i++, temp_j++)
	{
		int_temp[temp_j] = (int)Snap7Rcv[start_add + i];
	}
	int_data = ReadInt(int_temp);
	return int_data;
}

std::string DBReadString(int start_add, unsigned char* Snap7Rcv)
{
	unsigned char String_temp[256];
	int string_len = (int)Snap7Rcv[start_add + 1];
	std::string String_data;
	for (int i = 0, temp_j = 0; i < string_len; i++, temp_j++)
	{
		String_temp[temp_j] = (int)Snap7Rcv[start_add + i + 2];
	}
	std::string stringValue = bytesToString(String_temp, string_len);
	return stringValue;
}

int SendInitData(int DB_num, int add, bool data_temp, float box_len, float box_width, float box_height) {
	qDebug() << "add start = " << add;
	DBWriteBool(DB_num, data_temp, add);
	add += 2;
	DBWriteReal(DB_num, box_len, add);
	add += 4;
	DBWriteReal(DB_num, box_width, add);
	add += 4;
	DBWriteReal(DB_num, box_height, add);
	add += 4;
	return add;
}

int SendBoxData(int DB_num, int add, double x, double y, double z, double rz, int enter) {
	qDebug() << "add start = " << add;
	DBWriteReal(DB_num, x, add);
	add += 4;
	DBWriteReal(DB_num, y, add);
	add += 4;
	DBWriteReal(DB_num, z, add);
	add += 4;
	DBWriteReal(DB_num, rz, add);
	add += 4;
	DBWriteInt(DB_num, enter, add);
	add += 2;
	return add;
}

int SendInitDataTotal(int DB_num, int add, bool data_temp, std::string str, int max_box_num, float box_max_height, float box_len, float box_width, float box_height)
{
	int single_add_len = 276;
	std::vector<unsigned char> writeBuffer(single_add_len);

	int convert_data;
	if (data_temp == false) {
		convert_data = 0;
	}
	else {
		convert_data = 1;
	}
	unsigned char bool_data[2] = { 0 };
	bool_data[0] = convert_data;
	bool_data[1] = 0;
	std::memcpy(&writeBuffer[0], bool_data, sizeof(bool_data));

	int length = str.size();
	unsigned char string_data[254] = {};
	string_data[0] = 254;
	string_data[1] = length;
	for (int i = 0; i < length + 0; i++) {
		string_data[i + 2] = str.data()[i];
	}
	std::memcpy(&writeBuffer[2], string_data, sizeof(string_data));

	unsigned char byte_data_int[2];
	uint16_t max_box_num_temp = max_box_num;
	std::memcpy(byte_data_int, &max_box_num_temp, sizeof(max_box_num_temp));
	swapBytes(byte_data_int, sizeof(byte_data_int));
	std::memcpy(&writeBuffer[258], byte_data_int, sizeof(byte_data_int));

	unsigned char byte_data[4];
	float temp_num = box_max_height;
	std::memcpy(byte_data, &temp_num, sizeof(temp_num));
	swapBytes(byte_data, sizeof(byte_data));
	std::memcpy(&writeBuffer[260], byte_data, sizeof(byte_data));

	temp_num = box_len;
	std::memcpy(byte_data, &temp_num, sizeof(temp_num));
	swapBytes(byte_data, sizeof(byte_data));
	std::memcpy(&writeBuffer[264], byte_data, sizeof(byte_data));

	temp_num = box_width;
	std::memcpy(byte_data, &temp_num, sizeof(temp_num));
	swapBytes(byte_data, sizeof(byte_data));
	std::memcpy(&writeBuffer[268], byte_data, sizeof(byte_data));

	temp_num = box_height;
	std::memcpy(byte_data, &temp_num, sizeof(temp_num));
	swapBytes(byte_data, sizeof(byte_data));
	std::memcpy(&writeBuffer[272], byte_data, sizeof(byte_data));
	int reg = mClient->DBWrite(DB_num, add, single_add_len, writeBuffer.data());

	return reg;
}

int SendBoxDataTotal(int DB_num, int add, QVector<SingleSKUBpp::LayoutResultTotal> layout_total_ori, int box_num)
{
	//int total_size = layout_total_ori.size();
	int total_size = box_num;
	// 单组箱子地址长度
	int single_add_len = 18;
	int total_add_len = single_add_len * total_size;
	std::vector<unsigned char> writeBuffer(total_add_len);

	// 创建动态数组存数据
	for (int i = 0; i < total_size; ++i) {
		const auto& pair = layout_total_ori[i];
		unsigned char byte_data[4];
		float temp_num = pair.x;
		std::memcpy(byte_data, &temp_num, sizeof(temp_num));
		swapBytes(byte_data, sizeof(byte_data));
		std::memcpy(&writeBuffer[0 + single_add_len * i], byte_data, sizeof(byte_data));

		temp_num = pair.y;
		std::memcpy(byte_data, &temp_num, sizeof(temp_num));
		swapBytes(byte_data, sizeof(byte_data));
		std::memcpy(&writeBuffer[4 + single_add_len * i], byte_data, sizeof(byte_data));

		temp_num = pair.z;
		std::memcpy(byte_data, &temp_num, sizeof(temp_num));
		swapBytes(byte_data, sizeof(byte_data));
		std::memcpy(&writeBuffer[8 + single_add_len * i], byte_data, sizeof(byte_data));

		temp_num = pair.box_ori;
		std::memcpy(byte_data, &temp_num, sizeof(temp_num));
		swapBytes(byte_data, sizeof(byte_data));
		std::memcpy(&writeBuffer[12 + single_add_len * i], byte_data, sizeof(byte_data));


		unsigned char int_data[2];
		int_data[0] = pair.enter_dir & 0xFF;         // 低字节
		int_data[1] = (pair.enter_dir >> 8) & 0xFF;  // 高字节
		swapBytes(int_data, sizeof(int_data));
		std::memcpy(&writeBuffer[16 + single_add_len * i], int_data, sizeof(int_data));
	}
	int reg = mClient->DBWrite(DB_num, add, total_add_len, writeBuffer.data());
	return reg;
}

int ChangeAddressInit(int address_init) {
	return address_init + 276;
}

int ChangeAddressBox(int address_init) {
	return address_init + box_address_len;
}

int sqlThread::ThreadSetBox(int DB_num, std::string SKU, float box_len, float box_width, float box_height, float box_weight, float tray_len, float tray_wid, float tray_wei, int tray_max_num, float tray_max_height, float tray_max_weight, float layout_gap)
{
	float ratio = 0;
	SingleSKUBpp myObject(tray_len, tray_wid, tray_max_height, tray_max_weight - tray_wei, box_len, box_width, box_height, box_weight, ratio, tray_max_num);

	auto test_box_size = myObject.GetBoxMember();

	QVector<SingleSKUBpp::LayoutResult> result_LayoutTotalAuto = myObject.LayoutTotalAuto();
	qDebug() << "test total LayoutTotalAuto place result:";
	for (const auto& pair : result_LayoutTotalAuto) {
		qDebug() << "(" << pair.x << ", " << pair.y << ", " << pair.z << ", " << pair.x_len << ", " << pair.y_len << ")";
	}

	QVector<QString> sku_code_list(myObject.layout_total_sql.size(), QString::fromStdString(SKU));
	QVector<SingleSKUBpp::TrayMsql> sku_tray_list(myObject.layout_total_sql.size(), myObject.tray_sql_data);

	emit sqlSendSignal(sku_code_list, myObject.layout_total_sql, sku_tray_list);
    emit threeDSendSignal(result_LayoutTotalAuto,myObject.tray_sql_data.box_width,myObject.tray_sql_data.box_height);

	// 码垛总数量输出
	int box_total_length = myObject.layout_total_ori.length();
	if (box_total_length > tray_max_num) {
		box_total_length = tray_max_num;
	}
	qDebug() << "+++++++++++++++++++++++++++++++++" << box_total_length;
	int len_temp_i = 0;
	for (const auto& pair : myObject.layout_total_ori) {
		len_temp_i++;
		qDebug() << "(" << pair.x << ", " << pair.y << ", " << pair.z << ", " << pair.box_ori << ", " << pair.enter_dir << ")";
		if (len_temp_i >= box_total_length) {
			break;
		}
	}

	// 发送计时
	QElapsedTimer timer;
	timer.start();

	float box_total_height = myObject.layout_total_ori[box_total_length - 1].z + test_box_size.height;
	// 发送初始数据（货物信息）
	int address_temp = 0;
	int reg1 = SendInitDataTotal(DB_num, address_temp, false, SKU, box_total_length, box_total_height, test_box_size.length, test_box_size.width, test_box_size.height);
	// 地址变化
	address_temp = ChangeAddressInit(address_temp);

	// 发送垛型
	int reg2 = SendBoxDataTotal(DB_num, address_temp, myObject.layout_total_ori, box_total_length);

	qint64 elapsed = timer.elapsed();
	qDebug() << "auto bpp run time plc：" << elapsed / 1000.0 << "s";
	int reg = 0;
	if (reg1 == 0 && reg2 == 0) {
		reg = 0;
	}
	else {
		reg = 1;
	}
	return reg;
}

void sqlThread::ReadDBTempThread()
{
	bool error_tray = false;
	int SKU_add = 0;
	int PLC_add = SKU_add + 256;
	int BPP_add = PLC_add + 2;
	int info_add_start = BPP_add + 2;
	while (true) {
		unsigned char Snap7RcvTemp[304];
		int result = mClient->DBRead(DBTemp, 0, DB_length, &Snap7RcvTemp);
		if (result != 0) {
			std::cerr << "Error: " << CliErrorText(result) << " (Error code: " << result << ")" << std::endl;
		}
		// SKU码
		std::string SKU_code;
		// 发送位
		int temp_PLC;
		// 接收位
		int temp_BPP;
		// 箱子信息{长 宽 高 重量}
		float box_info[4];
		// 托盘信息{长 宽 重 最大个数 最大高度 最大重量 间隙}
		float tray_info[3];
		int tray_max_num;
		float tray_max_height;
		float tray_max_weight;
		float layout_gap;

		SKU_code = DBReadString(SKU_add, Snap7RcvTemp);
		temp_PLC = DBReadInt(PLC_add, Snap7RcvTemp);
		temp_BPP = DBReadInt(BPP_add, Snap7RcvTemp);
		qDebug() << "SKU_code:" << SKU_code;
		qDebug() << "temp_PLC:" << temp_PLC;
		qDebug() << "temp_BPP:" << temp_BPP;

		if (temp_PLC == 1 && temp_BPP == 0) {
			// 读取当前码垛箱子信息
			box_info[0] = DBReadReal(info_add_start, Snap7RcvTemp);
			box_info[1] = DBReadReal(info_add_start + 4, Snap7RcvTemp);
			box_info[2] = DBReadReal(info_add_start + 8, Snap7RcvTemp);
			box_info[3] = DBReadReal(info_add_start + 12, Snap7RcvTemp);

			qDebug() << "box_info len:" << box_info[0];
			qDebug() << "box_info wid:" << box_info[1];
			qDebug() << "box_info hei:" << box_info[2];
			qDebug() << "box_info wei:" << box_info[3];

			// 读取码垛栈板信息
			tray_info[0] = DBReadReal(info_add_start + 16, Snap7RcvTemp);
			tray_info[1] = DBReadReal(info_add_start + 20, Snap7RcvTemp);
			tray_info[2] = DBReadReal(info_add_start + 24, Snap7RcvTemp);

			tray_max_num = DBReadInt(info_add_start + 28, Snap7RcvTemp);
			tray_max_height = DBReadReal(info_add_start + 30, Snap7RcvTemp);
			tray_max_weight = DBReadReal(info_add_start + 34, Snap7RcvTemp);
			layout_gap = DBReadReal(info_add_start + 38, Snap7RcvTemp);

			qDebug() << "tray_info len:" << box_info[0];
			qDebug() << "tray_info wid:" << box_info[1];
			qDebug() << "tray_info wei:" << box_info[2];
			qDebug() << "tray_max_num:" << tray_max_num;
			qDebug() << "tray_max_height:" << tray_max_height;
			qDebug() << "tray_max_weight:" << tray_max_weight;
			qDebug() << "layout_gap:" << layout_gap;

			int reg = ThreadSetBox(DBLayout, SKU_code, box_info[0], box_info[1], box_info[2], box_info[3],
				tray_info[0], tray_info[1], tray_info[2], tray_max_num, tray_max_height, tray_max_weight,
				layout_gap);

			//// 修改对应temp BPP值 修改为1 表示已经接收到数据
			//if (reg == 0) {
			//	DBWriteInt(DBTemp, 1, BPP_add);
			//}
			//else {
			//	// 错误发送
			//	DBWriteInt(DBTemp, 4, BPP_add);
			//}

			break;
		}
		// 遇到PLC发送错误 停止
		else if (temp_PLC == 4) {
			qDebug() << "error Tray";
			error_tray = true;
			break;
		}
		// 遇到错误 中断程序。
		if (error_tray == true) {
			break;
		}
	}
}


sqlThread::sqlThread()
{

}

void sqlThread::run()
{
    static int count_num = 0;
    while(true)
    {
        QVector<QString> sku_code_list;
        QVector<SingleSKUBpp::LayoutResultMsql> lay_out_list;
        QVector<SingleSKUBpp::TrayMsql> sku_tray_list;

        SingleSKUBpp::LayoutResultMsql s1 ={0,1.5,0.5,0,90,0};
        SingleSKUBpp::LayoutResultMsql s2 ={0,5.5,0.5,0,0,0};
        SingleSKUBpp::LayoutResultMsql s3 ={0,1.5,5.5,0,0,0};
        SingleSKUBpp::LayoutResultMsql s4 ={0,6.5,4.5,0,90,0};
        SingleSKUBpp::LayoutResultMsql s5 ={1,1.5,0.5,3,0,0};
        SingleSKUBpp::LayoutResultMsql s6 ={1,6.5,0.5,3,90,0};
        SingleSKUBpp::LayoutResultMsql s7 ={1,1.5,4.5,3,90,0};
        SingleSKUBpp::LayoutResultMsql s8 ={1,5.5,5.5,3,0,0};
        SingleSKUBpp::LayoutResultMsql s9 ={2,1.5,0.5,6,90,0};
        SingleSKUBpp::LayoutResultMsql s10={2,5.5,0.5,6,0,0};


        lay_out_list << s1 << s2 << s3 << s4 << s5 << s6<<s7<<s8<<s9<<s10;
        SingleSKUBpp::TrayMsql tray1 = {12,10,18,100,500,5,4,3,10};
        for(int i=0;i<10;++i)
        {
            sku_code_list<< QString("sku") + QString::number(count_num);
            sku_tray_list << tray1;
        }
        count_num++;
        QVector<SingleSKUBpp::LayoutResult> layout_r;
        SingleSKUBpp::LayoutResult r1 ={1.5,0.5,0, 4, 5};
        SingleSKUBpp::LayoutResult r2 ={5.5,0.5,0, 5, 4};
        SingleSKUBpp::LayoutResult r3 ={1.5,5.5,0, 5, 4};
        SingleSKUBpp::LayoutResult r4 ={6.5,4.5,0, 4, 5};
        SingleSKUBpp::LayoutResult r5 ={1.5,0.5,3, 5, 4};
        SingleSKUBpp::LayoutResult r6 ={6.5,0.5,3, 4, 5};
        SingleSKUBpp::LayoutResult r7 ={1.5,4.5,3, 4, 5};
        SingleSKUBpp::LayoutResult r8 ={5.5,5.5,3, 5, 4};
        SingleSKUBpp::LayoutResult r9 ={1.5,0.5,6, 4, 5};
        SingleSKUBpp::LayoutResult r10={5.5,0.5,6, 5, 4};


        layout_r<<r1<<r2<<r3<<r4<<r5<<r6<<r7<<r8<<r9<<r10;
        emit sqlSendSignal(sku_code_list, lay_out_list, sku_tray_list);
      //  emit threeDSendSignal(layout_r,5,3);
        sleep(5);
    }
    return;
	ReadDBTempThread();

}
