#pragma once

#include <vector>

#include <QThread>
#include <QString>

#include "../mollusc.hpp"

class ImageLoaderThread : public QThread
{
    Q_OBJECT
public:
    ImageLoaderThread(std::vector<Mollusc*>* molluscs, const QString& dir, int start, int end)
        : m_molluscs{ molluscs }
        , m_dir{ dir }
        , m_start{ start }
        , m_end{ end } {};
    void run() override;
private:
    std::vector<Mollusc*>* m_molluscs;
    const QString& m_dir;
    int m_start;
    int m_end;
signals:
    void resultReady(const QString &s);
};
