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
    void loadUserNames();
    void loadWorkingList();
    std::vector<std::string> getGroupFiles(std::string user);
};

#endif // ADDSTRUCTUREGROUP_H
