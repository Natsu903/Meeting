#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>


class MyVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    MyVideoSurface(QObject *parent = 0);

    //支持的像素格式，返回此视频表面支持的像素格式列表
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    //检查给定的视频格式是否被支持
    bool isFormatSupported(const QVideoSurfaceFormat &format) const override;
    bool start(const QVideoSurfaceFormat &format) override;
    bool present(const QVideoFrame &frame) override;
    // 获取视频矩形区域
    QRect videoRect() const;
    void updateVideoRect();
    // 绘制视频帧
    void paint(QPainter *painter);

signals:
    // 当有新帧可用时发出的信号
    void frameAvailable(QVideoFrame);

};

#endif // MYVIDEOSURFACE_H
