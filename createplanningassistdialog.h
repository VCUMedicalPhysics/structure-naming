#ifndef CREATEPLANNINGASSISTDIALOG_H
#define CREATEPLANNINGASSISTDIALOG_H

#include <QDialog>

namespace Ui {
class CreatePlanningAssistDialog;
}

class CreatePlanningAssistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlanningAssistDialog(QWidget *parent = 0);
    ~CreatePlanningAssistDialog();

private:
    Ui::CreatePlanningAssistDialog *ui;
    void resetFields();

private slots:
    void closeWindow();
    void addStructure();
    void updateResultingName();
};

#endif // CREATEPLANNINGASSISTDIALOG_H
