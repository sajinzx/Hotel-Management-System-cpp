#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow loginWindow;
    loginWindow.exec(); // Show login window modally
    return a.exec();
}
