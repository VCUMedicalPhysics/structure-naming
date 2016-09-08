#include "structurenaming.h"
#include "createplanningassistdialog.h"
#include "aboutdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Structure Naming");

    StructureNaming w;
    w.setWindowTitle("Structure Naming");
    w.show();

    return app.exec();
}
