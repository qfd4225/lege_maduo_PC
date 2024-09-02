#ifndef NEWGRAPHICSVIEW_H
#define NEWGRAPHICSVIEW_H

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include "mygraphicrectitem.h"

struct ItemInfo
{
    QPointF position;
    qreal angle;
};

class NewGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    NewGraphicsView(QWidget* scene);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void itemPosSend(const ItemInfo& itemInfo);
    void itemChange(myGraphicRectItem* instance);
private:
    bool isDragging;

};

#endif // NEWGRAPHICSVIEW_H
