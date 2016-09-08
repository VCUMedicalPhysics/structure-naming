#include "structurenaming.h"
#include "createplanningassistdialog.h"
#include "targetvolumedialog.h"
#include "addstructuregroup.h"
#include "aboutdialog.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <vector>
#include <QUrl>
#include <QDesktopServices>

using namespace std;

StructureNaming::StructureNaming(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StructureNaming)
{
    installDirectory = getInstallDirectory();
    ui->setupUi(this);

    loadStructureDictionary();
    loadStructureGroups();

    connect(ui->actionSavePinnacleStructureList, SIGNAL(triggered()), this, SLOT(savePinnacleStructureList()));
    connect(ui->actionAddNewStructureGroup, SIGNAL(triggered()), this, SLOT(openAddStructureGroup()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openAboutDialog()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(openHelpDialog()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitProgram()));
    connect(ui->structure_name_search_line_edit, SIGNAL(textChanged(QString)), this, SLOT(updateSearch()));

    connect(ui->users_combo_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(setUserGroupList()));
    connect(ui->structure_group_combo_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(setGroupStructureList()));

    connect(ui->add_planning_assist_button, SIGNAL(clicked()), this, SLOT(openAddPlanningAssist()));
    connect(ui->add_target_volume_button, SIGNAL(clicked()), this, SLOT(openAddTarget()));
    connect(ui->save_pinnacle_structures_button, SIGNAL(clicked()), this, SLOT(savePinnacleStructureList()));
    connect(ui->remove_structures_button, SIGNAL(clicked()), this, SLOT(removeWorkingStructures()));
    connect(ui->copy_master_list_structures_button, SIGNAL(clicked()), this, SLOT(copyMasterListStructures()));

    connect(ui->copy_group_structures_button, SIGNAL(clicked()), this, SLOT(copyGroupStructures()));
    connect(ui->select_all_button, SIGNAL(clicked()), this, SLOT(selectAllGroupStructures()));

    dialog = new CreatePlanningAssistDialog(this);
    aboutDialog = new AboutDialog(this);
    targetVolumeDialog = new TargetVolumeDialog(this);
    addStructureGroup = new AddStructureGroup(this);

    connect(addStructureGroup, SIGNAL(structureGroupAdded()), this, SLOT(updateStructureGroups()));
}

StructureNaming::~StructureNaming()
{
    delete dialog;
    delete aboutDialog;
    delete targetVolumeDialog;
    delete ui;
}

string StructureNaming::getInstallDirectory() {
    char* path;
    path = getenv("STRUCTURENAMING");
    if(path != NULL) {
        cout << path << endl;
        return string(path);
    }
    else {
        return string("");
    }
}

std::map<std::string, std::map<std::string, std::vector<std::string> > >& StructureNaming::getStructureGroupMap() {
    return structureGroupMap;
}

QList<QListWidgetItem*> StructureNaming::getWorkingList() {
    QList<QListWidgetItem*> workingList;
    for(int i = 0; i < ui->working_structure_list_widget->count(); i++) {
        workingList.append(ui->working_structure_list_widget->item(i));
    }
    return workingList;
}

bool StructureNaming::addWorkingStructure(QString structureName) {
    QList<QListWidgetItem *> items = ui->working_structure_list_widget->findItems(structureName, Qt::MatchExactly);
    if(items.length() == 0) {
        ui->working_structure_list_widget->addItem(structureName);
        return true;
    }
    else {
        return false;
    }
}

void StructureNaming::removeWorkingStructures() {
    QList<QListWidgetItem*> itemsSelected = ui->working_structure_list_widget->selectedItems();
    for(int i = 0; i < itemsSelected.length(); i++) {
        QListWidgetItem* item = itemsSelected.at(i);
        delete item;
    }
}

void StructureNaming::copyMasterListStructures() {
    QStringList workingStructures;
    QList<QListWidgetItem*> itemsSelected = ui->master_structure_list_widget->selectedItems();

    for(int i = 0; i < itemsSelected.count(); i++) {
        QList<QListWidgetItem *> items = ui->working_structure_list_widget->findItems(itemsSelected.at(i)->text(), Qt::MatchExactly);
        if(items.length() == 0) {
            ui->working_structure_list_widget->insertItem(0, itemsSelected.at(i)->text());
        }
    }
    ui->working_structure_list_widget->sortItems();
}

void StructureNaming::loadStructureDictionary() {
    string structureDirectoryPath = installDirectory + string("/structures/structure_dictionary.txt");
    fstream myfile(structureDirectoryPath.c_str());
    string line;
    QStringList list;

    if (myfile.is_open()) {
        while (getline(myfile,line)) {
            if(line.size() > 0) {
                list.append(QString(line.c_str()));
            }
        }
        myfile.close();
    }

    ui->master_structure_list_widget->insertItems(0, list);
    ui->master_structure_list_widget->setSortingEnabled(true);
    ui->master_structure_list_widget->sortItems();
    ui->master_structure_list_widget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->working_structure_list_widget->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void StructureNaming::updateSearch() {
    QString currentSearch = ui->structure_name_search_line_edit->text();
    for(int i = 0; i < ui->master_structure_list_widget->count(); i++) {
        QListWidgetItem* item = ui->master_structure_list_widget->item(i);
        if(item->text().contains(currentSearch, Qt::CaseInsensitive)) {
            item->setHidden(false);
        }
        else {
            item->setHidden(true);
        }
    }
}

void StructureNaming::openHelpDialog() {
    QDesktopServices::openUrl(QUrl("https://github.com/VCUMedicalPhysics/structure-naming", QUrl::TolerantMode));
}

void StructureNaming::openAboutDialog() {
    aboutDialog->show();
}

void StructureNaming::openAddStructureGroup() {
    addStructureGroup->resetForm();
    addStructureGroup->show();
}

void StructureNaming::savePinnacleStructureList() {
    string pinnacle_pid = "9999"; //FIXME get real PID
    string savePath = "/tmp/test.Script";
    ofstream scriptFile(savePath.c_str());
    scriptFile << "Store.StringAt.ScriptCommand = \"LoadNoCheckSum = /usr/local/adacbeta/PinnacleSiteData/Scripts/add_single_roi.Script.fsleeman\";\n";
    for(int i = 0; i < ui->working_structure_list_widget->count(); i++) {
        string structureName = ui->working_structure_list_widget->item(i)->text().toStdString();
        scriptFile << "Store.StringAt.RoiToAdd = \"" << structureName <<  "\";\n";
        scriptFile << "IF.RoiList.#\"!ContainsObject\"." << structureName << ".THEN.Store.At.ScriptCommand.Execute=\"\";\n";
    }
    scriptFile.close();
}

void StructureNaming::exitProgram() {
     QApplication::quit();
}

void StructureNaming::openAddPlanningAssist() {
    dialog->show();
}

void StructureNaming::openAddTarget() {
    targetVolumeDialog->show();
}

void StructureNaming::updateStructureGroups() {    
    setUserGroupList();
}

int StructureNaming::loadStructureGroups() {
    DIR *dir, *dirInside;
    struct dirent *ent;
    struct stat sb;

    string structure_directory = installDirectory + string("/structures");
    string structure_user_directory;

    ui->structure_group_list_widget->setSortingEnabled(true);
    ui->structure_group_list_widget->sortItems();
    ui->structure_group_list_widget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    if ((dir = opendir (structure_directory.c_str())) != NULL) {
        // print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
            stat(ent->d_name, &sb);
            if(S_ISREG(sb.st_mode)) {
                continue;
            }
            else if(S_ISDIR(sb.st_mode)) {
                if(ent->d_name[0] == '.') {
                    continue;
                }

                map<string, vector<string> > groupMap;
                structure_user_directory = structure_directory + "/" + ent->d_name;
                string directoryName = ent->d_name;
                if ((dirInside = opendir (structure_user_directory.c_str())) != NULL) {
                    // print all the files and directories within directory
                    while ((ent = readdir (dirInside)) != NULL) {                                                
                        string filename = ent->d_name;
                        if(filename.length() < 5) {
                            continue;
                        }
                        else if(filename.substr(filename.length() - 4, 4) != ".txt") {
                            continue;
                        }

                        vector<string> temp;
                        string groupName = ent->d_name;
                        groupName = groupName.substr(0, groupName.size() - 4);

                        string groupFileName = structure_user_directory;
                        groupFileName += "/" + string(ent->d_name);

                        ifstream myfile(groupFileName.c_str());
                        string line;
                        groupMap[groupName] = temp;
                        if (myfile.is_open()) {
                            while (getline(myfile,line)) {
                                if(line.size() > 0) {
                                    groupMap[groupName].push_back(line);
                                }
                            }
                            myfile.close();
                        }
                    }
                    structureGroupMap[directoryName] = groupMap;
                    closedir(dirInside);
                }
            }
            else {
                perror ("error");
                return EXIT_FAILURE;
            }
        }
        closedir(dir);
    }

    typedef map<string, map<string, vector<string> > >::iterator it_type;
    for(it_type iterator = structureGroupMap.begin(); iterator != structureGroupMap.end(); iterator++) {
        ui->users_combo_box->addItem(QString(iterator->first.c_str()));
    }
    setUserGroupList();
    setGroupStructureList();

    return EXIT_SUCCESS;
}

void StructureNaming::setUserGroupList() {
    QString userName = ui->users_combo_box->currentText();
    ui->structure_group_combo_box->clear();

    for(map<string, vector<string> >::iterator it = structureGroupMap[userName.toStdString()].begin();
        it != structureGroupMap[userName.toStdString()].end(); it++) {
        if(it->first.size() > 0) {
            ui->structure_group_combo_box->addItem(QString(it->first.c_str()));
        }
    }    
}

void StructureNaming::setGroupStructureList() {
    QString userName = ui->users_combo_box->currentText();
    QString groupName = ui->structure_group_combo_box->currentText();
    ui->structure_group_list_widget->clear();
    vector<string> temp = structureGroupMap[userName.toStdString()][groupName.toStdString()];

    for(vector<string>::iterator it = temp.begin(); it != temp.end(); it++) {
        ui->structure_group_list_widget->addItem(QString((*it).c_str()));
    }
}

void StructureNaming::copyGroupStructures() {
    for(int i = 0; i < ui->structure_group_list_widget->count(); i++) {
        QListWidgetItem* item = ui->structure_group_list_widget->item(i);
        if(item->isSelected()) {
            addWorkingStructure(item->text());
        }
    }
    ui->working_structure_list_widget->sortItems();
}

void StructureNaming::selectAllGroupStructures() {
    for(int i = 0; i < ui->structure_group_list_widget->count(); i++) {
        QListWidgetItem* item = ui->structure_group_list_widget->item(i);
        item->setSelected(true);
    }
}
