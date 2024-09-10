#include "goods.h"
#include "ui_goods.h"
#include "mygraphicrectitem.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include<QLineEdit>
goods::goods(double p_stack_x ,double p_stack_y,double p_stack_z,double p_goods_x,double p_goods_y,double p_goods_h,double p_stack_weight,double p_goods_weight,
             double p_over_rate,int p_max_num , QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::goods)
{
	ui->setupUi(this);
	myGraphicRectItemInstance = nullptr;// 初始化指针
    connect(ui->pushbutton_create, &QPushButton::clicked, this, &goods::on_create_clicked);//生成方案的按钮
	connect(ui->levelMap, &NewGraphicsView::itemPosSend, this, &goods::updateInfo);//选中物体后在三个lineEdit中更新内容
	connect(ui->levelMap, &NewGraphicsView::itemChange, this, &goods::InstanceRefresh);//选中物体后修改当前实例指向
	connect(ui->return_to_level, &QPushButton::clicked, this, &goods::returnToEditMode);
	//修改两个lineEdit后将变动发送到newgraphicsview中对mygraphicsrectitem进行修改
    connect(ui->select_x, &QLineEdit::editingFinished, this, &goods::handleLineEditEditingFinished);
    connect(ui->select_y, &QLineEdit::editingFinished, this, &goods::handleLineEditEditingFinished);
	//connect(ui->select_degree, SIGNAL(editingFinished()), this, SLOT(handleLineEditEditingFinished()));
	//保存文件
    connect(ui->PushButton_saveToFile, SIGNAL(clicked()), this, SLOT(saveLog()));
    //保存sql
    connect(ui->PushButton_saveToSql, SIGNAL(clicked()), this, SLOT(saveSql()));


    // //初始化栈板尺寸、货物尺寸、超托率
    stack_x = p_stack_x;
    stack_y = p_stack_y;
    // stack_x *= 30;
    // stack_y *= 30;
    stack_z = p_stack_z;
    goods_x = p_goods_x;
    goods_y = p_goods_y;
    goods_h = p_goods_h;
    // goods_y *= 30;
    // goods_h *= 30;
    goods_weight = p_goods_weight;
    stack_weight = p_stack_weight;
    over_rate = p_over_rate;

    // //弹出窗口提示输入参数并赋值
    // if (!openInputDialog())
    // {
    // 	is_created = false;
    // 	qDebug() << "false!";
    // 	this->hide();
    // 	return;
    // }
    // is_created = true;

    // //为了显示美观，对尺寸进行放大
    stack_x_show = stack_x * 30;
    stack_y_show = stack_y * 30;
    goods_x_show = goods_x * 30;
    goods_y_show = goods_y * 30;

    // //根据货物尺寸建立myObject对象
    myObject = new SingleSKUBpp(stack_x, stack_y, stack_z, stack_weight, goods_x, goods_y, goods_h, goods_weight, over_rate,p_max_num);

	//根据栈板尺寸建立栈板对象并设置属性
    stack = new myGraphicRectItem(0, 0, stack_x_show, stack_y_show, 0);
	stack->setPos(0, 0);  // 设置左上角位置
	// 禁用选择
	stack->setFlag(QGraphicsRectItem::ItemIsSelectable, false);
	// 确保位于最底层
	stack->setZValue(-1);
	// 设置不透明度
	stack->setOpacity(0.2);
	// 将图形项添加到场景中
	scene->addItem(stack);
	qDebug() << "stack type:" << stack->type();
	//为newGraphicsView对象设置scene并调整参数
	ui->levelMap->setScene(scene);
	ui->levelMap->setRenderHint(QPainter::Antialiasing, true);
	ui->levelMap->setRenderHint(QPainter::SmoothPixmapTransform, true);
	ui->levelMap->setRenderHint(QPainter::TextAntialiasing, true);
	//取消滚动条
	//ui->levelMap->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//ui->levelMap->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//聚焦到中心
    ui->levelMap->centerOn(stack_x_show / 2, stack_y_show / 2);
	this->show();
}

goods::~goods()
{
	delete myGraphicRectItemInstance;
	myGraphicRectItemInstance = nullptr;
	delete ui;
	ui = nullptr;
}
void goods::refreshLayout(QVector<SingleSKUBpp::LayoutResult> layout_result)
{
    scene->clear();
    stack = new myGraphicRectItem(0, 0, stack_x_show, stack_y_show, 0);
    stack->setPos(0, 0);
    stack->setFlag(QGraphicsRectItem::ItemIsSelectable, false);
    stack->setZValue(-1);
    stack->setOpacity(0.2);
    scene->addItem(stack);
    QString currentContent = ui->opSelector->currentText();

    for (const auto& pair : layout_result) {
        qDebug()<<"pair = "<<pair.x <<pair.y << pair.x_len << pair.y_len << 1;
        myGraphicRectItem* rectItem = new myGraphicRectItem(pair.x * 30.0, pair.y * 30.0, pair.x_len * 30.0, pair.y_len * 30.0, 1);
        current_z = pair.z;
        scene->addItem(rectItem);
    }

    disconnect(this, &goods::updateMyGraphicRectItem, myGraphicRectItemInstance, &myGraphicRectItem::updateFromLineEdit);
    myGraphicRectItemInstance = nullptr;
    ui->select_x->setText(QString::number(0));
    ui->select_y->setText(QString::number(0));
    ui->select_degree->setText(QString::number(0));
}

void goods::changeForVisual(QVector<SingleSKUBpp::LayoutResult>& layout)
{
    for(auto& p_layout : layout)
    {
        p_layout.x = p_layout.x*30;
        p_layout.y= p_layout.y*30;
        p_layout.z= p_layout.z*30;
        p_layout.x_len= p_layout.x_len*30;
        p_layout.y_len= p_layout.y_len*30;
    }
}

void goods::on_create_clicked()//根据下拉框的内容生成货物排列
{
	scene->clear();
    stack = new myGraphicRectItem(0, 0, stack_x_show, stack_y_show, 0);
	stack->setPos(0, 0);  // 设置左上角位置
	// 禁用选择
	stack->setFlag(QGraphicsRectItem::ItemIsSelectable, false);
	// 确保不会与其他部件发生碰撞
	stack->setZValue(-1);
	// 设置不透明度
	stack->setOpacity(0.2);

    // 将图形项添加到场景中
    scene->addItem(stack);
	//根据下拉框内选择的内容生成货物
	QString currentContent = ui->opSelector->currentText();
    QVector<SingleSKUBpp::LayoutResult> lay_result;
    if (currentContent == "行模式")
	{
        qDebug()<<"1";
        lay_result = myObject->HorizontalLayout(stack_x, stack_y, 1, 1000);
	}
	else if (currentContent == "列模式")
	{
        lay_result = myObject->VerticalLayout(stack_x, stack_y, 1, 1000);
	}
	else if (currentContent == "纵横模式")
	{
        lay_result = myObject->CrossLayout(stack_x, stack_y, 1, 1000);
	}
	else if (currentContent == "单回字模式")
	{
        lay_result = myObject->SingleRectangularLayout(stack_x, stack_y, 1, 1000);
	}
	else if (currentContent == "多回字模式")
	{
        lay_result = myObject->MutiRectangularLayout(stack_x, stack_y, 1, 1000);
	}
	else if (currentContent == "镜像回字模式")
	{
        lay_result = myObject->MutiRectangularLayoutMirror(stack_x, stack_y, 1, 1000);
	}
	else
	{
		QMessageBox::warning(this, "警告", "模式选择错误！");
        return;
	}
    changeForVisual(lay_result);    // 转换为可视化尺寸
    for (const auto& pair : lay_result) {
        //获得坐标为边角所以不作变换
        myGraphicRectItem* rectItem = new myGraphicRectItem(pair.x , pair.y , pair.x_len , pair.y_len , 1);
        scene->addItem(rectItem);
    }

    qDebug()<<"test2";
	//清空x、y、degree显示和myGraphicRectItemInstance指向
	disconnect(this, &goods::updateMyGraphicRectItem, myGraphicRectItemInstance, &myGraphicRectItem::updateFromLineEdit);
	myGraphicRectItemInstance = nullptr;
	ui->select_x->setText(QString::number(0));
	ui->select_y->setText(QString::number(0));
	ui->select_degree->setText(QString::number(0));
}
//根据从newGraphicsView传来的信息更新三个lineEdit的内容
void goods::updateInfo(const ItemInfo& itemInfo)
{
    ui->select_x->setText(QString::number(itemInfo.position.x()));
    ui->select_y->setText(QString::number(itemInfo.position.y()));
	ui->select_degree->setText(QString::number(itemInfo.angle));
}
//发送两个lineEdit的内容到myGraphicRectItemInstance上
void goods::handleLineEditEditingFinished()
{
	// 获取LineEdit中的新值
    QPointF newPosition(ui->select_x->text().toDouble(), ui->select_y->text().toDouble());
	// 发送信号，通知myGraphicRectItem更新
	//qDebug()<<newPosition;
	emit updateMyGraphicRectItem(newPosition);
}
//在鼠标点击对象后进行更新
void goods::InstanceRefresh(myGraphicRectItem* instance)
{
	// 获取与鼠标按住位置相交的图形项
	myGraphicRectItem* item = qgraphicsitem_cast<myGraphicRectItem*>(instance);
	if (item)
	{
		disconnect(this, &goods::updateMyGraphicRectItem, myGraphicRectItemInstance, &myGraphicRectItem::updateFromLineEdit);
		disconnect(this, &goods::instanceRotate, myGraphicRectItemInstance, &myGraphicRectItem::rotateByKey);
		myGraphicRectItemInstance = dynamic_cast<myGraphicRectItem*>(item);
		if (myGraphicRectItemInstance == nullptr)
		{
			//ui->Debugger->setText(QString::number(1));
		}
		else
		{
			//ui->Debugger->setText(QString::number(0));
			connect(this, &goods::updateMyGraphicRectItem, myGraphicRectItemInstance, &myGraphicRectItem::updateFromLineEdit);
			connect(this, &goods::instanceRotate, myGraphicRectItemInstance, &myGraphicRectItem::rotateByKey);
		}
	}

}

//按键响应事件，目前只做了q、e控制顺时针/逆时针转90度
void goods::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_E)
	{
		if (myGraphicRectItemInstance != nullptr)
		{
			qreal num = fmod(ui->select_degree->text().toDouble() - 90, 180);//读数并取余
            ui->select_degree->setText(QString::number(num));
			emit instanceRotate(num);
		}
	}
	else if (event->key() == Qt::Key_Q)
	{
		if (myGraphicRectItemInstance != nullptr)
		{
			qreal num = fmod(ui->select_degree->text().toDouble() + 90, 180);
            ui->select_degree->setText(QString::number(num));
			emit instanceRotate(num);
		}
	}
	QWidget::keyPressEvent(event);
}

//打开输入框请求初始参数的输入
bool goods::openInputDialog()
{
	bool ok;
	QString inputText = QInputDialog::getMultiLineText(nullptr, "输入", "请输入各项数值（用换行分隔）:",
		"stack_x: " + QString::number(stack_x) + "\n"
		"stack_y: " + QString::number(stack_y) + "\n"
		"stack_weight: " + QString::number(stack_weight) + "\n"
		"goods_x: " + QString::number(goods_x) + "\n"
		"goods_y: " + QString::number(goods_y) + "\n"
		"goods_h: " + QString::number(goods_h) + "\n"
		"goods_weight: " + QString::number(goods_weight) + "\n"
		"over_rate: " + QString::number(over_rate), &ok);

	if (ok)
	{
		QStringList values = inputText.split('\n');

		if (values.size() >= 7)
		{
			stack_x = values.value(0).section(':', 1).trimmed().toDouble();
			stack_y = values.value(1).section(':', 1).trimmed().toDouble();
			stack_weight = values.value(2).section(':', 1).trimmed().toDouble();
			goods_x = values.value(3).section(':', 1).trimmed().toDouble();
			goods_y = values.value(4).section(':', 1).trimmed().toDouble();
			goods_h = values.value(5).section(':', 1).trimmed().toDouble();
			goods_weight = values.value(6).section(':', 1).trimmed().toDouble();
			over_rate = values.value(7).section(':', 1).trimmed().toDouble();
		}
		return true;
	}
	return false;
}
//用于获取全部矩形（除栈板矩形）以外的位置
QList<QRectF> goods::getAllRects()
{
	QList<QRectF> rects;
	foreach(QGraphicsItem * item, scene->items())
	{
		if (myGraphicRectItem* myGra = dynamic_cast<myGraphicRectItem*>(item))//转换得到的数据指针为myGraphicRectItem类型
		{
			if (myGra->type() == 1)//排除栈板
			{
				QRectF temp;
				if (myGra->m_RotateAngle == 0)//如果没有旋转过
				{
                    temp.setX(myGra->m_oldRect.x() + myGra->pos().x() );
                    temp.setY(myGra->m_oldRect.y() + myGra->pos().y() );
					temp.setWidth(myGra->m_oldRect.width());
					temp.setHeight(myGra->m_oldRect.height());
					//temp.setX(myGra->m_oldRect.x()+myGra->pos().x());
					//temp.setY(myGra->m_oldRect.y()+myGra->pos().y());
				}
				else//如果旋转过
				{
                    temp.setX(myGra->m_oldRect.x() + myGra->pos().x() );
                    temp.setY(myGra->m_oldRect.y() + myGra->pos().y() );
					temp.setWidth(myGra->m_oldRect.height());
					temp.setHeight(myGra->m_oldRect.width());
					//temp.setX(myGra->m_oldRect.x()+myGra->pos().x()+myGra->m_oldRect.height()/2);
					//temp.setY(myGra->m_oldRect.y()+myGra->pos().y()+myGra->m_oldRect.width()/2);
				}
				rects.append(temp);
			}
		}
	}
	return rects;
}
//存储所有货物矩形的坐标长宽到文件中
void goods::saveLog()
{
	QList<QRectF> nowRects = getAllRects();
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt);;All Files (*)"));

	if (!fileName.isEmpty())
	{
		saveToTxt(fileName, nowRects);
	}
}

//存储所有货物矩形的坐标长宽到sql中
void goods::saveSql()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认", "请确认操作!",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {return;}
    QList<QRectF> nowRects = getAllRects();
    QVector<SingleSKUBpp::LayoutResult> sql_result = getLayOutResult(nowRects);

    for(auto re : sql_result)
    {
        qDebug()<<"re="<<re.x<<re.y<<re.z<<re.x_len<<re.y_len;
    }


    if(sql_result.size()>0)
    {
        emit sendResult(sql_result);
    }

}

QVector<SingleSKUBpp::LayoutResult> goods::getLayOutResult(const QList<QRectF>& widgetRects)
{
    QVector<SingleSKUBpp::LayoutResult> results;
    foreach(QRectF rect, widgetRects)
    {
        double x = rect.x()/30.0;
        double y = rect.y()/30.0;
        double z = current_z;
        double x_len = rect.width()/30.0;
        double y_len = rect.height()/30.0;
        struct SingleSKUBpp::LayoutResult layout_result{x,y,z,x_len,y_len};
        results.append(layout_result);
    }
    return results;

}
//存储内容到txt
void goods::saveToTxt(const QString& fileName, const QList<QRectF>& widgetRects)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
        QTextStream stream(&file);
		foreach(QRectF rect, widgetRects)
        {
            stream << QString("%1 %2 %3 %4\n").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
		}
		file.close();
		QMessageBox::information(this, "Save Success", "Widget information saved to " + fileName);
	}
	else
	{
		QMessageBox::warning(this, "Save Failed", "Unable to save widget information to " + fileName);
	}
}
