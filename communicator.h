#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QTcpSocket>
#include "com_protocol.h"
//
//class Client : public QObject {
//    Q_OBJECT
//
//public:
//    explicit Client(QTcpSocket* socket, QObject* parent = nullptr);
//
//    void sendLoginRequest(const LoginRequest& request);
//    void sendProductListRequest(const ProductListRequest& request);
//    void sendProductDetailRequest(const ProductDetailRequest& request);
//    void sendPurchaseRequest(const PurchaseRequest& request);
//    void sendSeckillRequest(const SeckillRequest& request);
//
//signals:
//    void loginResponseReceived(const LoginResponse& response);
//    void productListResponseReceived(const ProductListResponse& response);
//    void productDetailResponseReceived(const ProductDetailResponse& response);
//    void purchaseResponseReceived(const PurchaseResponse& response);
//    void seckillResponseReceived(const SeckillResponse& response);
//
//private slots:
//    void onReadyRead();
//    void onDisconnected();
//
//private:
//    QTcpSocket* socket;
//    void handleLoginResponse();
//    void handleProductListResponse();
//    void handleProductDetailResponse();
//    void handlePurchaseResponse();
//    void handleSeckillResponse();
//    template<typename T>
//    void sendMessage(const T& message);
//};












#endif // COMMUNICATOR_H
