#ifndef lrgLinearModelSolverStrategyI_h
#define lrgLinearModelSolverStrategyI_h
#include <vector>

// pdd stands for pair of doubles, i.e. pair<double, double>
typedef std::vector<std::pair<double, double>> pdd_vector;
typedef std::pair<double, double> pdd;

class lrgLinearModelSolverStrategyI
{
public:
    virtual pdd FitData(pdd_vector vec) = 0;
};

#endif