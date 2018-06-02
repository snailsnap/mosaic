#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QDir>
#include <QString>

#include "mosaic.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("SnailSnap");

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption inputImageOption({ "i", "input" }, "Use given input image instead of camera input.", "input image");
    parser.addOption(inputImageOption);
    QCommandLineOption outputImageOption({ "o", "output" }, "Store the output image to this file.", "output image");
    parser.addOption(outputImageOption);
    QCommandLineOption dataDirOption({ "d", "data" }, "Location of the data files.", "data dir", "data");
    parser.addOption(dataDirOption);

    parser.process(app);

    QString input;

    if (parser.isSet(inputImageOption))
    {
        input = parser.value(inputImageOption);
        std::cout << "Input image: " << input.toStdString() << std::endl;
    }
    else
    {
        std::cout << "No input image specified." << std::endl;
        exit(-1);
    }

    QString output;

    if (parser.isSet(outputImageOption))
    {
        output = parser.value(outputImageOption);
        std::cout << "Output image: " << output.toStdString() << std::endl;
    }
    else
    {
        std::cout << "No output image specified." << std::endl;
        exit(-1);
    }

    QString data("data");

    if (parser.isSet(dataDirOption))
    {
        data = parser.value(dataDirOption);
        std::cout << "Data directory: " << data.toStdString() << std::endl;
    }

    if (!QDir(data).exists())
    {
        std::cout << "Data directory is missing: " << data.toStdString() << std::endl;
        exit(-1);
    }

    QImage image(input);

    QImage* result = createMosaic(image);

    result->save(output);

    delete result;
}
