#include "addstructuregroup.h"
#include "structurenaming.h"
#include "ui_addstructuregroup.h"
#include <iostream>
#include <fstream>
#include <QMessageBox>
#include "dirent.h"

using namespace std;

AddStructureGroup::AddStructureGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStructureGroup)
{
    ui->setupUi(this);
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->save_button, SIGNAL(clicked()), this, SLOT(saveGroup()));
    connect(ui->user_name_combo_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(loadStructureGroups()));

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

vector<string> AddStructureGroup::getGroupFiles(string user) {
    vector<string> groupDirectories;
    string directory = "../structure-naming/structures/" + user;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_type == DT_REG) {
                string filename = ent->d_name;
                if(filename.length() < 5) {
                    continue;
                }
                else if(filename.substr(filename.length() - 4, 4) != ".txt") {
                    continue;
                }
                groupDirectories.push_back(filename.substr(0, filename.length() - 4));
            }
        }
    }

    return groupDirectories;
}

void AddStructureGroup::saveGroup() {
    string newGroupName = ui->new_structure_group_line_edit->text().toStdString();
    map<string, map<string, vector<string> > >& groupMap = ((StructureNaming*)this->parent())->getStructureGroupMap();
    for (auto& x: groupMap[ui->user_name_combo_box->currentText().toStdString()]) {
        newGroupName.erase( std::remove_if( newGroupName.begin(),newGroupName.end(), ::isspace ), newGroupName.end() );
        if(newGroupName.compare(x.first) == 0) {
            string message = "Structure group name " + newGroupName + " alredy exists.";
            QMessageBox::warning(this, QString("Group Name Exists"), QString(message.c_str()));
            return;
        }        
        else if(newGroupName.length() == 0) { //FIXME trim whitespace first
            QMessageBox::warning(this, QString("Empty Structure Name"), QString("The structure name cannot be blank."));
            return;
        }
    }

    vector<string> workingStructureNames;
    for(int i = 0; i < ui->working_structure_list_widget->count(); i++) {
        workingStructureNames.push_back(ui->working_structure_list_widget->item(i)->text().toStdString());
    }
    groupMap[ui->user_name_combo_box->currentText().toStdString()][newGroupName] = workingStructureNames;

    for (auto& user: groupMap) {
        if(user.first.compare("StandardNames") != 0) {
            vector<string> groupFiles = getGroupFiles(user.first);
            for (auto& activeGroup: groupMap[user.first]) {
                bool groupFound = false;
                for(int i = 0; i < groupFiles.size(); i++) {
                    if(activeGroup.first.compare(groupFiles[i]) == 0) {
                        groupFound = true;
                    }
                }
                if(!groupFound) {
                    string filename = "../structure-naming/structures/";
                    filename += user.first;
                    filename += "/";
                    filename += ui->new_structure_group_line_edit->text().toStdString();
                    filename += ".txt";
                    cout << filename << endl;
                    ofstream groupFile;
                    groupFile.open(filename, ios::out);
                    for (auto& structure: groupMap[user.first][activeGroup.first]) {
                        groupFile << structure << endl;
                    }
                    groupFile.close();
                }
            }
        }
    }

    emit structureGroupAdded();
    emit close();
}

void AddStructureGroup::loadUserNames() {
    map<string, map<string, vector<string> > >& groupMap = ((StructureNaming*)this->parent())->getStructureGroupMap();
    ui->user_name_combo_box->clear();
    for (auto& x: groupMap) {
        if(x.first.size() > 0) { //FIXME - check why this is needed
            if(x.first.compare("StandardNames") != 0) {
                ui->user_name_combo_box->addItem(QString(x.first.c_str()));
            }
        }
    }
}

void AddStructureGroup::loadStructureGroups() {
    map<string, map<string, vector<string> > >& groupMap = ((StructureNaming*)this->parent())->getStructureGroupMap();
    ui->existing_structure_group_list_widget->clear();
    ui->working_structure_list_widget->clear();
    for (auto& x: groupMap[ui->user_name_combo_box->currentText().toStdString()]) {
        if(x.first.size() > 0) {
            ui->existing_structure_group_list_widget->addItem(QString(x.first.c_str()));
        }
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
