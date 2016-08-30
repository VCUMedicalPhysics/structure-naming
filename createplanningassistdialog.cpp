#include "createplanningassistdialog.h"
#include "structurenaming.h"
#include "ui_createplanningassistdialog.h"

CreatePlanningAssistDialog::CreatePlanningAssistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePlanningAssistDialog)
{
    ui->setupUi(this);
    connect(ui->close_button, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(ui->structure_name_line_edit, SIGNAL(textChanged(QString)), this, SLOT(updateResultingName()));
    connect(ui->add_planning_assist_structure_button, SIGNAL(clicked()), this, SLOT(addStructure()));
    this->setWindowTitle("Add Planning Assistance Structure");
    resetFields();
}

CreatePlanningAssistDialog::~CreatePlanningAssistDialog()
{
    delete ui;
}

void CreatePlanningAssistDialog::addStructure() {
    if(ui->structure_name_line_edit->text().length() > 0) {
        if(((StructureNaming*)this->parent())->addWorkingStructure(ui->resulting_name_line_edit->text())) {
            resetFields();
        }
    }
}

void CreatePlanningAssistDialog::updateResultingName() {
    ui->resulting_name_line_edit->setText(QString("NS_").append(ui->structure_name_line_edit->text()));
}

void CreatePlanningAssistDialog::closeWindow() {
    resetFields();
    this->close();
}

void CreatePlanningAssistDialog::resetFields() {
    ui->structure_name_line_edit->setText("");
    ui->resulting_name_line_edit->setText("NS_");
    updateResultingName();
}
