#ifndef lrgDataCreatorI_h
#define lrgDataCreatorI_h
#include <vector>

// pdd stands for pair of doubles, i.e. pair<double, double>
typedef std::vector<std::pair<double, double>> pdd_vector;

class lrgDataCreatorI
{
public:
    virtual pdd_vector GetData() = 0;
};

#endif