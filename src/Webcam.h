//
// Created by Leonard Geier on 02.06.18.
//

#pragma once

#include <QObject>
#include <QCamera>
#include <QCameraImageCapture>
#include <QImage>

class Webcam : public QObject {
    Q_OBJECT

public:
    Webcam();
    ~Webcam() override;

signals:
    void imageReady(std::shared_ptr<QImage> img);

public slots:
    void captureImage();

private:
    QCamera* cam;
    QCameraImageCapture* imgCapture;

    QString path;
};
