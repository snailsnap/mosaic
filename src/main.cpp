#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QPixmap>

#include "mainwindow.hpp"
#include "mosaic.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("SnailSnap");
	app.setStyleSheet("*{text-align: left; border: none; background: white}");

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption inputImageOption({ "i", "input" }, "Use given input image instead of camera input.", "input image");
    parser.addOption(inputImageOption);
    QCommandLineOption outputImageOption({ "o", "output" }, "Store the output image to this file.", "output image");
    parser.addOption(outputImageOption);

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

    QImage image(input);

	QWidget *widget = new QWidget;
	MainWindow mainWin(widget, image);
	mainWin.showMaximized();

	return app.exec();
}
