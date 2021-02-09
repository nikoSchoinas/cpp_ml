#ifndef lrgNormalEquationSolverStrategy_h
#define lrgNormalEquationSolverStrategy_h
#include "lrgLinearModelSolverStrategyI.h"


class lrgNormalEquationSolverStrategy : public lrgLinearModelSolverStrategyI
{
public:
    virtual pair_double FitData(pair_vector_double);
    lrgNormalEquationSolverStrategy(/* args */);
    ~lrgNormalEquationSolverStrategy();
};

#endif