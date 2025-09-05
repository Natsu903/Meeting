#include "recvsolve.h"
#include <QMetaType>
#include <QDebug>
#include <QMutexLocker>
extern QUEUE_DATA<MESG> queue_recv;

void RecvSolve::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}

RecvSolve::RecvSolve(QObject *par):QThread(par)
{
    //注册MESG指针类型，使其可以在信号和槽中使用
    qRegisterMetaType<MESG *>();
    m_isCanRun = true;
}

void RecvSolve::run()
{
    WRITE_LOG("start solving data thread: 0x%p", QThread::currentThreadId());
    for(;;)
    {
        {
            QMutexLocker locker(&m_lock);
            if (m_isCanRun == false)
            {
                WRITE_LOG("stop solving data thread: 0x%p", QThread::currentThreadId());
                return;
            }
        }
        // 从接收队列中取出消息
        MESG * msg = queue_recv.pop_msg();
        if(msg == NULL) continue;
        // 发出数据接收信号，将消息传递给连接的槽函数
        emit datarecv(msg);
    }
}
