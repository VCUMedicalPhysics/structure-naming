#include "addstructuregroup.h"
#include "structurenaming.h"
#include "ui_addstructuregroup.h"
#include <iostream>
#include <QMessageBox>
using namespace std;

AddStructureGroup::AddStructureGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStructureGroup)
{
    ui->setupUi(this);
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->save_button, SIGNAL(clicked()), this, SLOT(saveGroup()));
    connect(ui->user_name_combo_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(loadStructureGroups()));

//    ui->working_structure_list_widget->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
//    ui->existing_structure_group_list_widget->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);

    resetForm();
}

AddStructureGroup::~AddStructureGroup()
{
    delete ui;
}

void AddStructureGroup::resetForm() {
    loadUserNames();
    ui->new_structure_group_line_edit->setText("");
}

void AddStructureGroup::saveGroup() {
    cout << "saving group" << endl;
    cout << ui->user_name_combo_box->currentText().toStdString() << endl;
    cout << ui->new_structure_group_line_edit->text().toStdString() << endl;
    string newGroupName = ui->new_structure_group_line_edit->text().toStdString();
    map<string, map<string, vector<string> > >& groupMap = ((StructureNaming*)this->parent())->getStructureGroupMap();
    for (auto& x: groupMap[ui->user_name_combo_box->currentText().toStdString()]) {
        if(newGroupName.compare(x.first) == 0) {
            string message = "Structure group name " + newGroupName + " alredy exists.";
            QMessageBox::warning(this, QString("Group Name Exists"), QString(message.c_str()));
            return;
        }
    }

    vector<string> workingStructureNames;
    for(int i = 0; i < ui->working_structure_list_widget->count(); i++) {
        workingStructureNames.push_back(ui->working_structure_list_widget->item(i)->text().toStdString());
    }
    groupMap[ui->user_name_combo_box->currentText().toStdString()][newGroupName] = workingStructureNames;

    emit structureGroupAdded();
}

void AddStructureGroup::loadUserNames() {
    map<string, map<string, vector<string> > >& groupMap = ((StructureNaming*)this->parent())->getStructureGroupMap();
    ui->user_name_combo_box->clear();
    for (auto& x: groupMap) {
        if(x.first.size() > 0) { //FIXME - check why this is needed
            ui->user_name_combo_box->addItem(QString(x.first.c_str()));
        }
    }
}

void AddStructureGroup::loadStructureGroups() {
    map<string, map<string, vector<string> > >& groupMap = ((StructureNaming*)this->parent())->getStructureGroupMap();
    ui->existing_structure_group_list_widget->clear();
    ui->working_structure_list_widget->clear();
    for (auto& x: groupMap[ui->user_name_combo_box->currentText().toStdString()]) {
        ui->existing_structure_group_list_widget->addItem(QString(x.first.c_str()));
    }
    loadWorkingList();
}

void AddStructureGroup::loadWorkingList() {
    ui->working_structure_list_widget->clear();
    QList<QListWidgetItem*> workingList = ((StructureNaming*)this->parent())->getWorkingList();
    for(int i = 0; i < workingList.count(); i++) {
        ui->working_structure_list_widget->addItem(workingList.at(i)->text());
    }
}
