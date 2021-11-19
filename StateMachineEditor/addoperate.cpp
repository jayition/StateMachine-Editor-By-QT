#include "addoperate.h"
#include "ui_addoperate.h"

AddOperate::AddOperate(CJStateMachine* sm,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddOperate)
{
    m_statemachine=sm;
    ui->setupUi(this);
    UpdateOperateList();
    UpdateStateList();
    m_CurrentStatus="";
}

AddOperate::~AddOperate()
{
    m_statemachine=nullptr;
    delete ui;
}
void  AddOperate::SetNeedRefresh()
{
    emit NeedRefresh();
}
void AddOperate::on_pb_OK_clicked()
{
    bool generateSubStatus=ui->checkBox_AddSubStatus->isChecked();
    bool OperateBack=ui->checkBox_OperateBack->isChecked();
    string substatename=ui->lineEdit_SubStateName->text().toStdString();
    string prestatuscutoff=ui->lineEdit_PreStatusCutOff->text().toStdString();
    if(ui->listWidget_OperateAvailable->currentItem()==nullptr)
        return ;
    string operate=ui->listWidget_OperateAvailable->currentItem()->text().toStdString();
    string nextstate="";
    if(!generateSubStatus)
        nextstate=ui->listWidget_NextStausList->currentItem()->text().toStdString();
    if(operate.length()<=0)
        return ;
    if(!generateSubStatus&&nextstate.length()<=0)
        return ;
    if(m_statemachine)
    {
        QList<QListWidgetItem*> selectedSystemStatusItems = ui->listWidget_StatusList->selectedItems();
        if (selectedSystemStatusItems.count() > 0)
        {
            foreach (QListWidgetItem* var, selectedSystemStatusItems)
            {
               string name = var->text().toStdString();
               if(generateSubStatus)
               {
                   string prename=name;
                   if(prename.length()>prestatuscutoff.length())
                   {
                       string substr=prename.substr(prename.length()-prestatuscutoff.length(),prestatuscutoff.length());
                       if(substr==prestatuscutoff)
                       {
                           prename=prename.substr(0,prename.length()-prestatuscutoff.length());
                           if(prename[prename.length()-1]=='_'||prename[prename.length()-1]==' ')
                           {
                               prename=prename.substr(0,prename.length()-1);
                           }
                       }
                   }
                   string subname;
                   if(substatename.length()<=0)
                       subname=prename+"_"+operate;
                   else
                       subname=prename+"_"+substatename;
                    m_statemachine->AddStatus(subname);
                    m_statemachine->SetOperateNextStatus(name,operate,subname);
                    if(OperateBack)
                    {
                        m_statemachine->SetOperateNextStatus(subname,operate,name);
                    }
               }
               else
               {
                   m_statemachine->SetOperateNextStatus(name,operate,nextstate);
               }
            }
        }
    }
    SetNeedRefresh();
}

void AddOperate::UpdateOperateList()
{
    if(m_statemachine)
    {
        ui->listWidget_OperateAvailable->clear();
        int count=m_statemachine->GetOperateCount();
        for(int i=0;i<count;i++)
        {
            ui->listWidget_OperateAvailable->addItem(m_statemachine->GetOperate(i).c_str());
        }
    }
}
void AddOperate::SetCurrentStatus(string status)
{
    m_CurrentStatus=status;
    ui->lineEdit_StatusSrcFilter->setText(m_CurrentStatus.c_str());
    ui->lb_CurrentStatus->setText(m_CurrentStatus.c_str());
}
void AddOperate::UpdateStateList()
{
    if(m_statemachine)
    {
        ui->listWidget_NextStausList->clear();
        ui->listWidget_StatusList->clear();
        int count=m_statemachine->GetStatusCount();
        for(int i=0;i<count;i++)
        {
            ui->listWidget_NextStausList->addItem((*m_statemachine)[i].name.c_str());
            ui->listWidget_StatusList->addItem((*m_statemachine)[i].name.c_str());
        }
    }
}
void AddOperate::on_pb_Add_OperateAvailable_clicked()
{
    if(m_statemachine)
    {
        m_statemachine->AddOperate(ui->lineEdit_2->text().toStdString());
        UpdateOperateList();
        ui->lineEdit_2->setText("");
    }
}

void AddOperate::SetCurrentOperate(string operate,string nextstate)
{
    QList<QListWidgetItem *> list;
    ui->listWidget_OperateAvailable->selectedItems().clear();
    list=ui->listWidget_OperateAvailable->findItems(operate.c_str(),Qt::MatchFlag::MatchFixedString);
    for(int i=0;i<list.size();i++)
    {
        list[i]->setSelected(true);
        ui->listWidget_OperateAvailable->scrollToItem(list[i], QAbstractItemView::PositionAtTop);
    }
    list.clear();
    ui->listWidget_NextStausList->selectedItems().clear();
    list=ui->listWidget_NextStausList->findItems(nextstate.c_str(),Qt::MatchFlag::MatchFixedString);
    for(int i=0;i<list.size();i++)
    {
        list[i]->setSelected(true);
        ui->listWidget_NextStausList->scrollToItem(list[i], QAbstractItemView::PositionAtTop);
    }
}

void AddOperate::on_pushButton_Hide_clicked()
{
     this->hide();
}

void AddOperate::on_lineEdit_OperateFilter_textChanged(const QString &arg1)
{
    QList<QListWidgetItem *> list;
    list=ui->listWidget_OperateAvailable->findItems(arg1,Qt::MatchFlag::MatchContains);
    for(int i=0;i<list.size();i++)
    {
        ui->listWidget_OperateAvailable->scrollToItem(list[i], QAbstractItemView::PositionAtTop);
        return;
    }
}

void AddOperate::on_lineEdit_StatusFilter_textChanged(const QString &arg1)
{
    QList<QListWidgetItem *> list;
    list=ui->listWidget_NextStausList->findItems(arg1,Qt::MatchFlag::MatchContains);
    for(int i=0;i<list.size();i++)
    {
        ui->listWidget_NextStausList->scrollToItem(list[i], QAbstractItemView::PositionAtTop);
        return;
    }
}

void AddOperate::on_lineEdit_StatusSrcFilter_textChanged(const QString &arg1)
{
    QList<QListWidgetItem *> list;
    list=ui->listWidget_StatusList->findItems(arg1,Qt::MatchFlag::MatchContains);
    for(int i=0;i<list.size();i++)
    {
        ui->listWidget_StatusList->scrollToItem(list[i], QAbstractItemView::PositionAtTop);
        return;
    }
}

void AddOperate::on_checkBox_StatusMultSelect_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->listWidget_StatusList->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else
    {
        ui->listWidget_StatusList->setSelectionMode(QAbstractItemView::MultiSelection);
    }
}



void AddOperate::on_pushButton_Refresh_clicked()
{
    UpdateOperateList();
    UpdateStateList();
}

void AddOperate::on_checkBox_AddSubStatus_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->listWidget_NextStausList->setEnabled(true);
    }
    else
    {
        ui->listWidget_NextStausList->setEnabled(false);
    }
}

void AddOperate::on_listWidget_OperateAvailable_currentTextChanged(const QString &currentText)
{
    ui->lineEdit_SubStateName->setText(currentText);
}
