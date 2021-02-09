#ifndef lrgDataCreatorI_h
#define lrgDataCreatorI_h
#include <vector>

typedef std::vector<std::pair<double, double>> pair_vector_double;

class lrgDataCreatorI
{
public:
    virtual pair_vector_double GetData() = 0;
};

#endif