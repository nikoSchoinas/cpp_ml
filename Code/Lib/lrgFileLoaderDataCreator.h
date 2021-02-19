#ifndef lrgFileLoaderDataCreator_h
#define lrgFileLoaderDataCreator_h
#include "lrgDataCreatorI.h"
#include <string>
#include <memory>

typedef std::unique_ptr<std::vector<std::pair<double, double>>> pair_vector_double_ptr;


class lrgFileLoaderDataCreator : public lrgDataCreatorI
{
private:
    std::string m_filepath;
    pair_vector_double_ptr m_vec;
public:
    lrgFileLoaderDataCreator(std::string&  filepath, pair_vector_double_ptr vec);
    ~lrgFileLoaderDataCreator();
    virtual pair_vector_double GetData();
};

#endif