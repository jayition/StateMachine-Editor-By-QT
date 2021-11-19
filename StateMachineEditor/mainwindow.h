#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addstatus.h"
#include "addoperate.h"
#include "CJStateMachine.h"
#include "generatestatus.h"
#include "diagramscene.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateStateList();

    void UpdateOperateList(string state);

private slots:
    void on_pb_Status_Add_clicked();

    void on_pb_OperateList_Add_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_listWidget_StatusList_currentTextChanged(const QString &currentText);

    void StatusNeedRefresh();

    void OperateNeedRefresh();

    void on_listWidget_OperateList_currentTextChanged(const QString &currentText);

    void on_pb_StatusList_Delete_clicked();

    void on_pb_OperateList_Delete_clicked();

    void on_actionGenerate_Status_triggered();

    void on_checkBox_StatusListMultiSelect_stateChanged(int arg1);

    void on_checkBox_OperateListMultiSelect_stateChanged(int arg1);

    void resizeEvent(QResizeEvent* event);

    void on_lineEdit_StatusFilter_textChanged(const QString &arg1);

    void on_actionGenerate_Status_Tree_triggered();


    void on_gv_Tree_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

private:
    Ui::MainWindow *ui;
    AddStatus* m_addstatusDlg;
    AddOperate* m_addoperateDlg;
    GenerateStatus* m_generatestatusDlg;
    CJStateMachine m_statemachine;
    string m_DefaultFileName;
    string m_CurrentStatus;
    DiagramScene *m_TreeScene;
};
#endif // MAINWINDOW_H
