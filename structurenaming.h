#ifndef STRUCTURENAMING_H
#define STRUCTURENAMING_H

#include <QMainWindow>
#include "createplanningassistdialog.h"
#include "addstructuregroup.h"
#include "aboutdialog.h"
#include "targetvolumedialog.h"
#include "ui_structurenaming.h"

namespace Ui {
class StructureNaming;
}

class StructureNaming : public QMainWindow
{
    Q_OBJECT

public:
    explicit StructureNaming(QWidget *parent = 0);    
    bool addWorkingStructure(QString structureName);
    std::map<std::string, std::map<std::string, std::vector<std::string> > >& getStructureGroupMap();
    QList<QListWidgetItem*> getWorkingList();
    std::string getInstallDirectory();
    ~StructureNaming();
private:
    Ui::StructureNaming *ui;
    CreatePlanningAssistDialog *dialog;
    AboutDialog *aboutDialog;
    TargetVolumeDialog *targetVolumeDialog;
    AddStructureGroup *addStructureGroup;
    std::map<std::string, std::map<std::string, std::vector<std::string> > > structureGroupMap;    
    std::string installDirectory;

    void loadStructureDictionary();

private slots:
    void updateStructureGroups();
    int loadStructureGroups();
    void openAddStructureGroup();
    void selectAllGroupStructures();
    void copyGroupStructures();
    void setUserGroupList();
    void setGroupStructureList();
    void updateSearch();
    void openAddTarget();
    void removeWorkingStructures();
    void copyMasterListStructures();
    void openAddPlanningAssist();
    void exitProgram();
    void savePinnacleStructureList();
    void openHelpDialog();
    void openAboutDialog();
};

#endif // STRUCTURENAMING_H
