//
// Created by Leonard Geier on 02.06.18.
//

#include "Webcam.hpp"

#include <memory>
#include <iostream>

#include <QFile>
#include <QCameraInfo>
#include <QCameraImageCapture>


Webcam::Webcam() {
    cams = QCameraInfo::availableCameras();

    if (!cams.count())
        return;

    cam = std::make_shared<QCamera>(cams[0]);

    if (cam->isCaptureModeSupported(QCamera::CaptureStillImage)) {
        cam->setCaptureMode(QCamera::CaptureStillImage);
    } else {
        std::cerr << "Camera does not support still image capture" << std::endl;
        cam.reset();
        return;
    }

    cam->start();

    imgCapture = std::make_shared<QCameraImageCapture>(cam.get());
    std::cerr << "Capture to buffer supported: "
              << std::boolalpha
              << imgCapture->isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer)
              << std::endl;

    QObject::connect(imgCapture.get(), &QCameraImageCapture::imageSaved, this, &Webcam::emitImage);
}

Webcam::~Webcam() {
    cam->stop();
}

bool Webcam::supported() const {
    return !cams.empty();
}

void Webcam::captureImage() {
    cam->searchAndLock();
    imgCapture->capture();
    cam->unlock();
}

void Webcam::emitImage(int id, const QString &savePath) {
    std::shared_ptr<QImage> image = std::make_shared<QImage>(savePath);

    emit imageReady(image);

    std::cerr << savePath.toStdString() << std::endl;

    // delete file
    if(!QFile::remove(savePath)) {
        std::cerr << "Warning: Failed to remove temporary webcam image at " << savePath.toStdString() << std::endl;
    }
}
