#include "mygraphicrectitem.h"
#include <QtMath>
#include <QDebug>

myGraphicRectItem::myGraphicRectItem(qreal x,qreal y,qreal width,qreal height,int type,QGraphicsItem *parent):
    QGraphicsRectItem(x,y,width,height,parent),
    m_bRotate(false),
    m_RotateAngle(0),
    m_oldRect(x,y,width,height),
    m_bResize(false),
    m_StateFlag(DEFAULT_FLAG),
    collidited(false),
    rotateDegree(0),
    m_type(type)
{
    //setParent(parent);
    setRectSize(m_oldRect);
    setToolTip("Click and drag me!");//提示
    setCursor(Qt::ArrowCursor);//改变光标形状,手的形状
    setFlag(QGraphicsItem::ItemIsMovable);
    //setAcceptDrops(true);
    //pPointFofSmallRotateRect = new QPointF[4];//用于存储
    SetRotate(0);//设置初始旋转角度为0度
    setFlag(QGraphicsItem::ItemIsSelectable);//设置图形项可以选择
}
/*
//重写的shape函数，返回原始包围盒在旋转后的坐标系中的形状，由于旋转默认绕图形左上角，所以旋转前后要改变旋转中心
QPainterPath myGraphicRectItem::shape()
{
    QPainterPath path;
    path.addRect(m_oldRect);
    QTransform transform;
    transform.translate(m_oldRect.x()+m_oldRect.width()/2, m_oldRect.y()+m_oldRect.height()/2);
    transform.rotate(m_RotateAngle);
    transform.translate(-m_oldRect.x()-m_oldRect.width()/2, -m_oldRect.y()-m_oldRect.height()/2);
    return transform.map(path);
}
*/
// 重写的 boundingRect 函数，返回图形项的边界矩形
QRectF myGraphicRectItem::boundingRect() const
{
    QRectF boundingRectF = m_oldRectPolygon.boundingRect();
    //return QRectF(boundingRectF.x()-40,boundingRectF.y()-40,boundingRectF.width()+80,boundingRectF.height()+80);
    return boundingRectF;
}

// myGraphicRectItem 类的析构函数，释放动态分配的内存
myGraphicRectItem::~myGraphicRectItem()
{
    //delete []pPointFofSmallRotateRect;
    //pPointFofSmallRotateRect = nullptr;
}

// 设置图形项的大小和旋转中心
void myGraphicRectItem::setRectSize(QRectF mrect, bool bResetRotateCenter)
{
    // 一系列计算和更新图形项属性的操作
    m_oldRect = mrect;
    if(bResetRotateCenter)
    {
        m_RotateCenter.setX(m_oldRect.x()+m_oldRect.width()/2);
        m_RotateCenter.setY(m_oldRect.y()+m_oldRect.height()/2);
    }
    m_oldRectPolygon = getRotatePolygonFromRect(m_RotateCenter,m_oldRect,m_RotateAngle);//根据修改后的参数计算新的点坐标
    m_insicedRectf = QRectF(m_oldRect.x(),m_oldRect.y(),m_oldRect.width(),m_oldRect.height());
    m_insicedPolygon = getRotatePolygonFromRect(m_RotateCenter,m_insicedRectf,m_RotateAngle);
}

// 重写的 paint 函数，用于绘制图形项的外观
void myGraphicRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    QPen mPen = QPen(Qt::black);
    mPen.setWidth(1);
    painter->setPen(mPen);
    //绘制旋转后的矩形
    if(collidited)
    {
        painter->setBrush(Qt::red);
    }
    else
    {
        painter->setBrush(QColor(194, 178, 128));//货箱设置为褐色
    }
    if(this->type()==0)
    {
        painter->setBrush(QColor(152, 251, 152));//栈板设置为浅绿色
    }
    painter->drawPolygon(m_oldRectPolygon);
}

// 鼠标按下事件处理函数
void myGraphicRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标
        if(m_insicedPolygon.containsPoint(m_startPos,Qt::WindingFill))//在矩形内框区域时按下鼠标，则可拖动对象
        {
            setCursor(Qt::ClosedHandCursor);   //改变光标形状,手的形状
            m_StateFlag = MOV_RECT;//标记当前为鼠标拖动图片移动状态
        }
        else
        {
            m_StateFlag = DEFAULT_FLAG;
        }
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

//鼠标移动事件处理函数
void myGraphicRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 根据状态标志进行相应的操作，如旋转、拖动等
    if(m_StateFlag == MOV_RECT)
    {
        QPointF point = (event->pos() - m_startPos);
        moveBy(point.x(), point.y());
        setRectSize(m_oldRect);
        scene()->update();
        QList<QGraphicsItem *> collidItem = scene()->collidingItems(this,Qt::IntersectsItemBoundingRect);
        qDebug()<<"Size:"<<collidItem.size();
        qreal thisWidth = (m_RotateAngle == 0) ? m_oldRect.width() : m_oldRect.height();
        qreal thisHeight = (m_RotateAngle == 0) ? m_oldRect.height() : m_oldRect.width();
        for (auto it = collidItem.begin(); it != collidItem.end();)
        {
            myGraphicRectItem *rectItem = qgraphicsitem_cast<myGraphicRectItem*>(*it);
            if (rectItem->type() != 1)
            {
                it = collidItem.erase(it); // 删除符合条件的项
            }
            else
            {
                qreal thatWidth = (rectItem->m_RotateAngle == 0)?rectItem->m_oldRect.width():rectItem->m_oldRect.height();
                qreal thatHeight = (rectItem->m_RotateAngle == 0)?rectItem->m_oldRect.height():rectItem->m_oldRect.width();

                qreal thisx = m_oldRect.x() + this->pos().x() + m_oldRect.width() / 2;
                qreal thisy = m_oldRect.y() + this->pos().y() + m_oldRect.height() / 2;

                qreal thatx = rectItem->m_oldRect.x() + rectItem->pos().x() + rectItem->m_oldRect.width()/2;
                qreal thaty = rectItem->m_oldRect.y() + rectItem->pos().y() + rectItem->m_oldRect.height()/2;

                qreal xidealDis = thisWidth / 2 + thatWidth / 2;
                qreal yidealDis = thisHeight / 2 + thatHeight / 2;

                if (!(qAbs(thisx - thatx) < xidealDis && qAbs(thisy - thaty) < yidealDis))
                {
                    it = collidItem.erase(it); // 删除符合条件的项
                }
                else
                {
                    // 处理与其他类型的对象发生碰撞的情况
                    ++it;
                }
            }
        }
        if(!collidItem.isEmpty())
        {
            collidited=true;
            moveBy(-point.x(), -point.y());
            setRectSize(m_oldRect);
        }
        else
        {
            collidited=false;
        }
        scene()->update();
    }
}

// 鼠标释放事件处理函数
void myGraphicRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if(m_StateFlag == MOV_RECT)
    {
        m_StateFlag = DEFAULT_FLAG;
    }
    else
    {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

// 设置旋转状态和相关属性
void myGraphicRectItem::SetRotate(qreal RotateAngle, QPointF ptCenter)
{
    m_bRotate = true;

    if(ptCenter.x()==-999 && ptCenter.y()==-999)
    {
        m_RotateCenter = QPointF(m_oldRect.x()+m_oldRect.width()/2,m_oldRect.y()+m_oldRect.height()/2);
    }
    else
    {
        m_RotateCenter = ptCenter;
    }
    m_RotateAngle = RotateAngle;
    this->update();
}

//根据得到的参数进行位移
void myGraphicRectItem::updateFromLineEdit(const QPointF& newPosition)
{
    QPointF start(m_oldRect.x()+this->pos().x()+m_oldRect.width()/2,m_oldRect.y()+this->pos().y()+m_oldRect.height()/2);
    //qDebug()<<"m_oldRect.x():"<<m_oldRect.x()<<" m_oldRect.y():"<<m_oldRect.y();
    //qDebug()<<"this->pos().x():"<<this->pos().x()<<" this->pos().y():"<<this->pos().y();
    //qDebug()<<"newPosition.x():"<<newPosition.x()<<" newPosition.y():"<<newPosition.y();
    QPointF point = (newPosition - start);
    moveBy(point.x(), point.y());
}

// 获取旋转后的点
QPointF myGraphicRectItem::getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle)
{
    // 计算旋转后的点的坐标
    double dx = ptCenter.x();
    double dy = ptCenter.y();
    double x = ptIn.x();
    double y = ptIn.y();
    double xx,yy;
    if(angle ==90 ||angle==-90)
    {
        xx = -(y-dy)*sin(angle*M_PI/180)+dx;
        //qDebug()<<"cos(angle*M_PI/180):"<<cos(angle*M_PI/180);
        yy = (x-dx)*sin(angle*M_PI/180)+dy;
    }
    else
    {
        xx = (x-dx)-(y-dy)*sin(angle*M_PI/180)+dx;
        //qDebug()<<"cos(angle*M_PI/180):"<<cos(angle*M_PI/180);
        yy = (x-dx)*sin(angle*M_PI/180)+(y-dy)+dy;
    }
    //xx = (x-dx)*cos(angle*M_PI/180)-(y-dy)*sin(angle*M_PI/180)+dx;
    //qDebug()<<"cos(angle*M_PI/180):"<<cos(angle*M_PI/180);
    //yy = (x-dx)*sin(angle*M_PI/180)+(y-dy)*cos(angle*M_PI/180)+dy;
    return QPointF(xx,yy);
}

// 获取旋转后的多边形
QList<QPointF> myGraphicRectItem::getRotatePoints(QPointF ptCenter, QList<QPointF> ptIns, qreal angle)
{
    // 计算多个点的旋转后的坐标
    QList<QPointF> lstPt;
    for(int i = 0;i<ptIns.count();i++)
    {
        lstPt.append(getRotatePoint(ptCenter,ptIns.at(i),angle));
    }
    return lstPt;
}


// 获取旋转后的多边形（从矩形获取）
QPolygonF myGraphicRectItem::getRotatePolygonFromRect(QPointF ptCenter, QRectF rectIn, qreal angle)
{
    // 计算矩形的旋转后的多边形
    QVector<QPointF> vpt;
    QPointF pf = getRotatePoint(ptCenter,rectIn.topLeft(),angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter,rectIn.topRight(),angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter,rectIn.bottomRight(),angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter,rectIn.bottomLeft(),angle);
    vpt.append(pf);
    return QPolygonF(vpt);
}

// 获取图形项在场景坐标系中的位置
QRectF myGraphicRectItem::getCrtPosRectToSceen()
{
    QRectF retRect = QRectF(m_oldRect.x()+pos().x(),m_oldRect.y()+pos().y(),m_oldRect.width(),m_oldRect.height());
    return retRect;
}

//接收到主页面发来的按键旋转信号后进行处理
void myGraphicRectItem::rotateByKey(qreal degree)
{
    SetRotate(degree);
    setRectSize(m_oldRect);
    QList<QGraphicsItem *> collidItem = scene()->collidingItems(this,Qt::IntersectsItemBoundingRect);
    qreal thisWidth = (m_RotateAngle == 0) ? m_oldRect.width() : m_oldRect.height();
    qreal thisHeight = (m_RotateAngle == 0) ? m_oldRect.height() : m_oldRect.width();
    for (auto it = collidItem.begin(); it != collidItem.end();)
    {
        myGraphicRectItem *rectItem = qgraphicsitem_cast<myGraphicRectItem*>(*it);
        if (rectItem->type() != 1)
        {
            it = collidItem.erase(it); // 删除符合条件的项
        }
        else
        {
            qreal thatWidth = (rectItem->m_RotateAngle == 0)?rectItem->m_oldRect.width():rectItem->m_oldRect.height();
            qreal thatHeight = (rectItem->m_RotateAngle == 0)?rectItem->m_oldRect.height():rectItem->m_oldRect.width();

            qreal thisx = m_oldRect.x() + this->pos().x() + m_oldRect.width() / 2;
            qreal thisy = m_oldRect.y() + this->pos().y() + m_oldRect.height() / 2;

            qreal thatx = rectItem->m_oldRect.x() + rectItem->pos().x() + rectItem->m_oldRect.width()/2;
            qreal thaty = rectItem->m_oldRect.y() + rectItem->pos().y() + rectItem->m_oldRect.height()/2;

            qreal xidealDis = thisWidth / 2 + thatWidth / 2;
            qreal yidealDis = thisHeight / 2 + thatHeight / 2;

            if (!(qAbs(thisx - thatx) < xidealDis && qAbs(thisy - thaty) < yidealDis))
            {
                it = collidItem.erase(it); // 删除符合条件的项
            }
            else
            {
                // 处理与其他类型的对象发生碰撞的情况
                ++it;
            }
        }
    }
    if(collidited==true)
    {
        rotateDegree=std::fmod(90-degree,180);
        SetRotate(rotateDegree);
    }
    setRectSize(m_oldRect);
    scene()->update();
    /*
    SetRotate(degree);
    setRectSize(m_oldRect);
    QList<QGraphicsItem *> collidItem = scene()->collidingItems(this);
    for (auto it = collidItem.begin(); it != collidItem.end(); )
    {
        myGraphicRectItem *rectItem = qgraphicsitem_cast<myGraphicRectItem*>(*it);
        if(rectItem->type()!=1)
        {
            it = collidItem.erase(it);
        }
        else
        {
            it++;
        }
    }
    if(!collidItem.isEmpty())
    {
        collidited=true;
        rotateDegree=std::fmod(90-degree,180);
        SetRotate(rotateDegree);
        setRectSize(m_oldRect);
    }
    else
    {
        collidited=false;
    }
    */
    scene()->update();
}

int myGraphicRectItem::type()
{
    return m_type; // 用户自定义类型的值，货物为1，底盘为0
}

