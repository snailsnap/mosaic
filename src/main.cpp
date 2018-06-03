#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QPushButton>
#include "Webcam.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("SnailSnap");

    QPushButton button( "Capture");
    button.show();

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption inputImageOption({ "i", "input" }, "Use given input image instead of camera input.", "input image");
    parser.addOption(inputImageOption);

    parser.process(app);

    if (parser.isSet(inputImageOption))
    {
        auto inputImage = parser.value(inputImageOption);
        std::cout << "Input image: " << inputImage.toStdString() << std::endl;
    }
    else
    {
        std::cout << "No input image specified." << std::endl;
    }


    Webcam w;
    QObject::connect(&button, &QPushButton::clicked, &w, &Webcam::captureImage);

    return app.exec();
}
