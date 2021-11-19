#ifndef ADDSTATUS_H
#define ADDSTATUS_H

#include <QWidget>
#include "CJStateMachine.h"
namespace Ui {
class AddStatus;
}

class AddStatus : public QWidget
{
    Q_OBJECT

public:
    explicit AddStatus(CJStateMachine* sm,QWidget *parent = nullptr);
    ~AddStatus();

private slots:
    void on_pb_OK_clicked();
    void on_pushButton_Hide_clicked();

public slots:
    void SetNeedRefresh();

signals:
    void NeedRefresh();
private:
    Ui::AddStatus *ui;
    CJStateMachine* m_statemachine;
};

#endif // ADDSTATUS_H
