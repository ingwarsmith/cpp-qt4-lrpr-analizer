#include "mainwnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWnd wnd;
    wnd.show();

    return app.exec();
}
