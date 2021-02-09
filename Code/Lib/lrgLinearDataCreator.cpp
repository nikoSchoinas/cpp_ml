#include "lrgLinearDataCreator.h"
#include <random>
#include <functional>

lrgLinearDataCreator::lrgLinearDataCreator(){}
lrgLinearDataCreator::~lrgLinearDataCreator() {}
std::vector<std::pair<double, double>> lrgLinearDataCreator::GetData() 
{
    // We could use any generator.
    // std::default_random_engine generator;
    std::mt19937_64 mt64;

    // We use as a teplate the code example of "Hands-On Machine Learning" book, p.108,
    // where the noise is a random positive number in [0,1) from a uniform distribution.
    // That's from numpy.random.rand documentation.
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Bind the generator and the distribution together. It's more practical.
    auto rand_num = std::bind(distribution, mt64);

    // Declare x, y, noise variables outside of the for-loop.
    // In that way we can change their values inside for-loop without create new variables.
    double x;
    double y;
    double noise;

    for (unsigned int i = 0; i < m_size; i++)
    {
        x = rand_num();
        noise = rand_num();
        y = m_t1*x + m_t0 + noise;
        (*m_vec).push_back(std::make_pair(x,y));
    }

    return (*m_vec);
}