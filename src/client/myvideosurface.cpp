#include "myvideosurface.h"
#include <QVideoSurfaceFormat>
#include <QDebug>
MyVideoSurface::MyVideoSurface(QObject *parent):QAbstractVideoSurface(parent)
{

}

QList<QVideoFrame::PixelFormat> MyVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    // 如果是无句柄类型，则返回支持的像素格式列表
    if(handleType == QAbstractVideoBuffer::NoHandle)
    {
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32
                                                 << QVideoFrame::Format_ARGB32
                                                 << QVideoFrame::Format_ARGB32_Premultiplied
                                                 << QVideoFrame::Format_RGB565
                                                 << QVideoFrame::Format_RGB555;
    }
    else
    {
        return QList<QVideoFrame::PixelFormat>();
    }
}


bool MyVideoSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    //返回与视频帧像素格式等效的图像格式,如果能转换为有效的图像格式，则认为该格式是支持的
    //pixelFormat: 返回视频流中的像素格式
    return QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat()) != QImage::Format_Invalid;
}

//将视频流中像素格式转换成格式对等的图片格式
bool MyVideoSurface::start(const QVideoSurfaceFormat &format)
{
    if(isFormatSupported(format) && !format.frameSize().isEmpty())
    {
        QAbstractVideoSurface::start(format);
        return true;
    }
    return false;
}


//捕获每一帧视频，每一帧都会到present
bool MyVideoSurface::present(const QVideoFrame &frame)
{
    if(!frame.isValid())
    {
        stop();
        return false;
    }
    if(frame.isMapped())
    {
        // 发射帧可用信号
        emit frameAvailable(frame);
    }
    else
    {
        // 创建帧的副本并映射为只读模式
        QVideoFrame f(frame);
        f.map(QAbstractVideoBuffer::ReadOnly);
        emit frameAvailable(f);
    }

    return true;
}
