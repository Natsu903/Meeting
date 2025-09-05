/**
 * 音频输入类，用于捕获和处理音频数据
 */

#pragma once

#include <QObject>
#include <QAudioInput>
#include <QIODevice>
class AudioInput : public QObject
{
	Q_OBJECT
private:
	QAudioInput *audio;//音频输入对象
	QIODevice* inputdevice;//音频输入设备IO接口
	char* recvbuf;//音频数据接收缓冲区
public:
	AudioInput(QObject *par = 0);
	~AudioInput();
private slots:
	//处理音频数据就绪
	void onreadyRead();
	//处理音频设备状态变化
	void handleStateChanged(QAudio::State);
	//获取音频输入错误信息
	QString errorString();
	//设置音频输入音量
	void setVolumn(int);
public slots:
	//开始音频采集
	void startCollect();
	//停止音频采集
	void stopCollect();
signals:
	//音频输入错误信号
	void audioinputerror(QString);
};
