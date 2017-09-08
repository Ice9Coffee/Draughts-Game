#include "gamewindow.h"
#include <QApplication>

#include "gamestate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameState s;
    GameWindow w;
    w.show();

    return a.exec();
}
