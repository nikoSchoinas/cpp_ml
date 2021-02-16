#ifndef lrgNormalEquationSolverStrategy_h
#define lrgNormalEquationSolverStrategy_h
#include "lrgLinearModelSolverStrategyI.h"

class lrgNormalEquationSolverStrategy : public lrgLinearModelSolverStrategyI
{
public:
    lrgNormalEquationSolverStrategy(/* args */);
    ~lrgNormalEquationSolverStrategy();
    virtual pair_double FitData(pair_vector_double vec);
};

#endif