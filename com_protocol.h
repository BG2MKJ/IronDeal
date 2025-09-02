#ifndef COM_PROTOCOL_H
#define COM_PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>
#include <QString>
#include <QByteArray>
#include "data_info.h"

// 协议版本号
const uint16_t PROTOCOL_VERSION = 0x0001;

// 消息类型枚举
enum class MessageType : uint16_t {
    UNKNOWN = 0,

    // 认证相关
    LOGIN_REQUEST,
    LOGIN_RESPONSE,
    REGISTER_REQUEST,
    REGISTER_RESPONSE,
    LOGOUT_REQUEST,
    LOGOUT_RESPONSE,

    // 用户管理
    GET_USER_INFO_REQUEST,
    GET_USER_INFO_RESPONSE,
    UPDATE_USER_INFO_REQUEST,
    UPDATE_USER_INFO_RESPONSE,

    // 商品管理
    GET_PRODUCT_LIST_REQUEST,
    GET_PRODUCT_LIST_RESPONSE,
    GET_PRODUCT_DETAIL_REQUEST,
    GET_PRODUCT_DETAIL_RESPONSE,
    CREATE_PRODUCT_REQUEST,
    CREATE_PRODUCT_RESPONSE,
    UPDATE_PRODUCT_REQUEST,
    UPDATE_PRODUCT_RESPONSE,
    DELETE_PRODUCT_REQUEST,
    DELETE_PRODUCT_RESPONSE,
    GET_MY_PRODUCTS_REQUEST,
    GET_MY_PRODUCTS_RESPONSE,

    // 购物车管理
    GET_CART_REQUEST,
    GET_CART_RESPONSE,
    ADD_TO_CART_REQUEST,
    ADD_TO_CART_RESPONSE,
    UPDATE_CART_ITEM_REQUEST,
    UPDATE_CART_ITEM_RESPONSE,
    REMOVE_CART_ITEM_REQUEST,
    REMOVE_CART_ITEM_RESPONSE,
    CLEAR_CART_REQUEST,
    CLEAR_CART_RESPONSE,

    // 订单管理
    CREATE_ORDER_REQUEST,
    CREATE_ORDER_RESPONSE,
    GET_ORDER_LIST_REQUEST,
    GET_ORDER_LIST_RESPONSE,
    GET_ORDER_DETAIL_REQUEST,
    GET_ORDER_DETAIL_RESPONSE,
    UPDATE_ORDER_STATUS_REQUEST,
    UPDATE_ORDER_STATUS_RESPONSE,
    CANCEL_ORDER_REQUEST,
    CANCEL_ORDER_RESPONSE,

    // 图片管理
    UPLOAD_IMAGE_REQUEST,
    UPLOAD_IMAGE_RESPONSE,
    DOWNLOAD_IMAGE_REQUEST,
    DOWNLOAD_IMAGE_RESPONSE,
    DELETE_IMAGE_REQUEST,
    DELETE_IMAGE_RESPONSE,

    // 促销功能
    APPLY_DISCOUNT_REQUEST,
    APPLY_DISCOUNT_RESPONSE,
    APPLY_COUPON_REQUEST,
    APPLY_COUPON_RESPONSE,

    // 系统消息
    HEARTBEAT,
    ERROR_RESPONSE,

    // 主题切换
    CHANGE_THEME_REQUEST,
    CHANGE_THEME_RESPONSE
};

// 错误码枚举
enum class ErrorCode : uint16_t {
    SUCCESS = 0,
    UNKNOWN_ERROR,
    DATABASE_ERROR,
    NETWORK_ERROR,
    AUTHENTICATION_FAILED,
    PERMISSION_DENIED,
    INVALID_REQUEST,
    RESOURCE_NOT_FOUND,
    INSUFFICIENT_BALANCE,
    INSUFFICIENT_STOCK,
    INVALID_IMAGE_FORMAT,
    IMAGE_TOO_LARGE,
    OPERATION_TIMEOUT
};

// 图片类型枚举
enum class ImageType : uint8_t {
    USER_AVATAR = 1,
    PRODUCT_MAIN,
    PRODUCT_DETAIL,
    PRODUCT_THUMBNAIL
};

// 协议头结构 (共16字节)
#pragma pack(push, 1)
struct ProtocolHeader {
    uint16_t magic;         // 魔数 0xDEAL
    uint16_t version;       // 协议版本
    uint16_t type;          // 消息类型
    uint16_t reserved;      // 保留字段
    uint32_t sequence_id;   // 序列号用于匹配请求响应
    uint32_t body_length;   // 消息体长度
};
#pragma pack(pop)

// 魔数定义
const uint16_t PROTOCOL_MAGIC = 0xDEAL;

// 最大数据块大小 (64KB - 协议头大小)
const uint32_t MAX_CHUNK_SIZE = 65536 - sizeof(ProtocolHeader);

// 响应基础结构
struct BaseResponse {
    ErrorCode error_code;
    std::string error_msg;
};

// 登录请求
struct LoginRequest {
    std::string username;
    std::string password;
};

// 登录响应
struct LoginResponse : BaseResponse {
    int user_id;
    std::string nickname;
    std::string avatar_url;
    double balance;
};

// 注册请求
struct RegisterRequest {
    std::string username;
    std::string password;
    std::string nickname;
    std::string phone;
};

// 注册响应
struct RegisterResponse : BaseResponse {
    int user_id;
};

// 用户信息请求
struct UserInfoRequest {
    int user_id;
};

// 用户信息响应
struct UserInfoResponse : BaseResponse {
    User user_info;
};

// 更新用户信息请求
struct UpdateUserInfoRequest {
    int user_id;
    std::string nickname;
    std::string phone;
    std::string default_address;
};

// 更新用户信息响应
struct UpdateUserInfoResponse : BaseResponse {
};

// 商品列表请求
struct ProductListRequest {
    int page;
    int page_size;
    std::string category;
    std::string keyword;
};

// 商品列表响应
struct ProductListResponse : BaseResponse {
    std::vector<Product> products;
    int total_count;
    int total_pages;
};

// 商品详情请求
struct ProductDetailRequest {
    int product_id;
};

// 商品详情响应
struct ProductDetailResponse : BaseResponse {
    Product product;
};

// 创建商品请求
struct CreateProductRequest {
    Product product;
};

// 创建商品响应
struct CreateProductResponse : BaseResponse {
    int product_id;
};

// 更新商品请求
struct UpdateProductRequest {
    Product product;
};

// 更新商品响应
struct UpdateProductResponse : BaseResponse {
};

// 删除商品请求
struct DeleteProductRequest {
    int product_id;
};

// 删除商品响应
struct DeleteProductResponse : BaseResponse {
};

// 获取我的商品请求
struct MyProductsRequest {
    int user_id;
    int page;
    int page_size;
};

// 获取我的商品响应
struct MyProductsResponse : BaseResponse {
    std::vector<Product> products;
    int total_count;
};

// 购物车操作基础请求
struct CartBaseRequest {
    int user_id;
};

// 获取购物车请求
struct GetCartRequest : CartBaseRequest {
};

// 获取购物车响应
struct GetCartResponse : BaseResponse {
    Cart cart;
};

// 添加到购物车请求
struct AddToCartRequest : CartBaseRequest {
    OrderItem item;
};

// 添加到购物车响应
struct AddToCartResponse : BaseResponse {
};

// 更新购物车项请求
struct UpdateCartItemRequest : CartBaseRequest {
    int product_id;
    int class_id;
    int quantity;
};

// 更新购物车项响应
struct UpdateCartItemResponse : BaseResponse {
};

// 移除购物车项请求
struct RemoveCartItemRequest : CartBaseRequest {
    int product_id;
    int class_id;
};

// 移除购物车项响应
struct RemoveCartItemResponse : BaseResponse {
};

// 清空购物车请求
struct ClearCartRequest : CartBaseRequest {
};

// 清空购物车响应
struct ClearCartResponse : BaseResponse {
};

// 创建订单请求
struct CreateOrderRequest {
    int user_id;
    std::string address;
    double discount;        // 折扣率 0-1
    std::string coupon_code;// 优惠券代码
};

// 创建订单响应
struct CreateOrderResponse : BaseResponse {
    int order_id;
    double final_amount;
};

// 订单列表请求
struct OrderListRequest {
    int user_id;
    int page;
    int page_size;
    int status_filter;      // 0表示所有状态
};

// 订单列表响应
struct OrderListResponse : BaseResponse {
    std::vector<Order> orders;
    int total_count;
};

// 订单详情请求
struct OrderDetailRequest {
    int order_id;
};

// 订单详情响应
struct OrderDetailResponse : BaseResponse {
    Order order;
};

// 更新订单状态请求
struct UpdateOrderStatusRequest {
    int order_id;
    OrderStatus new_status;
};

// 更新订单状态响应
struct UpdateOrderStatusResponse : BaseResponse {
};

// 取消订单请求
struct CancelOrderRequest {
    int order_id;
};

// 取消订单响应
struct CancelOrderResponse : BaseResponse {
};

// 图片元数据
struct ImageMeta {
    ImageType type;
    std::string filename;
    std::string format;     // jpg, png, etc.
    uint32_t file_size;
    int related_id;         // user_id or product_id
};

// 上传图片请求头
struct UploadImageRequestHeader {
    ImageMeta meta;
    uint32_t total_chunks;
    uint32_t chunk_size;
};

// 上传图片数据块
struct ImageChunk {
    uint32_t chunk_index;
    uint32_t chunk_size;
    QByteArray chunk_data;
};

// 上传图片响应
struct UploadImageResponse : BaseResponse {
    std::string image_url;
    std::string image_id;   // 用于后续引用
};

// 下载图片请求
struct DownloadImageRequest {
    std::string image_id;
    ImageType type;
    int related_id;
};

// 下载图片响应头
struct DownloadImageResponseHeader : BaseResponse {
    ImageMeta meta;
    uint32_t total_chunks;
    uint32_t chunk_size;
};

// 应用折扣请求
struct ApplyDiscountRequest {
    int order_id;
    double discount_rate;   // 折扣率 0-1
};

// 应用折扣响应
struct ApplyDiscountResponse : BaseResponse {
    double new_total;
};

// 应用优惠券请求
struct ApplyCouponRequest {
    int user_id;
    std::string coupon_code;
};

// 应用优惠券响应
struct ApplyCouponResponse : BaseResponse {
    double discount_amount;
    std::string description;
};

// 切换主题请求
struct ChangeThemeRequest {
    int user_id;
    std::string theme_name; // "light", "dark", etc.
};

// 切换主题响应
struct ChangeThemeResponse : BaseResponse {
    std::string current_theme;
};

// 错误响应
struct ErrorResponse {
    ErrorCode error_code;
    std::string error_message;
    uint32_t original_sequence_id;
};

// 心跳包
struct Heartbeat {
    uint64_t timestamp;
};

// 协议序列化/反序列化工具类
class ProtocolHelper {
public:
    // 计算校验和
    static uint16_t calculateChecksum(const QByteArray &data);

    // 序列化消息
    static QByteArray serializeMessage(MessageType type, const QByteArray &body, uint32_t sequence_id = 0);

    // 反序列化消息头
    static bool parseHeader(const QByteArray &data, ProtocolHeader &header);

    // 创建错误响应
    static QByteArray createErrorResponse(ErrorCode code, const std::string &message, uint32_t sequence_id);

    // 创建心跳包
    static QByteArray createHeartbeat();
};

#endif // COM_PROTOCOL_H
