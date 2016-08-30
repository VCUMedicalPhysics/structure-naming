#include "targetvolumedialog.h"
#include "ui_targetvolumedialog.h"
#include "structurenaming.h"
#include <map>
#include <iostream>
#include <QMainWindow>

std::map<QString, QString>  nodeMap;

TargetVolumeDialog::TargetVolumeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TargetVolumeDialog)
{          
    ui->setupUi(this);
    this->setWindowTitle("Add Target Volume Structure");
    setLists();

    ui->icru_name_combo_box->addItems(*this->icruNames);
    ui->node_type_combo_box->addItems(*this->nodeNames);
    ui->single_multiple_combo_box->addItems(*this->singleMultipleNames);
    updateResultingName();

    connect(ui->close_button, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->add_structure_button, SIGNAL(clicked()), this, SLOT(addStructure()));
    connect(ui->icru_name_combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(updateResultingName()));
    connect(ui->node_type_combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(updateResultingName()));
    connect(ui->single_multiple_combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(updateResultingName()));

    connect(ui->structure_indicators_line_edit, SIGNAL(textChanged(QString)), this, SLOT(updateResultingName()));
    connect(ui->dose_line_edit, SIGNAL(textChanged(QString)), this, SLOT(updateResultingName()));
    connect(ui->custom_line_edit, SIGNAL(textChanged(QString)), this, SLOT(updateResultingName()));
}

void TargetVolumeDialog::setLists() {
    icruNames = new QStringList();
    icruNames->append("GTV");
    icruNames->append("CTV");
    icruNames->append("ITV");
    icruNames->append("IGTV");
    icruNames->append("ICTV");
    icruNames->append("PTV");

    nodeNames = new QStringList();
    nodeMap[QString("")] = QString("");
    nodeMap[QString("Nodal")] = QString("n");
    nodeMap[QString("Primary")] = QString("p");
    nodeMap[QString("Surgical Bed")] = QString("sb");
    nodeMap[QString("Parenchyma")] = QString("par");
    nodeMap[QString("Venous Thrombosis")] = QString("vt");
    nodeMap[QString("Vascular")] = QString("vas");
    nodeMap[QString("Pre-op")] = QString("preop");
    nodeMap[QString("Post-op")] = QString("postop");

    for (std::map<QString, QString>::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it) {
        nodeNames->append(it->first);
    }

    singleMultipleNames = new QStringList();
    singleMultipleNames->append("Single");
    singleMultipleNames->append("Multiple");
}

TargetVolumeDialog::~TargetVolumeDialog()
{
    delete icruNames;
    delete nodeNames;
    delete singleMultipleNames;
    delete ui;
}

void TargetVolumeDialog::updateResultingName() {
    QString targetName;
    targetName.append(ui->icru_name_combo_box->currentText());
    targetName.append(nodeMap[ui->node_type_combo_box->currentText()]);
    if(ui->single_multiple_combo_box->currentText() != "Single") {
        targetName.append(ui->single_multiple_combo_box->currentText());
    }
    if(ui->structure_indicators_line_edit->text().length() > 0) {
        targetName.append("_");
        targetName.append(ui->structure_indicators_line_edit->text());
    }
    if(ui->dose_line_edit->text().length() > 0) {
        targetName.append("_");
        targetName.append(ui->dose_line_edit->text());
    }
    if(ui->custom_line_edit->text().length() > 0) {
        targetName.append("^");
        targetName.append(ui->custom_line_edit->text());
    }

    ui->resulting_name_line_edit->setText(targetName);
}

void TargetVolumeDialog::addStructure() {
   if(((StructureNaming*)this->parent())->addWorkingStructure(ui->resulting_name_line_edit->text())) {
       resetFields();
   }
}

void TargetVolumeDialog::resetFields() {
    ui->icru_name_combo_box->setCurrentIndex(0);
    ui->node_type_combo_box->setCurrentIndex(0);
    ui->single_multiple_combo_box->setCurrentIndex(0);
    ui->structure_indicators_line_edit->setText("");
    ui->dose_line_edit->setText("");
    ui->custom_line_edit->setText("");
    ui->resulting_name_line_edit->setText("");
    updateResultingName();
}
