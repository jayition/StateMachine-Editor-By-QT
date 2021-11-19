#ifndef ADDOPERATE_H
#define ADDOPERATE_H

#include <QWidget>
#include "CJStateMachine.h"
namespace Ui {
class AddOperate;
}

class AddOperate : public QWidget
{
    Q_OBJECT

public:
    explicit AddOperate(CJStateMachine* sm,QWidget *parent = nullptr);
    ~AddOperate();

    void UpdateOperateList();
    void UpdateStateList();
    void SetCurrentStatus(string status);
    void SetCurrentOperate(string operate,string nextstate);
public slots:
    void SetNeedRefresh();

signals:
    void NeedRefresh();

private slots:
    void on_pb_OK_clicked();

    void on_pb_Add_OperateAvailable_clicked();

    void on_pushButton_Hide_clicked();

    void on_lineEdit_OperateFilter_textChanged(const QString &arg1);

    void on_lineEdit_StatusFilter_textChanged(const QString &arg1);

    void on_lineEdit_StatusSrcFilter_textChanged(const QString &arg1);

    void on_checkBox_StatusMultSelect_stateChanged(int arg1);


    void on_pushButton_Refresh_clicked();

    void on_checkBox_AddSubStatus_stateChanged(int arg1);

    void on_listWidget_OperateAvailable_currentTextChanged(const QString &currentText);

private:
    Ui::AddOperate *ui;
    CJStateMachine* m_statemachine;
    string m_CurrentStatus;
};

#endif // ADDOPERATE_H
