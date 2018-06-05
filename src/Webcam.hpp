//
// Created by Leonard Geier on 02.06.18.
//

#pragma once

#include <QObject>
#include <QCamera>
#include <QCameraImageCapture>
#include <QImage>
#include <memory>

class Webcam : public QObject {
    // Wraps QCamera and QCameraImageCapture so you don't have to bother with them
    // Use captureImage() to ... capture an image. This operation is probably asynchronous. YMMW
    // Once the image was taken, the imageReady(...) signal will fire

    Q_OBJECT

public:
    Webcam();
    ~Webcam() override;

signals:
    void imageReady(std::shared_ptr<QImage> img);

public slots:
    void captureImage();

private slots:
    void emitImage();

private:
    QCamera* cam;
    QCameraImageCapture* imgCapture;

    QString path;
};
