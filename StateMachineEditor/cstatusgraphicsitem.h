#ifndef CSTATUSGRAPHICSITEM_H
#define CSTATUSGRAPHICSITEM_H
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qpainter.h>
#include <qdebug.h>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <string>
#include <qstring.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qinputdialog.h>
#include <QTextItem>
#include <qmenu.h>
#include "CJStateMachine.h"

class CStatusGraphicsItem : public QGraphicsItem
{
public:
    CStatusGraphicsItem(QString Name = "");
    int m_iheight = 20;
    int m_iwidth = 80;
    QString m_Name;
    int AddCurveLine(string CurveName,CStatusGraphicsItem* subitem);
    void RefreshRect();
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
private:
     QFont m_font;
     QRect m_TextRect;
     map<string, CStatusGraphicsItem*> m_OperateCurveList;//存放子对像的清单
     QList<QPointF> points;
};

#endif // CSTATUSGRAPHICSITEM_H
