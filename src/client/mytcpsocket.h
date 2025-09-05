/**
 * TCP套接字通信类头文件
 * 用于处理客户端与服务器之间的TCP通信。包括连接管理、数据发送和接收等功能。
 */

#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include "netheader.h"
#ifndef MB
#define MB (1024 * 1024)
#endif

typedef unsigned char uchar;


class MyTcpSocket: public QThread
{
    Q_OBJECT
public:
    ~MyTcpSocket();
    MyTcpSocket(QObject *par=NULL);
    //建立与指定服务器的TCP连接
    bool connectToServer(QString, QString, QIODevice::OpenModeFlag);
    //获取错误信息
    QString errorString();
    //断开与服务器的连接,停止所有线程并清理资源
    void disconnectFromHost();
    //获取本地IP地址
    quint32 getlocalip();
private:
    //负责从发送队列中获取消息并发送到服务器
    void run() override;
    /**
     * @brief 从套接字读取指定字节数的数据
     * @param buf 读取数据的缓冲区
     * @param maxsize 缓冲区最大大小
     * @param n 需要读取的字节数
     * @return 实际读取的字节数
     */
    qint64 readn(char *, quint64, int);

private:
    QTcpSocket *_socktcp;//TCP套接字对象
    QThread *_sockThread;//用于接收数据的线程
    uchar *sendbuf;//发送数据缓冲区
    uchar* recvbuf;//接收数据缓冲区
    quint64 hasrecvive;//已接收数据的字节数

    QMutex m_lock;
    volatile bool m_isCanRun;
private slots:
    /**
     * @brief 实际连接服务器的函数
     * @param ip 服务器IP地址
     * @param port 服务器端口号
     * @param flag 连接模式
     * @return 连接是否成功
     */
    bool connectServer(QString, QString, QIODevice::OpenModeFlag);

    void sendData(MESG *);
    void closeSocket();

public slots:
    //处理来自服务器的数据包
    void recvFromSocket();
    //立即停止套接字线程
    void stopImmediately();
    void errorDetect(QAbstractSocket::SocketError error);
signals:
    //套接字错误信号
    void socketerror(QAbstractSocket::SocketError);
    void sendTextOver();
};

#endif // MYTCPSOCKET_H
