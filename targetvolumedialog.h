#ifndef TARGETVOLUMEDIALOG_H
#define TARGETVOLUMEDIALOG_H

#include <QDialog>

namespace Ui {
class TargetVolumeDialog;
}

class TargetVolumeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TargetVolumeDialog(QWidget *parent = 0);
    ~TargetVolumeDialog();

private slots:
    void addStructure();
    void updateResultingName();

private:
    Ui::TargetVolumeDialog *ui;
    QStringList* icruNames;
    QStringList* nodeNames;
    QStringList* singleMultipleNames;
    QString structureIndicators;
    QString customQualifier;
    QString resultingName;

    void setLists();
    void resetFields();
};

#endif // TARGETVOLUMEDIALOG_H
