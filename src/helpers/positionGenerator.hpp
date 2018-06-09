#include <random>

#include "../../dependencies/jc_voronoi/src/jc_voronoi.h"

class RandomPostionGenerator
{
public:
    RandomPostionGenerator(float minX, float minY, float maxX, float maxY, float meanX, float meanY, float sigmaX, float sigmaY);
    ~RandomPostionGenerator();
    void getPosition(jcv_point* const result);

private:
    float m_minX
        , m_minY
        , m_maxX
        , m_maxY;
    std::mt19937_64* m_generator;
    std::normal_distribution<float>* m_horizontal;
    std::normal_distribution<float>* m_vertical;
};