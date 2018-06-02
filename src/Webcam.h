//
// Created by Leonard Geier on 02.06.18.
//

#ifndef SNAILSNAP_WEBCAM_H
#define SNAILSNAP_WEBCAM_H


#include <QObject>
#include <QCamera>
#include <QCameraImageCapture>

class Webcam : public QObject{
public:
    explicit Webcam(QCamera* cam);

public slots:
    QImage* capture();

private:
    static const QString path;
    QCameraImageCapture imageCapture;
};

/*class CaptureWrapper : public QObject {
public:
    explicit CaptureWrapper(QCamera* c) {
        imgCap = new QCameraImageCapture(c);
    }

    ~CaptureWrapper() {
        delete imgCap;
    }

public slots:
    void capture() {
        std::cout << "yoooo" << std::endl;
        QString path("/Users/leo/Projects/snailsnap-mosaic/test.jpg");
        imgCap->capture(path);
    };

private:
    QCameraImageCapture* imgCap;
};*/
#endif //SNAILSNAP_WEBCAM_H
