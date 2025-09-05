/**
 * 音频输出类，用于播放音频数据
 */

#pragma once

#include <QObject>
#include <QThread>
#include <QAudioOutput>
#include <QMutex>
class AudioOutput : public QThread
{
	Q_OBJECT
private:
	QAudioOutput* audio;//Qt音频输出对象
	QIODevice* outputdevice;//音频输出设备IO接口
	QMutex device_lock;//互斥锁,保护outputdevice
	
	volatile bool is_canRun;//线程运行标志
	QMutex m_lock;//互斥锁,保护is_canRun

	void run();
	QString errorString();
public:
	AudioOutput(QObject *parent = 0);
	~AudioOutput();
	void stopImmediately();
	void startPlay();
	void stopPlay();
private slots:
	//处理音频设备状态变化
	void handleStateChanged(QAudio::State);
	//设置音频输出音量
	void setVolumn(int);
	//清空音频播放队列
	void clearQueue();
signals:
	//音频播放错误信号
	void audiooutputerror(QString);
	//发言者信号
	void speaker(QString);
};
