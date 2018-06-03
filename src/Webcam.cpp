//
// Created by Leonard Geier on 02.06.18.
//

#include "Webcam.h"

#include <QFile>
#include <iostream>
#include <QCameraInfo>

Webcam::Webcam() : path("./.temp.jpg") {

    if(QCameraInfo::availableCameras().count() > 0) {
        cam = new QCamera(QCameraInfo::availableCameras()[0]);
        cam->setCaptureMode(QCamera::CaptureStillImage);
        cam->start();

        imgCapture = new QCameraImageCapture(cam);
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
    std::cout << "sdufhkjsd\n";
}

void Webcam::imageReady(std::shared_ptr<QImage> img) {
}
