#include "painter.hpp"

#include <QPainter>

std::vector<Mollusc*> Painter::paint(const std::vector<MolluscPosition*>* molluscs, MolluscPalette* palette, QImage & colorImage, QImage & idImage)
{
    //auto colorImage = new QImage(width, height, input.format());
    colorImage.fill(Qt::GlobalColor::white);
    QPainter painter(&colorImage);
    idImage.fill(Qt::GlobalColor::black);
    QPainter idPainter(&idImage);

    auto i = 1;
    for (auto pos : *molluscs) {
        auto mollusc = palette->getClosestColor(pos->color); //TODO make getClosestColor return pointer
        
        // todo: better drawing with save/translate/rotate/restore
        if (mollusc.m_imageName.compare("NONE") != 0) {
            painter.drawPixmap(pos->x - pos->width / 2, pos->y - pos->height / 2, pos->width, pos->height, mollusc.m_image);
            auto mask = mollusc.m_image.copy();
            auto tintPainter = new QPainter(&mask);
            tintPainter->setCompositionMode(QPainter::CompositionMode_SourceIn);
            auto r = i & 0xff;
            auto g = (i >> 8) & 0xff;
            auto b = (i >> 16) & 0xff;
            tintPainter->fillRect(mask.rect(), QColor(r, g, b));
            idPainter.drawPixmap(pos->x - pos->width / 2, pos->y - pos->height / 2, pos->width, pos->height, mask);
        }

        ++i;
    }

    return std::vector<Mollusc*>();
}
