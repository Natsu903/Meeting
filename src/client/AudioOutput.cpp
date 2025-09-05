#include "AudioOutput.h"
#include <QMutexLocker>
#include "netheader.h"
#include <QDebug>
#include <QHostAddress>

#ifndef FRAME_LEN_125MS
#define FRAME_LEN_125MS 1900
#endif
extern QUEUE_DATA<MESG> audio_recv; //音频接收队列

AudioOutput::AudioOutput(QObject *parent)
	: QThread(parent)
{
	// 设置音频格式
	QAudioFormat format;
	format.setSampleRate(8000);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);

	// 检查默认输出设备是否支持该格式
	QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
	if (!info.isFormatSupported(format))
	{
		qWarning() << "Raw audio format not supported by backend, cannot play audio.";
		return;
	}

	audio = new QAudioOutput(format, this);
	connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State) ));
	outputdevice = nullptr;
}

AudioOutput::~AudioOutput()
{
	delete audio;
}

QString AudioOutput::errorString()
{
	if (audio->error() == QAudio::OpenError)
	{
		return QString("AudioOutput An error occurred opening the audio device").toUtf8();
	}
	else if (audio->error() == QAudio::IOError)
	{
		return QString("AudioOutput An error occurred during read/write of audio device").toUtf8();
	}
	else if (audio->error() == QAudio::UnderrunError)
	{
		return QString("AudioOutput Audio data is not being fed to the audio device at a fast enough rate").toUtf8();
	}
	else if (audio->error() == QAudio::FatalError)
	{
		return QString("AudioOutput A non-recoverable error has occurred, the audio device is not usable at this time.");
	}
	else
	{
		return QString("AudioOutput No errors have occurred").toUtf8();
	}
}

void AudioOutput::handleStateChanged(QAudio::State state)
{
	switch (state)
	{
		// 音频设备处于活跃状态
		case QAudio::ActiveState:
			break;
		// 音频设备被挂起
		case QAudio::SuspendedState:
			break;
		// 音频设备已停止
		case QAudio::StoppedState:
			if (audio->error() != QAudio::NoError)
			{
				audio->stop();
				emit audiooutputerror(errorString());
				qDebug() << "out audio error" << audio->error();
			}
			break;
		// 音频设备空闲
		case QAudio::IdleState:
			break;
		// 音频设备被中断
		case QAudio::InterruptedState:
			break;
		default:
			break;
	}
}

void AudioOutput::startPlay()
{
	if (audio->state() == QAudio::ActiveState) return;
	WRITE_LOG("start playing audio");
	// 启动音频输出设备
	outputdevice = audio->start();
}

void AudioOutput::stopPlay()
{
	if (audio->state() == QAudio::StoppedState) return;
	{
		QMutexLocker lock(&device_lock);
		outputdevice = nullptr;
	}
	// 停止音频设备
	audio->stop();
	WRITE_LOG("stop playing audio");
}

void AudioOutput::run()
{
	is_canRun = true;
	QByteArray m_pcmDataBuffer;

	WRITE_LOG("start playing audio thread 0x%p", QThread::currentThreadId());
	for (;;)
	{
		// 检查线程是否需要退出
		{
			QMutexLocker lock(&m_lock);
			if (is_canRun == false)
			{
				stopPlay();
				WRITE_LOG("stop playing audio thread 0x%p", QThread::currentThreadId());
				return;
			}
		}

		// 从音频接收队列中取出消息
		MESG* msg = audio_recv.pop_msg();
		if (msg == NULL) continue;
		{
			// 处理音频数据
			QMutexLocker lock(&device_lock);
			if (outputdevice != nullptr)
			{
				// 将数据添加到缓冲区
				m_pcmDataBuffer.append((char*)msg->data, msg->len);

				if (m_pcmDataBuffer.size() >= FRAME_LEN_125MS)
				{
					//写入音频数据到设备
					qint64 ret = outputdevice->write(m_pcmDataBuffer.data(), FRAME_LEN_125MS);
					if (ret < 0)
					{
						qDebug() << outputdevice->errorString();
						return;
					}
					else
					{
						emit speaker(QHostAddress(msg->ip).toString());
						m_pcmDataBuffer = m_pcmDataBuffer.right(m_pcmDataBuffer.size() - ret);
					}
				}
			}
			else
			{
				// 输出设备为空时清空缓冲区
				m_pcmDataBuffer.clear();
			}
		}
		// 释放消息内存
		if (msg->data) free(msg->data);
		if (msg) free(msg);
	}
}
void AudioOutput::stopImmediately()
{
	QMutexLocker lock(&m_lock);
	is_canRun = false;
}


void AudioOutput::setVolumn(int val)
{
	audio->setVolume(val / 100.0);
}

void AudioOutput::clearQueue()
{
	qDebug() << "audio recv clear";
	audio_recv.clear();
}
