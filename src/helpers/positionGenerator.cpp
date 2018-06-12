#include <random>

#include "positionGenerator.hpp"

RandomPostionGenerator::RandomPostionGenerator(float minX, float minY, float maxX, float maxY, float meanX, float meanY, float sigmaX, float sigmaY)
    : m_minX(minX)
    , m_minY(minY)
    , m_maxX(maxX)
    , m_maxY(maxY)
{
    std::random_device random;
    
    m_generator = new std::mt19937_64(random());

    m_horizontal = new std::normal_distribution<float>(meanX, sigmaX);
    m_vertical = new std::normal_distribution<float>(meanY, sigmaY);
}

RandomPostionGenerator::~RandomPostionGenerator()
{
    delete m_generator;
    delete m_horizontal;
    delete m_vertical;
}

void RandomPostionGenerator::getPosition(jcv_point* const result)
{
    auto x = m_minX - 1;
    auto y = m_minY - 1;
    while (x < m_minX || x > m_maxX || y < m_minY || y > m_maxY)
    {
        x = (*m_horizontal)(*m_generator);
        y = (*m_vertical)(*m_generator);
    }
    result->x = x;
    result->y = y;
}
