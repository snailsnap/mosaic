//
// Created by Leonard Geier on 02.06.18.
//

#include "Webcam.hpp"

#include <QFile>
#include <iostream>
#include <QCameraInfo>
#include <QCameraImageCapture>

Webcam::Webcam() {

    if(QCameraInfo::availableCameras().count() > 0) {
        cam = new QCamera(QCameraInfo::availableCameras()[0]);
        cam->setCaptureMode(QCamera::CaptureStillImage);
        cam->start();

        imgCapture = new QCameraImageCapture(cam);

        QObject::connect(imgCapture, &QCameraImageCapture::imageSaved, this, &Webcam::emitImage);
    }
    else {
        cam = nullptr;
    }
}

Webcam::~Webcam() {
    cam->stop();
    delete imgCapture;
    delete cam;
}


void Webcam::captureImage() {
    cam->searchAndLock();
    imgCapture->capture();
    cam->unlock();
}

void Webcam::emitImage(int id, const QString &savePath) {
    std::shared_ptr<QImage> image = std::make_shared<QImage>(savePath);

    emit imageReady(image);

    // delete file
    if(!QFile::remove(savePath)) {
        std::cerr << "Warning: Failed to remove temporary webcam image at " << savePath.toStdString() << std::endl;
    }
}
