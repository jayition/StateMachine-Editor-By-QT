#include "cstatusgraphicsitem.h"

CStatusGraphicsItem::CStatusGraphicsItem(QString Name)
{
    m_Name=Name;
    RefreshRect();
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    this->setAcceptedMouseButtons(Qt::LeftButton);
}

void CStatusGraphicsItem::RefreshRect()
{
    m_font.setFamily("宋体");
    m_font.setPointSize(8);
    QFontMetrics fm(m_font);
    m_TextRect=fm.boundingRect(m_Name);
    m_iwidth=m_TextRect.width()+5;
    m_iheight=m_TextRect.height()+5;
}
QRectF CStatusGraphicsItem::boundingRect() const
{
    return QRectF(0,0,m_iwidth,m_iheight);
}
void CStatusGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(0, 0, m_iwidth, m_iheight);
    painter->setFont(m_font);
    painter->drawText(0, 0, m_iwidth, m_iheight , Qt::AlignCenter, m_Name);

    std::map<string, CStatusGraphicsItem*>::iterator  it;
    for (it = m_OperateCurveList.begin(); it != m_OperateCurveList.end(); ++it)
    {
        QString subname = it->second->m_Name;
        QString finalname=m_Name+"_"+subname;

        QPointF from,to;
        from=QPointF(this->m_iwidth,this->m_iheight/2) ;
        to=it->second->pos()-this->pos();
        to.setY(to.y()+it->second->m_iheight/2);
        points.clear();
        points << from<< to;

        QPainterPath path(points[0]);
        //计算
        for (int i = 0; i < points.size() - 1; ++i) {
            // 控制点的 x 坐标为 sp 与 ep 的 x 坐标和的一半
            // 第一个控制点 c1 的 y 坐标为起始点 sp 的 y 坐标
            // 第二个控制点 c2 的 y 坐标为结束点 ep 的 y 坐标
            QPointF sp = points[i];
            QPointF ep = points[i + 1];
            QPointF c1 = QPointF((sp.x() + ep.x()) / 2, sp.y());
            QPointF c2 = QPointF((sp.x() + ep.x()) / 2, ep.y());
            path.cubicTo(c1, c2, ep);
        }
        //设置渲染提示为消除锯齿
        painter->setRenderHint(QPainter::Antialiasing, true);
        //设置画笔颜色和宽度
        painter->setPen(QPen(Qt::black, 2));
        //绘制path
        painter->drawPath(path);
        // 绘制曲线上的点
        //painter->setBrush(Qt::gray);
        //绘制曲线上的点
        for (int i = 0; i < points.size(); ++i)
        {
            painter->drawEllipse(points[i], 2, 2);
        }
    }


    QPen pen;
    pen.setColor(Qt::red);
    pen.setBrush(QBrush(QColor(Qt::green)));
    painter->setPen(pen);
    painter->drawRect(boundingRect());

}
void CStatusGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseMoveEvent(event);
}



int CStatusGraphicsItem::AddCurveLine(string CurveName,CStatusGraphicsItem* subitem)
{
    m_OperateCurveList[CurveName]=subitem;
    return 0;
}
