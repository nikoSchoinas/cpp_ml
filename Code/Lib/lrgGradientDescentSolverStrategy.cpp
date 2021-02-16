#include "lrgGradientDescentSolverStrategy.h"

// Passing values by reference.
lrgGradientDescentSolverStrategy::lrgGradientDescentSolverStrategy(double &eta, unsigned int &iterations, unsigned int &m)
{
    m_eta = eta;
    m_iterations = iterations;
    m_iterations = m;
}

// Empty constructor
lrgGradientDescentSolverStrategy::lrgGradientDescentSolverStrategy() {}

// Destructor
lrgGradientDescentSolverStrategy::~lrgGradientDescentSolverStrategy() {}

// Setters 
void lrgGradientDescentSolverStrategy::SetEta(double &eta)
{
    m_eta = eta;
}

void lrgGradientDescentSolverStrategy::SetIterations(unsigned int &iterations)
{
    m_iterations = iterations;
}

void lrgGradientDescentSolverStrategy::SetM(unsigned int &m)
{
    m_m = m;
}

pair_double lrgGradientDescentSolverStrategy::FitData(pair_vector_double vec)
{
    // add implementation. 
}