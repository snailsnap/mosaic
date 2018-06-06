#include <iostream>
#include <fstream>

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QString>

#include "mainwindow.hpp"
#include "algorithms/floyd-steinberg.hpp"
#include "mollusc.hpp"
#include "molluscpalette.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("SnailSnap");
    app.setStyleSheet("*{text-align: left; border: none; background: white}");

    QCommandLineParser parser;
    parser.addHelpOption();

    // set up inputs
    QCommandLineOption outputImageOption({ "o", "output" }, "Store the output image to this file.", "output image");
    parser.addOption(outputImageOption);
    QCommandLineOption dataDirOption({ "d", "data" }, "Location of the data files.", "data dir", "data");
    parser.addOption(dataDirOption);
    QCommandLineOption maxNumOfMolluscsOption({ "n", "maxNumOfMolluscs" }, "Maximum number of molluscs to use for the image.", "maxNumOfMolluscs", "1000");
    parser.addOption(maxNumOfMolluscsOption);
    QCommandLineOption useCamOption({ "c", "useCam" }, "Whether the cam or the image dialog is used.", "useCam", "true");
    parser.addOption(useCamOption);

    parser.process(app);

    // check inputs

    QString output;
    if (parser.isSet(outputImageOption))
    {
        output = parser.value(outputImageOption);
        std::cout << "Output image: " << output.toStdString() << std::endl;
    }
    else
    {
        std::cout << "No output image specified. initializing to empty string" << std::endl;
        output = "";
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

    auto useCam = true;
    if (parser.isSet(useCamOption))
    {
        useCam = parser.value(useCamOption).toUpper() == "TRUE";
        std::cout << "Use Cam: " << (useCam?"true":"false") << std::endl;
    }

    auto molluscPalette = new MolluscPalette(data);

    MainWindow mainWin(nullptr, molluscPalette, useCam, output, maxNumOfMolluscs, data);
    mainWin.showMaximized();

    return app.exec();
}
