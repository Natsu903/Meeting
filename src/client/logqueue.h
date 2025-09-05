#ifndef LOGQUEUE_H
#define LOGQUEUE_H

#include <QThread>
#include <QMutex>
#include <queue>
#include "netheader.h"

class LogQueue : public QThread
{
private:
    void run();
    QMutex m_lock;
    bool m_isCanRun;//线程运行标志，控制线程的启停

    QUEUE_DATA<Log> log_queue;//日志消息队列
    FILE *logfile;//日志文件指针
public:
    explicit LogQueue(QObject *parent = nullptr);
    void stopImmediately();
    void pushLog(Log*);
};

#endif // LOGQUEUE_H
