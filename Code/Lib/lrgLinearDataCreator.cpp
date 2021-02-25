#include "lrgLinearDataCreator.h"
#include <random>
#include <functional>


// t0,t1 are the coefficients of the linear function: y = t1*x + t0 + noise
// size is the length of the vector. Or similarly, how many pairs of (x,y) we want to add in it.
// vec_ptr is a pointer to a vector that hosts our data. 

lrgLinearDataCreator::lrgLinearDataCreator(double t0, double t1, unsigned int size, shared_ptr_pdd_vector vec_ptr)
{
    m_t0 = t0;
    m_t1 = t1;
    m_size = size;
    m_vec_ptr = std::move(vec_ptr);
}

// Empty constructor.
// It was made for testing purposes.
lrgLinearDataCreator::lrgLinearDataCreator()
{
    m_t0 = 0;
    m_t1 = 0;
    m_size = 0;
}

// Destructor
lrgLinearDataCreator::~lrgLinearDataCreator() {}

pdd_vector lrgLinearDataCreator::GetData()
{

    // If the attributes (m_t0, m_t1, m_size) are not specified throw an error.
    // This could happen if you create an object with the empty constructor and then call the GetData() method.
    if (m_t0 == 0 && m_t1 == 0 && m_size == 0)
    {
        throw std::invalid_argument("Class attributes have not been set correctly...");
    }

    if (m_size == 0)
    {
        throw std::invalid_argument("Vector size cannot be zero...");
    }
    
    
    // We could use any generator.
    // std::default_random_engine generator;
    std::mt19937_64 mt64;

    // We use as a template the code example of "Hands-On Machine Learning" book, p.108,
    // that uses numpy.random.rand which, according to the documentation, follows a uniform distribution.
    // In a few words, x is a random positive number in [0,1) from a uniform distribution.
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // We will use normal distribution for the noise as the code example of "Hands-On Machine Learning" book, p.108.
    // Normal distribution with mean:0.0 and standard deviation:1.0 .
    std::normal_distribution<double> noise_distribution(0.0, 1.0);

    // Bind the generator and the distribution together. It's more practical.
    auto rand_x = std::bind(distribution, mt64);
    auto rand_noise = std::bind(noise_distribution, mt64);

    // Declare x, y, noise.
    double x;
    double y;
    double noise;

    for (size_t i = 0; i < m_size; i++)
    {
        // The code example in "Hands-On Machine Learning" (p.108) multiply the random value x with the factor of 2.
        // This is useles since we can include this factor in the t0 factor.
        // e.g. instead of x = 2 * rand_x() and y = 4 + 3 * x + rand_noise()
        // we can compute
        // x = rand_x() and y = 4 + 6 * x + rand_noise()
        x = rand_x();
        noise = rand_noise();
        y = m_t1 * x + m_t0 + noise;
        m_vec_ptr->push_back(std::make_pair(x, y));
    }

    return (*m_vec_ptr);
}