#include <iostream>
#include <fstream>

#include <QApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QDir>
#include <QString>
#include <QWidget>
#include <QDesktopWidget>

#include "mainwindow.hpp"
#include "algorithms/floyd-steinberg.hpp"
#include "mollusc.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("SnailSnap");
    app.setStyleSheet("*{text-align: left; border: none; background: white}");

    QCommandLineParser parser;
    parser.addHelpOption();

    // set up inputs
    QCommandLineOption inputImageOption({ "i", "input" }, "Use given input image instead of camera input.", "input image");
    parser.addOption(inputImageOption);
    QCommandLineOption outputImageOption({ "o", "output" }, "Store the output image to this file.", "output image");
    parser.addOption(outputImageOption);
    QCommandLineOption dataDirOption({ "d", "data" }, "Location of the data files.", "data dir", "data");
    parser.addOption(dataDirOption);
    QCommandLineOption maxNumOfMolluscsOption({ "n", "maxNumOfMolluscs" }, "Maximum number of molluscs to use for the image.", "maxNumOfMolluscs", "1000");
    parser.addOption(maxNumOfMolluscsOption);

    parser.process(app);

    // check inputs
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

    auto maxNumOfMolluscs = 1000;
    if (parser.isSet(maxNumOfMolluscsOption))
    {
        maxNumOfMolluscs = parser.value(maxNumOfMolluscsOption).toInt();
        std::cout << "Maximum number of molluscs: " << maxNumOfMolluscs << std::endl;
    }

    // read input image
    QRect display = QApplication::desktop()->screenGeometry();
    QImage image = QImage(input).scaled(display.size(), Qt::KeepAspectRatio);

    // read meta file
    std::ifstream stream(data.toStdString() + "/meta_file.csv");
    std::vector<std::string> strings;
    std::string string;
    while (std::getline(stream, string))
    {
        strings.push_back(string);
    }

    // generate molluscs
    std::vector<Mollusc> molluscs;
    for (auto i = 1u; i < strings.size(); ++i)
    {
        molluscs.push_back(Mollusc(strings[i], data));
    }

    // white mollusc for background
    molluscs.push_back(Mollusc("NONE;#FFFFFF;0.0;1.0;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE"));

    auto mosaic = FloydSteinberg(molluscs);
    auto result = mosaic.createMosaic(image, maxNumOfMolluscs);

    result->save(output);

    QWidget *widget = new QWidget;
    MainWindow mainWin(widget, result);
    mainWin.showMaximized();

    return app.exec();
}
