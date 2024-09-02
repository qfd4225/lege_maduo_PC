#ifndef MYGRAPHICRECTITEM_H
#define MYGRAPHICRECTITEM_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRect>
#include <QPainter>
#include <QPolygon>
#include <QList>

enum STATE_FLAG{
    DEFAULT_FLAG=0,
    MOV_LEFT_LINE,//标记当前为用户按下矩形的左边界区域
    MOV_TOP_LINE,//标记当前为用户按下矩形的上边界区域
    MOV_RIGHT_LINE,//标记当前为用户按下矩形的右边界区域
    MOV_BOTTOM_LINE,//标记当前为用户按下矩形的下边界区域
    MOV_RIGHTBOTTOM_RECT,//标记当前为用户按下矩形的右下角
    MOV_RECT,//标记当前为鼠标拖动图片移动状态
    ROTATE//标记当前为旋转状态
};
class myGraphicRectItem:public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    myGraphicRectItem(qreal x=0,qreal y=0,qreal width=100,qreal height=100,int type=0,QGraphicsItem *parent = nullptr);
    //myGraphicRectItem(QRectF m_OriginRect = QRectF(0,0,100,100));
    QRectF  boundingRect() const;
    //QPainterPath shape();
    ~myGraphicRectItem();
    void setRectSize(QRectF mrect,bool bResetRotateCenter = true);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    QPointF getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle);//获取旋转后的点
    QList<QPointF> getRotatePoints(QPointF ptCenter,QList<QPointF> ptIns,qreal angle);//获取多个旋转后的点
    qfloat16 rotateDegree;
    QPolygonF getRotatePolygonFromRect(QPointF ptCenter,QRectF rectIn,qreal angle);//将矩形旋转之后返回多边形
    QRectF getCrtPosRectToSceen();
    bool    m_bRotate;
    qreal   m_RotateAngle;
    QPointF m_RotateCenter;
    QRectF  m_oldRect;
    int type();
    int m_type;
    bool collidited;
    QPointF m_startPos;
    QPolygonF m_insicedPolygon;
    QRectF  m_insicedRectf;
    STATE_FLAG m_StateFlag;

public slots:
    //根据得到的参数进行位移
    void updateFromLineEdit(const QPointF& newPosition);
    void rotateByKey(qreal degree);

private:
    void SetRotate(qreal RotateAngle,QPointF ptCenter=QPointF(-999,-999));
    QPolygonF m_oldRectPolygon;
    QRectF  m_RotateAreaRect;
    bool    m_bResize;
    QPolygonF m_leftUpPolygon;
    QRectF  m_leftUpRectf;
    QPolygonF m_leftDownPolygon;
    QRectF  m_leftDownRectf;
    QPolygonF m_rightUpPolygon;
    QRectF  m_rightUpRectf;
    QPolygonF m_rightDownPolygon;
    QRectF  m_rightDownRectf;
    //    QPolygonF m_rbPolygon;
    //    QRectF  m_rbRectf;

    //QPointF *pPointFofSmallRotateRect;

};
#endif // MYGRAPHICRECTITEM_H
