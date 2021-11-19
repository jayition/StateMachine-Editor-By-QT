#include "addstatus.h"
#include "ui_addstatus.h"

AddStatus::AddStatus(CJStateMachine* sm,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddStatus)
{
    m_statemachine=sm;
    ui->setupUi(this);
}

AddStatus::~AddStatus()
{
    m_statemachine=nullptr;
    delete ui;
}

void AddStatus::on_pb_OK_clicked()
{
    if(m_statemachine)
    {
        m_statemachine->AddStatus(ui->lineEdit_StatusName->text().toStdString());
        SetNeedRefresh();
        ui->lineEdit_StatusName->setText("");
    }
}

void  AddStatus::SetNeedRefresh()
{
    emit NeedRefresh();
}

void AddStatus::on_pushButton_Hide_clicked()
{
    this->hide();
}
