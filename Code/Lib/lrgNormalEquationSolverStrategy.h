#ifndef lrgNormalEquationSolverStrategy_h
#define lrgNormalEquationSolverStrategy_h
#include "lrgLinearModelSolverStrategyI.h"


class lrgNormalEquationSolverStrategy : public lrgLinearModelSolverStrategyI
{
public:
    virtual std::pair<double, double> FitData(std::vector<std::pair<double, double> >);
    lrgNormalEquationSolverStrategy(/* args */);
    ~lrgNormalEquationSolverStrategy();
};

#endif