#ifndef OFFLINE_SHOW_DIALOG_H
#define OFFLINE_SHOW_DIALOG_H
#include "goods.h"
#include <QDialog>
#include<QVector>
#include"sql_show_widget.h"
#include "sql_key_edit_dialog.h"
namespace Ui {
	class settingmode;
}

class OfflineShowDialog : public QDialog
{
public:
    struct KeyStruct {
        QString sku_code;
        int tray_Length;
        int tray_Width;
        int tray_MaxHeight;
        int tray_MaxWeight;
        int tray_MaxNum;
        int floor_num;

        bool operator<(const KeyStruct& other) const {
            return std::tie(sku_code, tray_Length, tray_Width, tray_MaxHeight,tray_MaxWeight,tray_MaxNum, floor_num) <
                   std::tie(other.sku_code, other.tray_Length, other.tray_Width, other.tray_MaxHeight, other.tray_MaxWeight,other.tray_MaxNum,other.floor_num);
        }
    };

    struct ValueStruct {
        double x;
        double y;
        double z;
        int box_ori;
        int enter_dir;
    };



	Q_OBJECT

public:
    explicit OfflineShowDialog(QWidget* parent = nullptr);
    ~OfflineShowDialog();
    bool fixHeight = false;       //固定放货点高度
	bool rowAligned = false;      //竖直方向排列
	bool floorAligned = false;    //层自动对齐
	QVector<QVector<QString>> myVector;

    void setSelectedData(QList<QStringList> result_list,QList<QStringList> box_list);
protected:
	void wheelEvent(QWheelEvent* event) override { return; }
private:
	Ui::settingmode* ui;
    goods* g = nullptr;
    QMultiMap<KeyStruct, ValueStruct> sqlMap;
    void clearMap();
    void insertEntry(const QStringList &p_result);
    QList<ValueStruct> findEntries(const KeyStruct& key);
    sqlKeyEditDialog *key_edit_dialog = nullptr;
    QString Sku_Code ;
    int Tray_Length ;
    int Tray_Width;
    int Tray_MaxHeight ;
    int Tray_MaxWeight;
    int Tray_MaxNumber;
    bool isIndexing = false;
signals:
    void updateSql(KeyStruct key,QList<OfflineShowDialog::ValueStruct> values);
	void returnToMain();
	void returnToModeChoose();
    void getDataSignal(QString Sku_Code,int Tray_Length,int Tray_Width,int Tray_MaxHeight,int Tray_MaxWeight,int Tray_MaxNumber);
};

#endif // OFFLINE_SHOW_DIALOG_H
