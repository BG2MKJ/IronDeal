#include "database_manager.h"
#include <QDateTime>
#include <QDebug>
#include <QVariant>

DatabaseManager::DatabaseManager()
    : isOpen(false)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    disconnectFromDatabase();
}

bool DatabaseManager::connectToDatabase(const QString &host,
                                        const QString &dbname,
                                        const QString &user,
                                        const QString &password)
{
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(password);

    if (!db.open()) {
        qDebug() << "Error: connection with database failed: " << db.lastError().text();
        isOpen = false;
        return false;
    }

    isOpen = true;
    qDebug() << "Database connection established";
    return true;
}

void DatabaseManager::disconnectFromDatabase()
{
    if (db.isOpen()) {
        db.close();
        isOpen = false;
        qDebug() << "Database connection closed";
    }
}

bool DatabaseManager::executeQuery(const QString &query)
{
    if (!isOpen) {
        qDebug() << "Database is not connected";
        return false;
    }

    QSqlQuery sqlQuery(db);
    if (!sqlQuery.exec(query)) {
        qDebug() << "Query failed: " << sqlQuery.lastError().text();
        qDebug() << "Query: " << query;
        return false;
    }
    return true;
}

bool DatabaseManager::createUser(const User &user)
{
    if (!isOpen)
        return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (userID, username, password, nickname, avatarURL, "
                  "phone, default_address, balance, registerTime, userLevel) "
                  "VALUES (:userID, :username, :password, :nickname, :avatarURL, "
                  ":phone, :default_address, :balance, :registerTime, :userLevel)");

    query.bindValue(":userID", user.userID);
    query.bindValue(":username", QString::fromStdString(user.username));
    query.bindValue(":password", QString::fromStdString(user.password));
    query.bindValue(":nickname", QString::fromStdString(user.nickname));
    query.bindValue(":avatarURL", QString::fromStdString(user.avatarURL));
    query.bindValue(":phone", QString::fromStdString(user.phone));
    query.bindValue(":default_address", QString::fromStdString(user.default_address));
    query.bindValue(":balance", user.balance);
    query.bindValue(":registerTime", QString::fromStdString(user.registerTime));
    query.bindValue(":userLevel", user.userLevel);

    if (!query.exec()) {
        qDebug() << "Create user failed: " << query.lastError().text();
        return false;
    }
    return true;
}

User DatabaseManager::getUserByID(int userID)
{
    User user;
    if (!isOpen)
        return user;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE userID = :userID");
    query.bindValue(":userID", userID);

    if (!query.exec() || !query.next()) {
        qDebug() << "Get user failed: " << query.lastError().text();
        return user;
    }

    user.userID = query.value("userID").toInt();
    user.username = query.value("username").toString().toStdString();
    user.password = query.value("password").toString().toStdString();
    user.nickname = query.value("nickname").toString().toStdString();
    user.avatarURL = query.value("avatarURL").toString().toStdString();
    user.phone = query.value("phone").toString().toStdString();
    user.default_address = query.value("default_address").toString().toStdString();
    user.balance = query.value("balance").toDouble();
    user.registerTime = query.value("registerTime").toString().toStdString();
    user.userLevel = query.value("userLevel").toInt();

    return user;
}

bool DatabaseManager::updateUser(const User &user)
{
    if (!isOpen)
        return false;

    QSqlQuery query(db);
    query.prepare("UPDATE users SET username = :username, password = :password, "
                  "nickname = :nickname, avatarURL = :avatarURL, phone = :phone, "
                  "default_address = :default_address, balance = :balance, "
                  "registerTime = :registerTime, userLevel = :userLevel "
                  "WHERE userID = :userID");

    query.bindValue(":username", QString::fromStdString(user.username));
    query.bindValue(":password", QString::fromStdString(user.password));
    query.bindValue(":nickname", QString::fromStdString(user.nickname));
    query.bindValue(":avatarURL", QString::fromStdString(user.avatarURL));
    query.bindValue(":phone", QString::fromStdString(user.phone));
    query.bindValue(":default_address", QString::fromStdString(user.default_address));
    query.bindValue(":balance", user.balance);
    query.bindValue(":registerTime", QString::fromStdString(user.registerTime));
    query.bindValue(":userLevel", user.userLevel);
    query.bindValue(":userID", user.userID);

    if (!query.exec()) {
        qDebug() << "Update user failed: " << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteUser(int userID)
{
    if (!isOpen)
        return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE userID = :userID");
    query.bindValue(":userID", userID);

    if (!query.exec()) {
        qDebug() << "Delete user failed: " << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::createProduct(const Product &product)
{
    if (!isOpen)
        return false;

    // 首先插入产品基本信息
    QSqlQuery query(db);
    query.prepare("INSERT INTO products (productID, description, brief_description, "
                  "specification, brand, productName, category, sellerID, salesCount) "
                  "VALUES (:productID, :description, :brief_description, "
                  ":specification, :brand, :productName, :category, :sellerID, :salesCount)");

    query.bindValue(":productID", product.productID);
    query.bindValue(":description", QString::fromStdString(product.description));
    query.bindValue(":brief_description", QString::fromStdString(product.brief_description));
    query.bindValue(":specification", QString::fromStdString(product.specification));
    query.bindValue(":brand", QString::fromStdString(product.brand));
    query.bindValue(":productName", QString::fromStdString(product.productName));
    query.bindValue(":category", QString::fromStdString(product.category));
    query.bindValue(":sellerID", product.sellerID);
    query.bindValue(":salesCount", product.salesCount);

    if (!query.exec()) {
        qDebug() << "Create product failed: " << query.lastError().text();
        return false;
    }

    // 插入产品图片
    for (const auto &imageURL : product.description_imageURLs) {
        QSqlQuery imgQuery(db);
        imgQuery.prepare("INSERT INTO product_images (productID, imageURL) "
                         "VALUES (:productID, :imageURL)");
        imgQuery.bindValue(":productID", product.productID);
        imgQuery.bindValue(":imageURL", QString::fromStdString(imageURL));

        if (!imgQuery.exec()) {
            qDebug() << "Insert product image failed: " << imgQuery.lastError().text();
        }
    }

    // 插入产品分类
    for (const auto &productClass : product.product_class) {
        QSqlQuery classQuery(db);
        classQuery.prepare("INSERT INTO product_classes (classID, productID, stock, "
                           "small_imageURL, name, price) "
                           "VALUES (:classID, :productID, :stock, "
                           ":small_imageURL, :name, :price)");

        classQuery.bindValue(":classID", productClass.classID);
        classQuery.bindValue(":productID", product.productID);
        classQuery.bindValue(":stock", productClass.stock);
        classQuery.bindValue(":small_imageURL", QString::fromStdString(productClass.small_imageURL));
        classQuery.bindValue(":name", QString::fromStdString(productClass.name));
        classQuery.bindValue(":price", productClass.price);

        if (!classQuery.exec()) {
            qDebug() << "Insert product class failed: " << classQuery.lastError().text();
        }
    }

    return true;
}

Product DatabaseManager::getProductByID(int productID)
{
    Product product;
    if (!isOpen)
        return product;

    // 获取产品基本信息
    QSqlQuery query(db);
    query.prepare("SELECT * FROM products WHERE productID = :productID");
    query.bindValue(":productID", productID);

    if (!query.exec() || !query.next()) {
        qDebug() << "Get product failed: " << query.lastError().text();
        return product;
    }

    product.productID = query.value("productID").toInt();
    product.description = query.value("description").toString().toStdString();
    product.brief_description = query.value("brief_description").toString().toStdString();
    product.specification = query.value("specification").toString().toStdString();
    product.brand = query.value("brand").toString().toStdString();
    product.productName = query.value("productName").toString().toStdString();
    product.category = query.value("category").toString().toStdString();
    product.sellerID = query.value("sellerID").toInt();
    product.salesCount = query.value("salesCount").toInt();

    // 获取产品图片
    QSqlQuery imgQuery(db);
    imgQuery.prepare("SELECT imageURL FROM product_images WHERE productID = :productID");
    imgQuery.bindValue(":productID", productID);

    if (imgQuery.exec()) {
        while (imgQuery.next()) {
            product.description_imageURLs.push_back(
                imgQuery.value("imageURL").toString().toStdString());
        }
    }

    // 获取产品分类
    QSqlQuery classQuery(db);
    classQuery.prepare("SELECT * FROM product_classes WHERE productID = :productID");
    classQuery.bindValue(":productID", productID);

    if (classQuery.exec()) {
        while (classQuery.next()) {
            ProductClass productClass;
            productClass.classID = classQuery.value("classID").toInt();
            productClass.stock = classQuery.value("stock").toInt();
            productClass.small_imageURL = classQuery.value("small_imageURL").toString().toStdString();
            productClass.name = classQuery.value("name").toString().toStdString();
            productClass.price = classQuery.value("price").toDouble();

            product.product_class.push_back(productClass);
        }
    }

    return product;
}

bool DatabaseManager::updataProduct(const Product &product)
{
    if (!isOpen)
        return false;

    QSqlQuery query(db);
    query.prepare("UPDATE products SET description = :description, "
                  "brief_description = :brief_description, specification = :specification, "
                  "brand = :brand, productName = :productName, category = :category, "
                  "sellerID = :sellerID, salesCount = :salesCount "
                  "WHERE productID = :productID");

    query.bindValue(":description", QString::fromStdString(product.description));
    query.bindValue(":brief_description", QString::fromStdString(product.brief_description));
    query.bindValue(":specification", QString::fromStdString(product.specification));
    query.bindValue(":brand", QString::fromStdString(product.brand));
    query.bindValue(":productName", QString::fromStdString(product.productName));
    query.bindValue(":category", QString::fromStdString(product.category));
    query.bindValue(":sellerID", product.sellerID);
    query.bindValue(":salesCount", product.salesCount);
    query.bindValue(":productID", product.productID);

    if (!query.exec()) {
        qDebug() << "Update product failed: " << query.lastError().text();
        return false;
    }

    // 更新产品图片（先删除再插入）
    QSqlQuery deleteImgQuery(db);
    deleteImgQuery.prepare("DELETE FROM product_images WHERE productID = :productID");
    deleteImgQuery.bindValue(":productID", product.productID);
    deleteImgQuery.exec();

    for (const auto &imageURL : product.description_imageURLs) {
        QSqlQuery imgQuery(db);
        imgQuery.prepare("INSERT INTO product_images (productID, imageURL) "
                         "VALUES (:productID, :imageURL)");
        imgQuery.bindValue(":productID", product.productID);
        imgQuery.bindValue(":imageURL", QString::fromStdString(imageURL));
        imgQuery.exec();
    }

    // 更新产品分类（先删除再插入）
    QSqlQuery deleteClassQuery(db);
    deleteClassQuery.prepare("DELETE FROM product_classes WHERE productID = :productID");
    deleteClassQuery.bindValue(":productID", product.productID);
    deleteClassQuery.exec();

    for (const auto &productClass : product.product_class) {
        QSqlQuery classQuery(db);
        classQuery.prepare("INSERT INTO product_classes (classID, productID, stock, "
                           "small_imageURL, name, price) "
                           "VALUES (:classID, :productID, :stock, "
                           ":small_imageURL, :name, :price)");

        classQuery.bindValue(":classID", productClass.classID);
        classQuery.bindValue(":productID", product.productID);
        classQuery.bindValue(":stock", productClass.stock);
        classQuery.bindValue(":small_imageURL", QString::fromStdString(productClass.small_imageURL));
        classQuery.bindValue(":name", QString::fromStdString(productClass.name));
        classQuery.bindValue(":price", productClass.price);

        classQuery.exec();
    }

    return true;
}

bool DatabaseManager::deleteProduct(int productID)
{
    if (!isOpen)
        return false;

    // 先删除相关数据
    QSqlQuery deleteImgQuery(db);
    deleteImgQuery.prepare("DELETE FROM product_images WHERE productID = :productID");
    deleteImgQuery.bindValue(":productID", productID);
    deleteImgQuery.exec();

    QSqlQuery deleteClassQuery(db);
    deleteClassQuery.prepare("DELETE FROM product_classes WHERE productID = :productID");
    deleteClassQuery.bindValue(":productID", productID);
    deleteClassQuery.exec();

    // 再删除产品
    QSqlQuery query(db);
    query.prepare("DELETE FROM products WHERE productID = :productID");
    query.bindValue(":productID", productID);

    if (!query.exec()) {
        qDebug() << "Delete product failed: " << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::createOrder(const Order &order)
{
    if (!isOpen)
        return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO orders (orderID, userID, sellerID, totalAmount, status, address) "
                  "VALUES (:orderID, :userID, :sellerID, :totalAmount, :status, :address)");

    query.bindValue(":orderID", order.orderID);
    query.bindValue(":userID", order.userID);
    query.bindValue(":sellerID", order.sellerID);
    query.bindValue(":totalAmount", order.totalAmount);
    query.bindValue(":status", order.status);
    query.bindValue(":address", QString::fromStdString(order.address));

    if (!query.exec()) {
        qDebug() << "Create order failed: " << query.lastError().text();
        return false;
    }

    // 插入订单项
    for (const auto &item : order.orderitem) {
        QSqlQuery itemQuery(db);
        itemQuery.prepare("INSERT INTO order_items (orderID, productID, classID, mount) "
                          "VALUES (:orderID, :productID, :classID, :mount)");

        itemQuery.bindValue(":orderID", order.orderID);
        itemQuery.bindValue(":productID", item.productID);
        itemQuery.bindValue(":classID", item.classID);
        itemQuery.bindValue(":mount", item.mount);

        if (!itemQuery.exec()) {
            qDebug() << "Insert order item failed: " << itemQuery.lastError().text();
        }
    }

    return true;
}

Order DatabaseManager::getOrderById(int orderId)
{
    Order order;
    if (!isOpen)
        return order;

    // 获取订单基本信息
    QSqlQuery query(db);
    query.prepare("SELECT * FROM orders WHERE orderID = :orderID");
    query.bindValue(":orderID", orderId);

    if (!query.exec() || !query.next()) {
        qDebug() << "Get order failed: " << query.lastError().text();
        return order;
    }

    order.orderID = query.value("orderID").toInt();
    order.userID = query.value("userID").toInt();
    order.sellerID = query.value("sellerID").toInt();
    order.totalAmount = query.value("totalAmount").toDouble();
    order.status = query.value("status").toInt();
    order.address = query.value("address").toString().toStdString();

    // 获取订单项
    QSqlQuery itemQuery(db);
    itemQuery.prepare("SELECT * FROM order_items WHERE orderID = :orderID");
    itemQuery.bindValue(":orderID", orderId);

    if (itemQuery.exec()) {
        while (itemQuery.next()) {
            OrderItem item;
            item.productID = itemQuery.value("productID").toInt();
            item.classID = itemQuery.value("classID").toInt();
            item.mount = itemQuery.value("mount").toInt();

            order.orderitem.push_back(item);
        }
    }

    return order;
}

bool DatabaseManager::updateOrderStatus(int orderId, int status)
{
    if (!isOpen)
        return false;

    QSqlQuery query(db);
    query.prepare("UPDATE orders SET status = :status WHERE orderID = :orderID");
    query.bindValue(":status", status);
    query.bindValue(":orderID", orderId);

    if (!query.exec()) {
        qDebug() << "Update order status failed: " << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteOrder(int orderId)
{
    if (!isOpen)
        return false;

    // 先删除订单项
    QSqlQuery deleteItemsQuery(db);
    deleteItemsQuery.prepare("DELETE FROM order_items WHERE orderID = :orderID");
    deleteItemsQuery.bindValue(":orderID", orderId);
    deleteItemsQuery.exec();

    // 再删除订单
    QSqlQuery query(db);
    query.prepare("DELETE FROM orders WHERE orderID = :orderID");
    query.bindValue(":orderID", orderId);

    if (!query.exec()) {
        qDebug() << "Delete order failed: " << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::initializeDatabase(const QString &dbPath)
{
    // 如果已经打开，先关闭
    if (isOpen) {
        disconnectFromDatabase();
    }

    // 设置数据库文件路径
    if (dbPath.isEmpty()) {
        // 默认路径：项目目录下的 database 文件夹
        QDir dir(QDir::currentPath());
        if (!dir.exists("database")) {
            if (!dir.mkpath("database")) {
                qDebug() << "Error: Cannot create database directory";
                return false;
            }
        }
        databasePath = dir.filePath("database/ecommerce.db");
    } else {
        databasePath = dbPath;
    }

    qDebug() << "Database path:" << databasePath;

    db.setDatabaseName(databasePath);

    // 尝试打开数据库（如果不存在会自动创建）
    if (!db.open()) {
        qDebug() << "Error: Cannot open database:" << db.lastError().text();
        isOpen = false;
        return false;
    }

    isOpen = true;
    qDebug() << "Database opened successfully";

    // 创建所有必要的表
    return createTables();
}

bool DatabaseManager::createTables()
{
    if (!isOpen) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    // 启用外键约束（SQLite需要显式启用）
    if (!query.exec("PRAGMA foreign_keys = ON;")) {
        qDebug() << "Failed to enable foreign keys:" << query.lastError().text();
        return false;
    }

    // 创建用户表
    QStringList tables = {// 用户表
                          "CREATE TABLE IF NOT EXISTS users ("
                          "userID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "username TEXT UNIQUE NOT NULL,"
                          "password TEXT NOT NULL,"
                          "nickname TEXT,"
                          "avatarURL TEXT,"
                          "phone TEXT,"
                          "default_address TEXT,"
                          "balance REAL DEFAULT 0.0,"
                          "registerTime TEXT,"
                          "userLevel INTEGER DEFAULT 1"
                          ");",

                          // 产品表
                          "CREATE TABLE IF NOT EXISTS products ("
                          "productID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "description TEXT,"
                          "brief_description TEXT,"
                          "specification TEXT,"
                          "brand TEXT,"
                          "productName TEXT NOT NULL,"
                          "category TEXT,"
                          "sellerID INTEGER,"
                          "salesCount INTEGER DEFAULT 0"
                          ");",

                          // 产品图片表
                          "CREATE TABLE IF NOT EXISTS product_images ("
                          "imageID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "productID INTEGER,"
                          "imageURL TEXT NOT NULL,"
                          "FOREIGN KEY (productID) REFERENCES products(productID) ON DELETE CASCADE"
                          ");",

                          // 产品分类表
                          "CREATE TABLE IF NOT EXISTS product_classes ("
                          "classID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "productID INTEGER,"
                          "stock INTEGER DEFAULT 0,"
                          "small_imageURL TEXT,"
                          "name TEXT NOT NULL,"
                          "price REAL DEFAULT 0.0,"
                          "FOREIGN KEY (productID) REFERENCES products(productID) ON DELETE CASCADE"
                          ");",

                          // 订单表
                          "CREATE TABLE IF NOT EXISTS orders ("
                          "orderID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "userID INTEGER,"
                          "sellerID INTEGER,"
                          "totalAmount REAL DEFAULT 0.0,"
                          "status INTEGER DEFAULT 1,"
                          "address TEXT,"
                          "createdTime TEXT DEFAULT CURRENT_TIMESTAMP"
                          ");",

                          // 订单项表
                          "CREATE TABLE IF NOT EXISTS order_items ("
                          "itemID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "orderID INTEGER,"
                          "productID INTEGER,"
                          "classID INTEGER,"
                          "mount INTEGER DEFAULT 1,"
                          "FOREIGN KEY (orderID) REFERENCES orders(orderID) ON DELETE CASCADE"
                          ");"};

    for (const QString &tableSql : tables) {
        if (!query.exec(tableSql)) {
            qDebug() << "Failed to create table:" << query.lastError().text();
            qDebug() << "SQL:" << tableSql;
            return false;
        }
    }

    qDebug() << "All tables created successfully";
    return true;
}
