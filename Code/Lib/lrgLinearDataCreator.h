#ifndef lrgLinearDataCreator_h
#define lrgLinearDataCreator_h
#include "lrgDataCreatorI.h"

class lrgLinearDataCreator : public lrgDataCreatorI
{
public:
    lrgLinearDataCreator(/* args */);
    ~lrgLinearDataCreator();
    std::vector<std::pair<double, double> > GetData();
};




#endif