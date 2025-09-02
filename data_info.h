#ifndef DATA_INFO_H
#define DATA_INFO_H

#include <string>
#include <vector>

struct ProductClass
{
    int classID;
    int stock;
    std::string small_imageURL;
    std::string name;
    double price;
};

struct Product
{
    int productID;
    std::string description;
    std::string brief_description;
    std::vector<std::string> description_imageURLs;
    std::string specification;
    std::string brand;
    std::vector<ProductClass> product_class;
    std::string productName;
    std::string category;
    int sellerID;
    int salesCount;
};

struct User
{
    int userID;
    std::string username;
    std::string password;
    std::string nickname;
    std::string avatarURL;
    std::string phone;
    std::string default_address;
    double balance;
    std::string registerTime;
    int userLevel;
};
struct OrderItem
{
    int productID;
    int classID;
    int mount;
};

struct Cart
{
    int userID;
    std::vector<OrderItem> Items;
};

enum class OrderStatus {
    wait_to_pay = 1,
    collecting = 2,
    shipping = 3,
    finished = 4,
    canceled = 5

};

struct Order
{
    int orderID;
    int userID;
    int sellerID;
    double totalAmount;
    int status;
    std::string address;
    std::vector<OrderItem> orderitem;
};

#endif // DATA_INFO_H
