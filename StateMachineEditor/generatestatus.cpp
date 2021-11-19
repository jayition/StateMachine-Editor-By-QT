#include "generatestatus.h"
#include "ui_generatestatus.h"

GenerateStatus::GenerateStatus(CJStateMachine* sm,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GenerateStatus)
{
    m_statemachine=sm;
    ui->setupUi(this);
     UpdateStateList();
}

GenerateStatus::~GenerateStatus()
{
    delete ui;
}
void  GenerateStatus::SetNeedRefresh()
{
    emit NeedRefresh();
}
void GenerateStatus::UpdateStateList()
{
    if(m_statemachine)
    {
        ui->listWidget_SystemStatus->clear();
        int count=m_statemachine->GetStatusCount();
        for(int i=0;i<count;i++)
        {
            ui->listWidget_SystemStatus->addItem((*m_statemachine)[i].name.c_str());
        }
    }
}

void GenerateStatus::on_pushButton_Add_clicked()
{
    QString name=ui->lineEdit_SubStatusName->text();
    if(name.length()<=0)
        return;
    ui->listWidget_SubStatus->addItem(name);
}

void GenerateStatus::on_pushButton_Delete_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidget_SubStatus->selectedItems();
    if (selectedItems.count() > 0)
    {
        foreach (QListWidgetItem* var, selectedItems) {
            ui->listWidget_SubStatus->removeItemWidget(var);
            selectedItems.removeOne(var);
            delete var;
        }
    }
}

void GenerateStatus::on_pushButton_Generate_clicked()
{
    QList<QListWidgetItem*> selectedSystemStatusItems = ui->listWidget_SystemStatus->selectedItems();
    if (selectedSystemStatusItems.count() > 0)
    {
        foreach (QListWidgetItem* var, selectedSystemStatusItems)
        {
           string name = var->text().toStdString();
           QList<QListWidgetItem*> selectedItems = ui->listWidget_SubStatus->selectedItems();
           if (selectedItems.count() > 0)
           {
               foreach (QListWidgetItem* subvar, selectedItems)
               {
                  string subname = subvar->text().toStdString();
                  string finalname=name+"_"+subname;
                  m_statemachine->AddStatus(finalname);
               }
           }
        }
    }
    SetNeedRefresh();
}

void GenerateStatus::on_pushButton_Refresh_clicked()
{
    UpdateStateList();
}

void GenerateStatus::on_pushButton_Hide_clicked()
{
     this->hide();
}


