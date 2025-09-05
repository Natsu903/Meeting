/**
 * 用于处理会议中的用户
 */

#ifndef PARTNER_H
#define PARTNER_H

#include <QLabel>

class Partner : public QLabel
{
    Q_OBJECT
private:
    quint32 ip;// 参与者的IP地址

    void mousePressEvent(QMouseEvent *ev) override;
    int w;
public:
    Partner(QWidget * parent = nullptr, quint32 = 0);
    // 设置参与者头像
    void setpic(QImage img);
signals:
    // 当用户点击该参与者时发出此信号，发送对应的IP地址
    void sendip(quint32); //发送ip
};

#endif // PARTNER_H
