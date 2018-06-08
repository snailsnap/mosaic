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
            auto mask = mollusc->m_image.toImage();
            auto tintPainter = new QPainter(&mask);
            tintPainter->setCompositionMode(QPainter::CompositionMode_SourceIn);
            auto r = i & 0xff;
            auto g = (i >> 8) & 0xff;
            auto b = (i >> 16) & 0xff;

            for (int y = 0; y < mask.height(); y++) {
                for (int x = 0; x < mask.width(); x++) {
                    auto a = mask.pixelColor(x, y).alpha()<127?0:255;
                    mask.setPixelColor(x, y, QColor(r,g,b,a));
                }
            }
            tintPainter->fillRect(mask.rect(), QColor(r, g, b));
            tintPainter->end();
            idPainter.drawPixmap(pos->x - pos->width / 2, pos->y - pos->height / 2, pos->width, pos->height, QPixmap::fromImage(mask));
            ++i;
        }

    }

    for (auto y = 0; y < idImage.height(); ++y) {
        for (auto x = 0; x < idImage.width(); ++x) {
            auto r = idImage.pixelColor(x, y).red();
            auto g = idImage.pixelColor(x, y).green();
            auto b = idImage.pixelColor(x, y).blue();

            if (b > 0 && r < 255 && g << 255) {
                std::cout << "color(" + std::to_string(r) << "," << std::to_string(g) << "," << std::to_string(b) << ")" << std::endl;
            }
        }
    }

    return molluscs;
}
