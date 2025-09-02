#include <QApplication>
#include "data_info.h"
#include "database_manager.h"
#include "mainwindow.h"
#include "login.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QApplication app(argc, argv);
    login li;
    li.show();

    // 初始化数据库
    DatabaseManager dbManager;
    if (!dbManager.initializeDatabase("E:/IronDeal/IronDeal/data/data.db")) {
        qDebug() << "Failed to initialize database";
        return -1;
    }
    // User u1={255,"cnm","liupass","sb","555url","151333","sanda",5.4,"44",43};
    // dbManager.createUser(u1);
    User u2;
    u2=dbManager.getUserByID(255);
    qDebug()<<u2.username;


    // w.show();
    return a.exec();
}
