#ifndef GOODS_H
#define GOODS_H

#include "qgraphicsscene.h"
#include <QWidget>
#include "newgraphicsview.h"
#include <QString>
#include "mygraphicrectitem.h"
#include "single_SKU.h"

namespace Ui {
	class goods;
}


class goods : public QWidget
{
	Q_OBJECT

public:
    explicit goods(double p_stack_x ,double p_stack_y,double p_stack_z,double p_goods_x,double p_goods_y,double p_goods_h,double p_stack_weight,double p_goods_weight,
                   double p_over_rate,int p_max_num , QWidget* parent = nullptr);
	~goods();
	bool is_created = false;
    void refreshLayout(QVector<SingleSKUBpp::LayoutResult> layout_result);
private:
	Ui::goods* ui;
	QGraphicsScene* scene = new QGraphicsScene;
	myGraphicRectItem* myGraphicRectItemInstance;
	void InstanceRefresh(myGraphicRectItem* instance);
	void keyPressEvent(QKeyEvent* event);
    double stack_x, stack_y,stack_z;
	double goods_x, goods_y, goods_h, stack_weight, goods_weight;
	double over_rate;
    double stack_x_show, stack_y_show;
    double goods_x_show, goods_y_show;
    double current_z;
	bool openInputDialog();
	myGraphicRectItem* stack;
	SingleSKUBpp* myObject;
	QList<QRectF> getAllRects();
	void saveToTxt(const QString& fileName, const QList<QRectF>& widgetRects);

    void changeForVisual(QVector<SingleSKUBpp::LayoutResult> &layout);

    QVector<SingleSKUBpp::LayoutResult> getLayOutResult(const QList<QRectF> &widgetRects);
private slots:
	void on_create_clicked();
	void updateInfo(const ItemInfo& itemInfo);
	void handleLineEditEditingFinished();
	void saveLog();
    void saveSql();

signals:
	void updateMyGraphicRectItem(const QPointF& newPosition);
	void instanceRotate(qreal degree);
	void returnToEditMode();
    void sendResult(QVector<SingleSKUBpp::LayoutResult> layout);
};

#endif // GOODS_H
