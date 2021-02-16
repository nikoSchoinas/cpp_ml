#ifndef lrgLinearModelSolverStrategyI_h
#define lrgLinearModelSolverStrategyI_h
#include <vector>

typedef std::vector<std::pair<double, double>> pair_vector_double;
typedef std::pair<double, double> pair_double;

class lrgLinearModelSolverStrategyI
{
public:
    virtual pair_double FitData(pair_vector_double vec) = 0;
};

#endif