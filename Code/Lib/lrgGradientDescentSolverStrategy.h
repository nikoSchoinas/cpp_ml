#ifndef lrgGradientDescentSolverStrategy_h
#define lrgGradientDescentSolverStrategy_h
#include "lrgLinearModelSolverStrategyI.h"
#include <memory>

class lrgGradientDescentSolverStrategy : public lrgLinearModelSolverStrategyI
{
private:
    double m_eta;
    unsigned int m_iterations;

public:
    lrgGradientDescentSolverStrategy(double &eta, unsigned int &iterations);
    lrgGradientDescentSolverStrategy();
    ~lrgGradientDescentSolverStrategy();
    void SetEta(double &eta);
    void SetIterations(unsigned int &iterations);
    virtual pdd FitData(pdd_vector vec);
};

#endif