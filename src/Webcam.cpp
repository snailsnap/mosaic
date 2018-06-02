//
// Created by Leonard Geier on 02.06.18.
//

#include "Webcam.h"

#include <QFile>
#include <iostream>

const QString Webcam::path = QString("./.temp.jpg");

Webcam::Webcam(QCamera* cam) : imageCapture(cam, nullptr) {}

QImage *Webcam::capture() {
    imageCapture.capture(path);
    QImage *image = new QImage(path);// maybe think about returning a smarter pointer

    if (!QFile::remove(path)) {
        std::cerr << "Warning: Failed to remove temporary image" << std::endl;
    }

    return image;
}
