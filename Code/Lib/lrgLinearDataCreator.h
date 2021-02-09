#ifndef lrgLinearDataCreator_h
#define lrgLinearDataCreator_h
#include "lrgDataCreatorI.h"
#include <memory>
class lrgLinearDataCreator : public lrgDataCreatorI
{
public:
    lrgLinearDataCreator(const double t0, const double t1, const unsigned int size, std::unique_ptr<std::vector<std::pair<double, double>>> vec);
    lrgLinearDataCreator();
    ~lrgLinearDataCreator();
    virtual std::vector<std::pair<double, double>> GetData();

private:
    unsigned int m_size;
    double m_t0;
    double m_t1;
    std::unique_ptr<std::vector<std::pair<double, double>>> m_vec;
};

#endif