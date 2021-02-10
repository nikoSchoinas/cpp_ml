#ifndef lrgLinearDataCreator_h
#define lrgLinearDataCreator_h
#include "lrgDataCreatorI.h"
#include <memory>

typedef std::unique_ptr<std::vector<std::pair<double, double>>> pair_vector_double_ptr;

class lrgLinearDataCreator : public lrgDataCreatorI
{
public:
    lrgLinearDataCreator(const double t0, const double t1, const unsigned int size, pair_vector_double_ptr vec);
    lrgLinearDataCreator();
    ~lrgLinearDataCreator();
    virtual pair_vector_double GetData();

private:
    unsigned int m_size;
    double m_t0;
    double m_t1;
    pair_vector_double_ptr m_vec;
};

#endif