#include <iostream>

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("SnailSnap");

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
}
