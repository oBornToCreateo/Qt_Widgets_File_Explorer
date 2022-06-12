#include "QFile_Explorer.h"
#include <QtWidgets/QApplication>
#include <QWidget> 

#include <qdebug.h>

int main(int argc, char *argv[])
{
    try {
        
        QApplication* QApp = new QApplication(argc, argv);

        QFile_Explorer* QExplorer = new QFile_Explorer;


        QExplorer->show();

        return QApp->exec();

    }
    catch (QException except) {

        QMessageBox messagebox;
        messagebox.setText("Warning");
        messagebox.setInformativeText(except.what());
        messagebox.exec();

    }
    
}

