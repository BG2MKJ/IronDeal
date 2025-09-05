#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QVector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "data_info.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool createTables();
    bool initializeDatabase(const QString& dbPath = "");

    bool isDatabaseOpen() const { return isOpen; }

    bool connectToDatabase(const QString& host,
        const QString& dbname,
        const QString& user,
        const QString& password);
    void disconnectFromDatabase();

    bool createUser(const User& user);
    User getUserByID(int userID);
    bool updateUser(const User& user);
    bool deleteUser(int userID);
    bool updateUserRating(int userID, int newRating); // 新增用户评级更新

    bool createProduct(const Product& product);
    Product getProductByID(int productID);
    bool updateProduct(const Product& product); // 修正拼写错误：updataProduct -> updateProduct
    bool deleteProduct(int productID);

    bool createOrder(const Order& order);
    Order getOrderById(int orderId);
    bool updateOrderStatus(int orderId, int status);
    bool deleteOrder(int orderId);

    Cart getCartByUserID(int userID); // 修改函数名更准确
    bool updateCart(const Cart& cart);
    bool clearCart(int userID); // 新增清空购物车
    bool addItemToCart(int userID, const OrderItem& item); // 新增添加商品到购物车
    bool removeItemFromCart(int userID, int productID, int classID); // 新增从购物车移除商品

private:
    QSqlDatabase db;
    bool isOpen;
    QString databasePath;

    bool executeQuery(const QString& query);
    bool createUserCart(int userID); // 为用户创建购物车
};

#endif // DATABASE_MANAGER_H