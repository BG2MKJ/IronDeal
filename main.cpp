#include <QApplication>
#include "data_info.h"
#include "database_manager.h"
#include <Windows.h>
#include "mainwindow.h"
#include "login.h"
int main(int argc, char *argv[])
{
    ::SetEnvironmentVariableA("NT_SYMBOL_PATH", "");
    QApplication a(argc, argv);
    MainWindow w;

    
    /*login li;
    li.show();*/

    // 初始化数据库
    DatabaseManager dbManager;
    if (!dbManager.initializeDatabase("E:/IronDeal/IronDeal/data/data.db")) {
        qDebug() << "Failed to initialize database";
        return -1;
    }
    User u1={255,"cnm","liupass","sb","555url","151333","sanda",5.4,54,2,"44",43};
    dbManager.createUser(u1);
    Product p = { 123,"desc","brief",{"url1"},"sprci","brand",{{1,3,"3","34",34} },"name","cate",234,4 };
    dbManager.createProduct(p);
    OrderItem item = { 123,1,2,4555555 };
    qDebug()<<dbManager.addItemToCart(255, item);
    qDebug() << dbManager.addItemToCart(255, item);

    qDebug() << dbManager.getCartByUserID(255).items[0].classID;
    qDebug() << dbManager.getCartByUserID(255).items[0].price;
    qDebug() << dbManager.getCartByUserID(255).items[0].productID;
    qDebug() << dbManager.getCartByUserID(255).items[0].quantity;
    
    
    
    


     w.show();
    return a.exec();
}
