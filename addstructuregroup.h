#ifndef ADDSTRUCTUREGROUP_H
#define ADDSTRUCTUREGROUP_H

#include <QDialog>

namespace Ui {
class AddStructureGroup;
}

class AddStructureGroup : public QDialog
{
    Q_OBJECT

public:
    explicit AddStructureGroup(QWidget *parent = 0);
    ~AddStructureGroup();
    void resetForm();

private slots:
    void saveGroup();
    void loadStructureGroups();

signals:
    void structureGroupAdded();

private:
    Ui::AddStructureGroup *ui;
    //std::map<std::string, std::map<std::string, std::vector<std::string> > >& groupMap;
    void loadUserNames();
    void loadWorkingList();
};

#endif // ADDSTRUCTUREGROUP_H
