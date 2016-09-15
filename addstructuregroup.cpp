#include "addstructuregroup.h"
#include "structurenaming.h"
#include "ui_addstructuregroup.h"
#include <iostream>
#include <fstream>
#include <QMessageBox>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

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
    string directory = ((StructureNaming*)this->parent())->getInstallDirectory() + string("/structures/") + user;
    DIR *dir;
    struct dirent *ent;
    struct stat sb;

    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            stat(ent->d_name, &sb);
            if(S_ISDIR(sb.st_mode)) {
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
    typedef map<string, vector<string> >::iterator it_type;
    for(it_type iterator = groupMap[ui->user_name_combo_box->currentText().toStdString()].begin(); iterator != groupMap[ui->user_name_combo_box->currentText().toStdString()].end(); iterator++) {
        newGroupName.erase(remove_if( newGroupName.begin(),newGroupName.end(), ::isspace ), newGroupName.end() );
        if(newGroupName.length() == 0) { //FIXME trim whitespace first
            QMessageBox::warning(this, QString("Empty Structure Name"), QString("The structure name cannot be blank."));
            return;
        }
        else if(newGroupName.compare(iterator->first) == 0) {
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

    typedef map<string, map<string, vector<string> > >::iterator it_type2;
    for(it_type2 iterator = groupMap.begin(); iterator != groupMap.end(); iterator++) {
        if(iterator->first.compare("StandardNames") != 0) {
            vector<string> groupFiles = getGroupFiles(iterator->first);
            typedef map<string, vector<string> >::iterator active_group_iterator;
            for(active_group_iterator active_iterator = groupMap[iterator->first].begin(); active_iterator != groupMap[iterator->first].end(); active_iterator++) {
                bool groupFound = false;
                for(unsigned int i = 0; i < groupFiles.size(); i++) {
                    if(active_iterator->first.compare(groupFiles[i]) == 0) {
                        groupFound = true;
                    }
                }
                if(!groupFound) {
                    string filename = ((StructureNaming*)this->parent())->getInstallDirectory() + string("/structures/");
                    filename += iterator->first;
                    filename += "/";
                    filename += ui->new_structure_group_line_edit->text().toStdString();
                    filename += ".txt";                    
                    ofstream groupFile(filename.c_str());
                    for(vector<string>::iterator it = groupMap[iterator->first][active_iterator->first].begin();
                        it != groupMap[iterator->first][active_iterator->first].end(); it++) {
                        groupFile << *it << endl;
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
    typedef map<string, map<string, vector<string> > >::iterator it_type;
    for(it_type iterator = groupMap.begin(); iterator != groupMap.end(); iterator++) {
        if(iterator->first.size() > 0) { //FIXME - check why this is needed
            if(iterator->first.compare("StandardNames") != 0) {
                ui->user_name_combo_box->addItem(QString(iterator->first.c_str()));
            }
        }
    }
}

void AddStructureGroup::loadStructureGroups() {
    map<string, map<string, vector<string> > >& groupMap = ((StructureNaming*)this->parent())->getStructureGroupMap();
    ui->existing_structure_group_list_widget->clear();
    ui->working_structure_list_widget->clear();
    typedef map<string, vector<string> >::iterator it_type;
    for(it_type iterator = groupMap[ui->user_name_combo_box->currentText().toStdString()].begin(); iterator != groupMap[ui->user_name_combo_box->currentText().toStdString()].end(); iterator++) {
        if(iterator->first.size() > 0) {
            ui->existing_structure_group_list_widget->addItem(QString(iterator->first.c_str()));
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
