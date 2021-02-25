#ifndef lrgLinearDataCreator_h
#define lrgLinearDataCreator_h
#include "lrgDataCreatorI.h"
#include <memory>

// pdd stand for pair of doubles, i.e. pair<double, double>
typedef std::shared_ptr<std::vector<std::pair<double, double>>> shared_ptr_pdd_vector; 
class lrgLinearDataCreator : public lrgDataCreatorI
{
public:
    lrgLinearDataCreator(double t0, double t1, unsigned int size, shared_ptr_pdd_vector vec_ptr);
    lrgLinearDataCreator();
    ~lrgLinearDataCreator();
    virtual pdd_vector GetData();

private:
    unsigned int m_size;
    double m_t0;
    double m_t1;
    shared_ptr_pdd_vector m_vec_ptr;
};

#endif