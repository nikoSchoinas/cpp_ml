#ifndef lrgLinearDataCreator_h
#define lrgLinearDataCreator_h
#include "lrgDataCreatorI.h"
#include <memory>
class lrgLinearDataCreator : public lrgDataCreatorI
{
public:
    lrgLinearDataCreator();
    ~lrgLinearDataCreator();
    std::vector<std::pair<double, double>> GetData();
    void GenerateData();

private:
    unsigned int m_size;
    double m_t0;
    double m_t1;
    std::unique_ptr<std::vector<std::pair<double, double>>> m_vec;
};

#endif