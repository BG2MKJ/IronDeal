#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVector>
#include <QString>
#include <QDir>
#include <QFile>
#include <QDebug>
#include "data_info.h"

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool createTables();
    bool initializeDatabase(const QString& dbPath = "");  // 改为initialize

    bool isDatabaseOpen() const { return isOpen; }  // 新增状态检查

    bool connectToDatabase(const QString& host,const QString& dbname,const QString& user,const QString& password);
    void disconnectFromDatabase();

    bool createUser(const User &user);
    User getUserByID(int userID);
    bool updateUser(const User& user);
    bool deleteUser(int userID);

    bool createProduct(const Product& product);
    Product getProductByID(int productID);
    bool updataProduct(const Product& product);
    bool deleteProduct(int productID);

    bool createOrder(const Order& order);
    Order getOrderById(int orderId);
    bool updateOrderStatus(int orderId, int status);
    bool deleteOrder(int orderId);

private:
    QSqlDatabase db;
    bool isOpen;
    bool executeQuery(const QString &query);
    QString databasePath;

};


#endif // DATABASE_MANAGER_H
