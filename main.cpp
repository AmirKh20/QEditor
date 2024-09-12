#include "qeditorwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QEditorWindow editor;
    editor.show();
    return app.exec();
}
