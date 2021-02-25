#ifndef lrgNormalEquationSolverStrategy_h
#define lrgNormalEquationSolverStrategy_h
#include "lrgLinearModelSolverStrategyI.h"

class lrgNormalEquationSolverStrategy : public lrgLinearModelSolverStrategyI
{
public:
    lrgNormalEquationSolverStrategy();
    ~lrgNormalEquationSolverStrategy();
    virtual pdd FitData(pdd_vector vec);
};

#endif