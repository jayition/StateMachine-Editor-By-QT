/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "diagramscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>


DiagramScene::DiagramScene(CJStateMachine* sm,QObject *parent)
    : QGraphicsScene(parent)
{
    m_Mode = MoveItem;
    line = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
    m_statemachine=sm;

}

void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
}

void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
}
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
}

void DiagramScene::setFont(const QFont &font)
{
    myFont = font;
}

void DiagramScene::setMode(Mode mode)
{
    m_Mode = mode;
}


void DiagramScene::ClearObjects()
{
    std::map<string, CStatusGraphicsItem*>::iterator  it;
    for (it = m_StatusObjectList.begin(); it !=  m_StatusObjectList.end(); ++it)
    {
        if(it->second)
            delete it->second;
        m_StatusObjectList.erase(it);
    }
}

void DiagramScene::ReLoadObjects()
{
    ClearObjects();
    GenerateStatusObject();
    GenerateLineObject();
    update();
}

void DiagramScene::GenerateStatusObject()
{
    if(m_statemachine==nullptr)
        return ;
    int count=m_statemachine->GetStatusCount();
    int currentx=100;
    int currenty=100;
    int spacew=10;
    int spaceh=10;
    for(int i=0;i<count;i++)
    {
        if(m_StatusObjectList[(*m_statemachine)[i].name]==nullptr)
        {
            CStatusGraphicsItem* item=new CStatusGraphicsItem((*m_statemachine)[i].name.c_str());
            addItem(item);
            item->setPos(currentx,currenty);
            m_StatusObjectList[(*m_statemachine)[i].name]=item;
            int iheight,iwidth;
            iheight=height();
            iwidth=width();
            if((currentx+item->m_iwidth+100)>=iwidth)
            {
                if((currentx+item->m_iheight+100)>=iheight)
                {
                    currentx=100;
                    currenty=100;
                }
                else
                {
                    currentx=100;
                    currenty+=item->m_iheight+spaceh;
                }
            }
            else
            {
                 currentx+=item->m_iwidth+spacew;
            }
        }
    }
}

void DiagramScene::GenerateLineObject()
{
    if(m_statemachine==nullptr)
        return ;
    std::map<string, CStatusGraphicsItem*>::iterator  it;
    string startname,nextname,operatename,name;

    int count=m_statemachine->GetStatusCount();
    for(int i=0;i<count;i++)
    {
        string statename=(*m_statemachine)[i].name;
        if(m_StatusObjectList[statename]!=nullptr)
        {
            startname=(*m_statemachine)[i].name;
            std::map<string, CJOperate>::iterator  itoperate;
            for (itoperate = (*m_statemachine)[i].m_StatusOperateList.begin(); itoperate !=  (*m_statemachine)[i].m_StatusOperateList.end(); ++itoperate)
            {
                operatename=itoperate->second.name;
                nextname=itoperate->second.m_NextState->name;
                m_StatusObjectList[statename]->AddCurveLine(operatename,m_StatusObjectList[nextname]);
            }
        }
    }
    update();
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QTransform trasfrom;
    if (mouseEvent->button() != Qt::LeftButton)
    {
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    update();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DiagramScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    update();
    QGraphicsScene::wheelEvent(event);
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool DiagramScene::isItemChange(int type) const
{

}

