#include "abacus.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Abacus w;
    w.show();
    return a.exec();
}
