#include "scoreboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScoreBoard w;
    w.show();

    return a.exec();
}
