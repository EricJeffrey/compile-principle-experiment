#include "mainwindow.h"
#include <QApplication>
#include "commonHeader.h"

bool errorExit(const string &str, ostringstream & scout, char x = 0, char a = 0) {
    scout << endl << "!!!!!!!!!!!!!!!!!!!!" << endl << str << endl;
    if (x != 0 || a != 0)
        scout << "x: " << x << "    a: " << a;
    scout << endl << "!!!!!!!!!!!!!!!!!!!!" << endl;
    return false;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFont font;
    font.setPointSize(10);
    font.setFamily("微软雅黑");
    a.setFont(font);

    w.setMinimumSize(1200, 600);
    w.show();

    return a.exec();
}
