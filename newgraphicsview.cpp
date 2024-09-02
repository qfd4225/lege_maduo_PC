#include "NewGraphicsView.h"
#include "mygraphicrectitem.h"
NewGraphicsView::NewGraphicsView(QWidget* scene)
    : QGraphicsView(scene)
{
}

void NewGraphicsView::mousePressEvent(QMouseEvent* event)
{
    // 在鼠标点击事件中获取鼠标点击位置
    QPointF scenePos = mapToScene(event->pos());
    // 获取与鼠标点击位置相交的图形项
    QGraphicsItem* item = scene()->itemAt(scenePos, transform());
    // 如果有图形项被选中
    if (item)
    {
        // 发送图形项的位置和角度信息
        myGraphicRectItem* item2 =qgraphicsitem_cast<myGraphicRectItem*>(item);
        ItemInfo itemInfo;
        itemInfo.position.setX(item2->m_oldRect.x()+item2->pos().x()+item2->m_oldRect.width()/2);
        itemInfo.position.setY(item2->m_oldRect.y()+item2->pos().y()+item2->m_oldRect.height()/2);
        itemInfo.angle = item2->m_RotateAngle;
        isDragging = true;
        emit itemPosSend(itemInfo);
        emit itemChange(item2);
    }
    else
    {
        isDragging = false;
    }
    QGraphicsView::mousePressEvent(event);
}

void NewGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if(isDragging)
    {
        // 在鼠标移动事件中获取鼠标点击位置
        QPointF scenePos = mapToScene(event->pos());
        // 获取与鼠标按住位置相交的图形项
        myGraphicRectItem* item = qgraphicsitem_cast<myGraphicRectItem*>(scene()->itemAt(scenePos, transform()));
        if (item)
        {
            // 发送图形项的位置和角度信息
            ItemInfo itemInfo;
            itemInfo.position.setX(item->m_oldRect.x()+item->pos().x()+item->m_oldRect.width()/2);
            itemInfo.position.setY(item->m_oldRect.y()+item->pos().y()+item->m_oldRect.height()/2);
            itemInfo.angle = item->m_RotateAngle;
            emit itemPosSend(itemInfo);
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void NewGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    setCursor(Qt::ArrowCursor);
    if (isDragging)
    {
        isDragging = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void NewGraphicsView::wheelEvent(QWheelEvent *event)
{
    qreal factor = 1.2;
    if (event->angleDelta().y() < 0)
    {
        factor = 1.0 / factor;
    }

    scale(factor, factor);
}
