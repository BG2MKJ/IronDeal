#include "mainwindow.h"
#include "database_manager.h"
#include "data_info.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QApplication app(argc, argv);

    // 初始化数据库
    DatabaseManager dbManager;
    if (!dbManager.initializeDatabase()) {
        qDebug() << "Failed to initialize database";
        return -1;
    }
    // w.show();
    return a.exec();
}
