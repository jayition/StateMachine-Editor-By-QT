#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_addstatusDlg=nullptr;
    m_addoperateDlg=nullptr;
    m_generatestatusDlg=nullptr;
    m_DefaultFileName="";
    m_statemachine.Load("D:\\US_StateMachine.xml");
    m_TreeScene = new DiagramScene(&m_statemachine,this);
    m_TreeScene->setSceneRect(QRectF(0, 0, 1000, 1000));
    ui->gv_Tree->setScene(m_TreeScene);
    m_TreeScene->setMode(DiagramScene::InsertItem);
    m_TreeScene->GenerateStatusObject();
    m_TreeScene->GenerateLineObject();
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pb_Status_Add_clicked()
{
    if(m_addoperateDlg)
        m_addoperateDlg->hide();
    if(m_generatestatusDlg)
        m_generatestatusDlg->hide();
    ui->gv_Tree->hide();
    if(m_addstatusDlg==nullptr)
    {
         m_addstatusDlg=new AddStatus(&m_statemachine,ui->widgetRight);
         connect(m_addstatusDlg, &AddStatus::NeedRefresh, this, &MainWindow::StatusNeedRefresh);
    }
    m_addstatusDlg->show();
    if(m_addstatusDlg)
        m_addstatusDlg->setGeometry(0,0,ui->widgetRight->width(),ui->widgetRight->height());
    /*m_statemachine.AddStatus("A");
    m_statemachine.AddStatus("B");
    m_statemachine.AddStatus("C");
    m_statemachine.AddStatus("D");
    m_statemachine.AddStatus("E");
    m_statemachine.AddStatus("F");
    m_statemachine.AddStatus("G");
    m_statemachine.AddStatus("H");
    m_statemachine.AddOperate("2B");
    m_statemachine.AddOperate("2C");
    m_statemachine.AddOperate("2D");
    m_statemachine.AddOperate("2E");
    m_statemachine.AddOperate("2F");
    m_statemachine.AddOperate("2G");
    m_statemachine.AddOperate("2H");
    m_statemachine.AddOperate("2A");
    m_statemachine.SetOperateNextStatus("A","2B","B");
    m_statemachine.SetOperateNextStatus("A","2C","C");
    m_statemachine.SetOperateNextStatus("A","2D","D");
    m_statemachine.SetOperateNextStatus("A","2F","F");
    m_statemachine.SetOperateNextStatus("C","2A","A");
    m_statemachine.SetOperateNextStatus("B","2C","C");
    m_statemachine.SetOperateNextStatus("B","2D","D");
    m_statemachine.SetOperateNextStatus("C","2F","F");
    m_statemachine.SetOperateNextStatus("E","2C","C");
    m_statemachine.SetOperateNextStatus("E","2D","D");
    m_statemachine.SetOperateNextStatus("E","2A","A");
    m_statemachine.SetOperateNextStatus("F","2A","A");
    m_statemachine.SetOperateNextStatus("F","2C","C");
    m_statemachine.SetOperateNextStatus("G","2F","F");
    m_statemachine.SetOperateNextStatus("G","2A","A");
    m_statemachine.SetOperateNextStatus("H","2C","C");
    m_statemachine.SetOperateNextStatus("H","2D","D");
    m_statemachine.SetOperateNextStatus("H","2E","E");
    m_statemachine.SetOperateNextStatus("H","2F","F");
    m_statemachine.SetOperateNextStatus("F","2H","H");
    m_statemachine.SetBootState("A");
    m_statemachine.Save("D:\\TestStateMachine.xml");

    m_statemachine.Load("D:\\US_StateMachine.xml");
    m_statemachine.Save("D:\\TestStateMachineConfirm.xml");*/
}

void MainWindow::on_pb_OperateList_Add_clicked()
{
    if(m_addstatusDlg)
        m_addstatusDlg->hide();
    if(m_generatestatusDlg)
        m_generatestatusDlg->hide();
     ui->gv_Tree->hide();
    if(m_addoperateDlg==nullptr)
    {
         m_addoperateDlg=new AddOperate(&m_statemachine,ui->widgetRight);
         m_addoperateDlg->SetCurrentStatus(m_CurrentStatus);
         connect(m_addoperateDlg, &AddOperate::NeedRefresh, this, &MainWindow::OperateNeedRefresh);
    }
    m_addoperateDlg->show();
    if(m_addoperateDlg)
        m_addoperateDlg->setGeometry(0,0,ui->widgetRight->width(),ui->widgetRight->height());
    m_addoperateDlg->UpdateStateList();
    m_addoperateDlg->UpdateOperateList();
}

void MainWindow::UpdateStateList()
{
    ui->listWidget_StatusList->clear();
    int count=m_statemachine.GetStatusCount();
    for(int i=0;i<count;i++)
    {
        ui->listWidget_StatusList->addItem(m_statemachine[i].name.c_str());
    }
}

void MainWindow::UpdateOperateList(string state)
{
    if(state.length()<=0)
        return;
    ui->listWidget_OperateList->clear();
    std::map<string, CJOperate>::iterator  itoperate;
    for (itoperate = m_statemachine[state].m_StatusOperateList.begin(); itoperate !=  m_statemachine[state].m_StatusOperateList.end(); ++itoperate)
    {
        ui->listWidget_OperateList->addItem(itoperate->first.c_str());
    }
}

void MainWindow::on_actionOpen_triggered()
{
   QString filename = QFileDialog::getOpenFileName(this,"Open StateMachine Config File",QDir::currentPath(),"StateMachine Config File (*.xml);;All files(*.*)");
   bool ret=m_statemachine.Load(filename.toStdString());
   if(ret)
   {
       ui->statusbar->showMessage("文件加载成功!",3000);
       UpdateStateList();
   }
   else
       QMessageBox::information(this, "打开失败", "文件打开失败", QMessageBox::Ok);

}

void MainWindow::on_actionSave_triggered()
{
     QString filename;
    if(m_DefaultFileName.length()<=0)
    {
        filename = QFileDialog::getSaveFileName(this,"Save As StateMachine Config File",QDir::currentPath(),"StateMachine Config File (*.xml);;All files(*.*)");
        m_DefaultFileName=filename.toStdString();
    }
    bool ret=m_statemachine.Save(m_DefaultFileName);
    if(ret)
        ui->statusbar->showMessage("文件保存成功!",3000);
    else
        QMessageBox::information(this, "保存失败", "文件保存失败", QMessageBox::Ok);
}

void MainWindow::on_actionSave_As_triggered()
{
   QString filename;
   filename = QFileDialog::getSaveFileName(this,"Save As StateMachine Config File",QDir::currentPath(),"StateMachine Config File (*.xml);;All files(*.*)");
   m_DefaultFileName=filename.toStdString();
   bool ret=m_statemachine.Save(m_DefaultFileName);
   if(ret)
       ui->statusbar->showMessage("文件保存成功!",3000);
   else
       QMessageBox::information(this, "保存失败", "文件保存失败", QMessageBox::Ok);
}

void MainWindow::on_listWidget_StatusList_currentTextChanged(const QString &currentText)
{
    m_CurrentStatus=currentText.toStdString();
    string text = currentText.toStdString();
    if(text.length()>0)
    {
        if(m_addoperateDlg)
            m_addoperateDlg->SetCurrentStatus(text);
        UpdateOperateList(text);
    }
}

void MainWindow::StatusNeedRefresh()
{
    UpdateStateList();
    if(m_addoperateDlg)
        m_addoperateDlg->UpdateStateList();
}

void MainWindow::OperateNeedRefresh()
{
    UpdateStateList();
    if(m_CurrentStatus.length()>0)
        UpdateOperateList(m_CurrentStatus);
}

void MainWindow::on_listWidget_OperateList_currentTextChanged(const QString &currentText)
{
   string curtxt = currentText.toStdString();
   if(curtxt.length()<=0)
       return;
   CJState* state= m_statemachine[m_CurrentStatus].m_StatusOperateList[curtxt].m_NextState;
   if(state)
   {
       ui->lb_NextState->setText(state->name.c_str());
       if(m_addoperateDlg)
            m_addoperateDlg->SetCurrentOperate(curtxt,state->name);
   }
}

void MainWindow::on_pb_StatusList_Delete_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidget_StatusList->selectedItems();
    if (selectedItems.count() > 0)
    {
        foreach (QListWidgetItem* var, selectedItems)
        {
           string name = var->text().toStdString();
           m_statemachine.DeleteState(name);
        }
    }
    UpdateStateList();
}

void MainWindow::on_pb_OperateList_Delete_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidget_OperateList->selectedItems();
    if (selectedItems.count() > 0)
    {
        foreach (QListWidgetItem* var, selectedItems)
        {
           string name = var->text().toStdString();
           m_statemachine.DeleteOperate(m_CurrentStatus,name);
        }
    }
    UpdateOperateList(m_CurrentStatus);
}

void MainWindow::on_actionGenerate_Status_triggered()
{
    if(m_addoperateDlg)
        m_addoperateDlg->hide();
    if(m_addstatusDlg)
        m_addstatusDlg->hide();
    ui->gv_Tree->hide();
    if(m_generatestatusDlg==nullptr)
    {
         m_generatestatusDlg=new GenerateStatus(&m_statemachine,ui->widgetRight);
         connect(m_generatestatusDlg, &GenerateStatus::NeedRefresh, this, &MainWindow::StatusNeedRefresh);
    }
    m_generatestatusDlg->show();
    m_generatestatusDlg->UpdateStateList();
    if(m_generatestatusDlg)
        m_generatestatusDlg->setGeometry(0,0,ui->widgetRight->width(),ui->widgetRight->height());
}

void MainWindow::on_checkBox_StatusListMultiSelect_stateChanged(int arg1)
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

void MainWindow::on_checkBox_OperateListMultiSelect_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->listWidget_OperateList->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else
    {
        ui->listWidget_OperateList->setSelectionMode(QAbstractItemView::MultiSelection);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    if(m_addoperateDlg)
        m_addoperateDlg->setGeometry(0,0,ui->widgetRight->width(),ui->widgetRight->height());
    if(m_addstatusDlg)
        m_addstatusDlg->setGeometry(0,0,ui->widgetRight->width(),ui->widgetRight->height());
    if(m_generatestatusDlg)
        m_generatestatusDlg->setGeometry(0,0,ui->widgetRight->width(),ui->widgetRight->height());
}

void MainWindow::on_lineEdit_StatusFilter_textChanged(const QString &arg1)
{
    QList<QListWidgetItem *> list;
    list=ui->listWidget_StatusList->findItems(arg1,Qt::MatchFlag::MatchContains);
    for(int i=0;i<list.size();i++)
    {
        list[i]->setSelected(true);
        ui->listWidget_StatusList->scrollToItem(list[i], QAbstractItemView::PositionAtTop);
        return;
    }
}

void MainWindow::on_actionGenerate_Status_Tree_triggered()
{
    if(m_addoperateDlg)
        m_addoperateDlg->hide();
    if(m_addstatusDlg)
        m_addstatusDlg->hide();
    if(m_generatestatusDlg)
        m_generatestatusDlg->hide();

    ui->gv_Tree->show();

}

void MainWindow::on_gv_Tree_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint)
{
    if(m_TreeScene)
        m_TreeScene->update();
}
