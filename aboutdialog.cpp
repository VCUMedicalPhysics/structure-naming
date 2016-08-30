#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(close()));

    QString aboutText = "Pinnacle Structure Renaming\nFord Sleeman\nwilliam.sleemaniv@vcuhealth.org";
    ui->aboutText->setText(aboutText);
    this->setWindowTitle("About");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
