#include "painter.hpp"

#include <iostream>

#include <QPainter>

std::vector<Mollusc*>* Painter::paint(const std::vector<MolluscPosition*>* molluscPositions, MolluscPalette* palette, QImage & colorImage, QImage & idImage)
{
    colorImage.fill(Qt::GlobalColor::white);
    QPainter painter(&colorImage);
    idImage.fill(Qt::GlobalColor::white);
    QPainter idPainter(&idImage);

    auto i = 0;
    auto molluscs = new std::vector<Mollusc*>();
    for (auto pos : *molluscPositions) {
        auto mollusc = palette->getClosestColor(pos->color);
        
        // todo: better drawing with save/translate/rotate/restore
        if (mollusc->m_imageName.compare("NONE") != 0) {
            molluscs->push_back(mollusc);
            painter.drawPixmap(pos->x - pos->width / 2, pos->y - pos->height / 2, pos->width, pos->height, mollusc->m_image);
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
            idPainter.drawPixmap(pos->x - pos->width / 2, pos->y - pos->height / 2, pos->width, pos->height, QPixmap::fromImage(mask));
            ++i;
        }

    }

    return molluscs;
}
