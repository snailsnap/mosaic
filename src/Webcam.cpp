//
// Created by Leonard Geier on 02.06.18.
//

#include "Webcam.hpp"

#include <QFile>
#include <iostream>
#include <QCameraInfo>
#include <QCameraImageCapture>

Webcam::Webcam() : path("./.temp.jpg") {

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
    imgCapture->capture(path);
    cam->unlock();
}

void Webcam::emitImage() {
    std::shared_ptr<QImage> image = std::make_shared<QImage>(path);

    emit imageReady(image);

    // delete file
    if(!QFile::remove(path)) {
        std::cerr << "Warning: Failed to remove temporary webcam image at " << path.toStdString() << std::endl;
    }
}
