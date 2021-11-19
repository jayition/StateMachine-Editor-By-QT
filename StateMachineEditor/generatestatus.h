#ifndef GENERATESTATUS_H
#define GENERATESTATUS_H

#include <QWidget>
#include "CJStateMachine.h"
namespace Ui {
class GenerateStatus;
}

class GenerateStatus : public QWidget
{
    Q_OBJECT

public:
    explicit GenerateStatus(CJStateMachine* sm,QWidget *parent = nullptr);
    ~GenerateStatus();
    void UpdateStateList();
public slots:
    void SetNeedRefresh();

signals:
    void NeedRefresh();
private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_Delete_clicked();

    void on_pushButton_Generate_clicked();

    void on_pushButton_Refresh_clicked();

    void on_pushButton_Hide_clicked();

private:
    Ui::GenerateStatus *ui;
        CJStateMachine* m_statemachine;
};

#endif // GENERATESTATUS_H
