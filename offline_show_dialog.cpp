#include "offline_show_dialog.h"
#include "ui_offline_show_dialog.h"
#include<QScrollBar>
#include <QFile>
#include<QFileDialog>
#include<QMessageBox>

bool isApproximatelyEqual(double a, double b, double epsilon = 1e-3) {
    return std::fabs(a - b) < epsilon;
}

OfflineShowDialog::OfflineShowDialog(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::settingmode)
{
    ui->setupUi(this);
    connect(ui->pushButton_logout, &QPushButton::clicked, this, &OfflineShowDialog::returnToMain);
    connect(ui->pushButton_return, &QPushButton::clicked, this, &OfflineShowDialog::returnToModeChoose);

    key_edit_dialog = new sqlKeyEditDialog(this);
    connect(key_edit_dialog,&sqlKeyEditDialog::confirmSignal,[=](QStringList combo_info)
            {
                if(combo_info.size() != 6)
                {
                    QMessageBox::warning(NULL, "提示！", "combo_info.size() != 6",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                    return;
                }
                QString str_skuCode = combo_info[0];
                QString str_TrayLength = combo_info[1];
                QString str_TrayWidth = combo_info[2];
                QString str_TrayMaxHeight = combo_info[3];
                QString str_TrayMaxWeight = combo_info[4];
                QString str_TrayMaxNumber = combo_info[5];

                if (str_skuCode.isEmpty() ||
                    str_TrayLength.isEmpty() ||
                    str_TrayWidth.isEmpty() ||
                    str_TrayMaxHeight.isEmpty() ||
                    str_TrayMaxWeight.isEmpty() ||
                    str_TrayMaxNumber.isEmpty()) {
                    QMessageBox::warning(NULL, "提示！", "请完成信息填写!",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::Yes);
                    return;
                }

                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "确认", "请确认操作!",
                                              QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    key_edit_dialog->close();
                    Sku_Code = str_skuCode;
                    Tray_Length = str_TrayLength.toInt();
                    Tray_Width = str_TrayWidth.toInt();
                    Tray_MaxHeight = str_TrayMaxHeight.toInt();
                    Tray_MaxWeight = str_TrayMaxWeight.toInt();
                    Tray_MaxNumber = str_TrayMaxNumber.toInt();
                    isIndexing = true; // 索引开始
                    emit getDataSignal(Sku_Code,Tray_Length,Tray_Width,Tray_MaxHeight,Tray_MaxWeight,Tray_MaxNumber);
                    }
            });

    connect(ui->pushButton_readData,&QPushButton::clicked,[=]() {
        key_edit_dialog->clearForm();
        key_edit_dialog->exec();
         });

    // 取消窗口中所有按钮的回车键绑定
	foreach(QPushButton * button, findChildren<QPushButton*>()) {
		button->setAutoDefault(false);
    }

    connect(ui->pushButton_setAll,&QPushButton::clicked,[=]() {
        QString current_text = ui->ComboBox_selectPlan->currentText();

        QString sku_code = ui->label_sku->text();
        int tray_Lenght = ui->label_Tray_Length->text().toInt();
        int tray_With = ui->label_Tray_Width->text().toInt();
        int tray_MaxHeight = ui->label_Tray_MaxHeight->text().toInt();
        int tray_MaxWeight = ui->label_Tray_MaxWeight->text().toInt();
        int tray_MaxNumber = ui->label_Tray_MaxNumber->text().toInt();
        QList<int> floornums = getComboBoxValues(ui->comboBox_floornumbers);
         qDebug()<<"floornums="<<floornums;
        for(int floornum:floornums)
        {
            KeyStruct c_Key{
                sku_code,tray_Lenght,tray_With, tray_MaxHeight,tray_MaxWeight,tray_MaxNumber, floornum
            };

            create_Form(current_text,c_Key);
        }

    });
    connect(ui->comboBox_floornumbers, &QComboBox::currentIndexChanged, this,
        [=](int index){
            if(ui->comboBox_floornumbers->currentText().isEmpty())
            {
                if(g)
                {
                    g->hide();
                }
                return;
            }
            int floor_num = ui->comboBox_floornumbers->currentText().toInt();
            QVector<SingleSKUBpp::LayoutResult> layout_result;

            QString sku_code = ui->label_sku->text();
            int tray_Lenght = ui->label_Tray_Length->text().toInt();
            int tray_With = ui->label_Tray_Width->text().toInt();
            int tray_MaxHeight = ui->label_Tray_MaxHeight->text().toInt();
            int tray_MaxWeight = ui->label_Tray_MaxWeight->text().toInt();
            int tray_MaxNumber = ui->label_Tray_MaxNumber->text().toInt();


            qDebug()<<"sqlMap.size()="<<sqlMap.size();
            KeyStruct p_Key = {sku_code,tray_Lenght,tray_With, tray_MaxHeight,tray_MaxWeight,tray_MaxNumber, floor_num};
            qDebug()<<"p_Key="<<sku_code<<tray_Lenght<<tray_With<< tray_MaxHeight<<tray_MaxWeight<<tray_MaxNumber<< floor_num;
            QList<OfflineShowDialog::ValueStruct> find_value = findEntries(p_Key);
            double boxLength =ui->label_len->text().toDouble();
            double boxWidth =ui->label_wid->text().toDouble();
            double boxHeight = ui->label_hei->text().toDouble();
            double boxWeight = ui->label_wei->text().toDouble();
            for (const auto& value : find_value) {
                qDebug()<<"in for";
                SingleSKUBpp::LayoutResult l_r;

                if(value.box_ori == 0)
                {
                    l_r = {value.x,value.y,value.z,boxLength,boxWidth};
                }
                else
                {
                    l_r = {value.x,value.y,value.z,boxWidth,boxLength};
                }
                layout_result.append(l_r);
            }
            if (g)
            {
                delete g;
                g = nullptr;
            }

            g = new goods(tray_Lenght,tray_With,tray_MaxHeight,boxLength,boxWidth,boxHeight,tray_MaxWeight,boxWeight,0.0,tray_MaxNumber,ui->widget_ShowFloor);
            g->resize(ui->widget_ShowFloor->size());
            connect(g,&goods::returnToEditMode,[=](){
                this->show();
                g->hide();
            });
            connect(g,&goods::sendResult,[=](QVector<SingleSKUBpp::LayoutResult> layout){
                QList<OfflineShowDialog::ValueStruct> p_values;
                for(const auto &p_layout:layout)
                {
                    OfflineShowDialog::ValueStruct p_value;
                    if (isApproximatelyEqual(p_layout.x_len, boxLength) && isApproximatelyEqual(p_layout.y_len, boxWidth)) {
                         p_value = { p_layout.x,p_layout.y,p_layout.z,0,0};
                    }
                    else
                    {
                          p_value = { p_layout.x,p_layout.y,p_layout.z,90,0};
                    }
                    p_values.append(p_value);
                }
                emit updateSql(p_Key,p_values);

                QTimer::singleShot(100, [=](){
                    emit getDataSignal(Sku_Code,Tray_Length,Tray_Width,Tray_MaxHeight,Tray_MaxWeight,Tray_MaxNumber);
                });

                         });

            g->refreshLayout(layout_result);
        }
    );
}

OfflineShowDialog::~OfflineShowDialog()
{
    if (g)
    {
        g->close();
        delete g;
        g = nullptr;
    }
	delete ui;
}



QList<int> OfflineShowDialog::getComboBoxValues(QComboBox* comboBox) {
    // 获取条目数量
    int itemCount = comboBox->count();
    QList<int> values;
    // 遍历所有条目
    for (int i = 0; i < itemCount; ++i) {
        // 获取条目的文本
        QString text = comboBox->itemText(i);
        if (text.isEmpty())
        {
            continue;
        }
        // 将文本转换为整数
        bool ok;
        int value = text.toInt(&ok);

        // 检查转换是否成功
        if (ok) {
            values.append(value);
        }
    }
    return values;
}

void OfflineShowDialog::create_Form(QString currentContent,KeyStruct p_Key)
{
    //  disconnect(this, &goods::updateMyGraphicRectItem, myGraphicRectItemInstance, &myGraphicRectItem::updateFromLineEdit);

    double boxLength =ui->label_len->text().toDouble();
    double boxWidth =ui->label_wid->text().toDouble();
    double boxHeight = ui->label_hei->text().toDouble();
    double boxWeight = ui->label_wei->text().toDouble();

    SingleSKUBpp* skuObject = new SingleSKUBpp(p_Key.tray_Length, p_Key.tray_Width, p_Key.tray_MaxHeight,p_Key.tray_MaxWeight,
                                               boxLength, boxWidth, boxHeight, boxWeight,
                                               0.0,p_Key.tray_MaxNum);

    QVector<SingleSKUBpp::LayoutResult> lay_result;
    if (currentContent == "行模式")
    {
        qDebug()<<"1";
        lay_result = skuObject->HorizontalLayout(p_Key.tray_Length, p_Key.tray_Width, p_Key.floor_num + 1 , 1000);
    }
    else if (currentContent == "列模式")
    {
        lay_result = skuObject->VerticalLayout(p_Key.tray_Length, p_Key.tray_Width, p_Key.floor_num + 1, 1000);
    }
    else if (currentContent == "纵横模式")
    {
        lay_result = skuObject->CrossLayout(p_Key.tray_Length, p_Key.tray_Width, p_Key.floor_num + 1, 1000);
    }
    else if (currentContent == "单回字模式")
    {
        lay_result = skuObject->SingleRectangularLayout(p_Key.tray_Length, p_Key.tray_Width, p_Key.floor_num + 1, 1000);
    }
    else if (currentContent == "多回字模式")
    {
        lay_result = skuObject->MutiRectangularLayout(p_Key.tray_Length, p_Key.tray_Width, p_Key.floor_num + 1, 1000);
    }
    else if (currentContent == "镜像回字模式")
    {
        lay_result = skuObject->MutiRectangularLayoutMirror(p_Key.tray_Length, p_Key.tray_Width, p_Key.floor_num + 1, 1000);
    }
    else
    {
        QMessageBox::warning(this, "警告", "模式选择错误！");
        return;
    }

    // 确认操作
    // QMessageBox::StandardButton reply;
    // reply = QMessageBox::question(this, "确认", "请确认操作!",
    //                               QMessageBox::Yes | QMessageBox::No);
    // if (reply != QMessageBox::Yes) {return;}

    if(lay_result.size()>0)     // 更新至sql
    {
        qDebug()<<"更新至sql1";
        QString sku_code = ui->label_sku->text();
        int tray_Lenght = ui->label_Tray_Length->text().toInt();
        int tray_With = ui->label_Tray_Width->text().toInt();
        int tray_MaxHeight = ui->label_Tray_MaxHeight->text().toInt();
        int tray_MaxWeight = ui->label_Tray_MaxWeight->text().toInt();
        int tray_MaxNumber = ui->label_Tray_MaxNumber->text().toInt();

        QList<OfflineShowDialog::ValueStruct> p_values;

        for(const auto &p_layout:lay_result)
        {
            qDebug()<<"更新至sql2";
            OfflineShowDialog::ValueStruct p_value;
            if (isApproximatelyEqual(p_layout.x_len, boxLength) && isApproximatelyEqual(p_layout.y_len, boxWidth)) {
                p_value = { p_layout.x,p_layout.y,p_layout.z,0,0};
                qDebug()<<"0";
            }
            else
            {
                p_value = { p_layout.x,p_layout.y,p_layout.z,90,0};
                qDebug()<<"90";
            }

            p_values.append(p_value);
        }
        qDebug()<<"p_value="<<p_values.size();
        emit updateSql(p_Key,p_values);

        QTimer::singleShot(100, [=](){
            qDebug()<<"更新至sql3";
            emit getDataSignal(sku_code,tray_Lenght,tray_With,tray_MaxHeight,tray_MaxWeight,tray_MaxNumber);
        });
    }
}


void OfflineShowDialog::setSelectedData(QList<QStringList> result_list, QList<QStringList> box_list)
{
    if(isIndexing)
    {
        if(result_list.size()<=0 || box_list.size()<=0)
        {
            QMessageBox::information(this,"提示","未查询到该sku信息!");
            return;
        }
        else
        {
            QMessageBox::information(this,"提示","查询成功!");
            isIndexing = false;
        }
    }


    clearMap();

    QSet<int> floor_num_list;
    for(auto p_result : result_list)
    {       
        int floor_num = p_result.at(6).toInt();
        floor_num_list.insert(floor_num);
        insertEntry(p_result);
    }

    auto b_result = box_list[0];

    QString sku_code_str = b_result.at(0);
    QString length_str = b_result.at(1);
    QString width_str = b_result.at(2);
    QString height_str = b_result.at(3);
    QString weight_str = b_result.at(4);

    ui->label_sku->setText(sku_code_str);
    ui->label_len->setText(length_str);
    ui->label_wid->setText(width_str);
    ui->label_hei->setText(height_str);
    ui->label_wei->setText(weight_str);

    ui->label_Tray_Length->setText(QString::number(Tray_Length));
    ui->label_Tray_MaxHeight->setText(QString::number(Tray_MaxHeight));
    ui->label_Tray_MaxNumber->setText(QString::number(Tray_MaxNumber));
    ui->label_Tray_MaxWeight->setText(QString::number(Tray_MaxWeight));
    ui->label_Tray_Width->setText(QString::number(Tray_Width));

    QVector<int> sorted_list  = floor_num_list.values();
    std::sort(sorted_list.begin(), sorted_list.end());

    QString  Current_Floor = "";
    qDebug()<<"ui->comboBox_floornumbers->currentText()="<<ui->comboBox_floornumbers->currentText();
    if(!ui->comboBox_floornumbers->currentText().isEmpty())
    {
        Current_Floor = ui->comboBox_floornumbers->currentText();
    }
    ui->comboBox_floornumbers->clear();
    ui->comboBox_floornumbers->addItem("");
    for(auto floor_n : sorted_list)
    {
        QString floor_n_str = QString::number(floor_n);
        ui->comboBox_floornumbers->addItem(floor_n_str);
    }
    qDebug()<<"Current_Floor="<<Current_Floor;
    if(!Current_Floor.isEmpty())
    {
        ui->comboBox_floornumbers->setCurrentText(Current_Floor);
    }
}

void OfflineShowDialog::insertEntry(const QStringList& p_result) {
        KeyStruct key = {p_result.at(0),
                         p_result.at(1).toInt(),
                         p_result.at(2).toInt(),
                         p_result.at(3).toInt(),
                         p_result.at(4).toInt(),
                         p_result.at(5).toInt(),
                         p_result.at(6).toInt()};


        ValueStruct value = {p_result.at(7).toDouble(),
                             p_result.at(8).toDouble(),
                             p_result.at(9).toDouble(),
                             p_result.at(10).toInt(),
                             p_result.at(11).toInt()};

        sqlMap.insert(key, value);
}

QList<OfflineShowDialog::ValueStruct> OfflineShowDialog::findEntries(const KeyStruct& key) {
    return sqlMap.values(key);
}


void OfflineShowDialog::clearMap() {
    sqlMap.clear();
}

