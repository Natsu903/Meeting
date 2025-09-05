/**
 * 聊天消息显示类
 * 该类用于在聊天界面中显示不同类型的消息，包括系统消息、用户消息和时间消息。
 * 支持左右对齐的消息显示，以及消息气泡、头像等视觉元素的绘制。
 */

#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QWidget>
#include <QLabel>

class ChatMessage:public QWidget
{
    Q_OBJECT
public:
    explicit ChatMessage(QWidget *parent = nullptr);

    enum User_Type{
        User_System,//系统
        User_Me,    //自己
        User_She,   //用户
        User_Time,  //时间
    };
    //设置文本发送成功状态
    void setTextSuccess();
    /**
     * @brief 设置消息文本内容及相关信息
     * @param text 消息文本内容
     * @param time 消息时间戳
     * @param allSize 整体尺寸
     * @param ip 发送者IP地址
     * @param userType 用户类型（默认为时间类型）
     */
    void setText(QString text, QString time, QSize allSize, QString ip = "",  User_Type userType = User_Time);
    // 计算字符串实际显示尺寸
    QSize getRealString(QString src);
    //计算字体显示区域
    QSize fontRect(QString str);

    inline QString text() {return m_msg;}
    inline QString time() {return m_time;}
    inline User_Type userType() {return m_userType;}
protected:
    //负责绘制消息气泡、文本、头像等界面元素
    void paintEvent(QPaintEvent *event);
private:
    QString m_msg;//文本内容
    QString m_time;//时间戳
    QString m_curTime;//当前时间
    QString m_ip;//IP地址

    QSize m_allSize;//整体尺寸
    User_Type m_userType = User_System;//用户类型

    int m_kuangWidth;//消息框宽度
    int m_textWidth;//文本区域宽度
    int m_spaceWid;//间隔宽度
    int m_lineHeight;//行高

    //IP显示区域
    QRect m_ipLeftRect;
    QRect m_ipRightRect;
    //头像显示区域
    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    //气泡三角区域
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
    //消息框区域
    QRect m_kuangLeftRect;
    QRect m_kuangRightRect;
    //文本显示区域
    QRect m_textLeftRect;
    QRect m_textRightRect;
    //头像图片
    QPixmap m_leftPixmap;
    QPixmap m_rightPixmap;
    //动画加载
    QLabel* m_loading = Q_NULLPTR;
    QMovie* m_loadingMovie = Q_NULLPTR;
    bool m_isSending = false;
};

#endif // CHATMESSAGE_H
