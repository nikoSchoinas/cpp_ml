#ifndef lrgGradientDescentSolverStrategy_h
#define lrgGradientDescentSolverStrategy_h
#include "lrgLinearModelSolverStrategyI.h"
#include <memory>

typedef std::unique_ptr<std::vector<std::pair<double, double>>> pair_vector_double_ptr;

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
    virtual pair_double FitData(pair_vector_double vec);
};

#endif