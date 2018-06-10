#include "painter.hpp"

#include <iostream>

#include <QPainter>
#include <QtMath>

std::vector<Mollusc*>* Painter::paint(const std::vector<MolluscPosition*>* molluscPositions, MolluscPalette* palette, QImage & colorImage, QImage & idImage)
{
    colorImage.fill(Qt::GlobalColor::white);
    QPainter painter(&colorImage);
    idImage.fill(Qt::GlobalColor::white);
    QPainter idPainter(&idImage);

    std::random_device random;
    std::mt19937_64 generator(random());
    std::uniform_int_distribution<int> dist(0, 1);

    auto i = 0;
    auto molluscs = new std::vector<Mollusc*>();
    for (auto pos : *molluscPositions) {
        auto mollusc = palette->getClosestColor(pos->color);
        
        if (mollusc->m_imageName.compare("NONE") != 0) {
            molluscs->push_back(mollusc);

            auto angle = pos->rotation + dist(generator) * M_PI + mollusc->m_rotation;

            auto imageWidth = mollusc->m_image.width();
            auto imageHeight = mollusc->m_image.height();
            auto imageSizeRatio = (float)imageHeight / imageWidth;

            auto boxWidth = pos->width;
            auto boxHeight = pos->height;
            if (boxHeight > boxWidth)
            {
                auto temp = boxWidth;
                boxWidth = boxHeight;
                boxHeight = temp;
                angle += M_PI_2;
            }
            auto boxSizeRatio = (float)boxHeight / boxWidth;

            auto targetWidth = 0;
            auto targetHeight = 0;
            if (imageSizeRatio > boxSizeRatio)
            {
                targetHeight = boxHeight;
                targetWidth = targetHeight / imageSizeRatio;
            }
            else
            {
                targetWidth = boxWidth;
                targetHeight = targetWidth * imageSizeRatio;
            }

            const auto scale = 1.3;

            painter.save();
            painter.translate(pos->x, pos->y);
            painter.rotate(-qRadiansToDegrees(angle));
            painter.scale(scale, scale);
            painter.translate(-targetWidth / 2, -targetHeight / 2);
            painter.drawPixmap(0, 0, targetWidth, targetHeight, mollusc->m_image);
            painter.restore();

            auto mask = mollusc->m_image.toImage().scaled(pos->width, pos->height);
            auto tintPainter = new QPainter(&mask);
            tintPainter->setCompositionMode(QPainter::CompositionMode_SourceIn);
            auto r = i & 0xff;
            auto g = (i >> 8) & 0xff;
            auto b = (i >> 16) & 0xff;

            for (int y = 0; y < mask.height(); y++) {
                QRgb *line = (QRgb *)mask.scanLine(y);
                for (int x = 0; x < mask.width(); x++) {
                    // line[x] has an individual pixel
                    auto a = qAlpha(line[x]) < 127 ? 0 : 255;
                    line[x] = qRgba(r, g, b, a);
                }
            }

            tintPainter->fillRect(mask.rect(), QColor(r, g, b));
            tintPainter->end();

            idPainter.save();
            idPainter.translate(pos->x, pos->y);
            idPainter.rotate(-qRadiansToDegrees(angle));
            idPainter.scale(scale, scale);
            idPainter.translate(-targetWidth / 2, -targetHeight / 2);
            idPainter.drawImage(0, 0, mask);
            idPainter.restore();

            ++i;
        }

    }

    return molluscs;
}
