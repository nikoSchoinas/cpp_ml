#ifndef lrgFileLoaderDataCreator_h
#define lrgFileLoaderDataCreator_h
#include "lrgDataCreatorI.h"
#include <string>
#include <memory>

// pdd stands for pair of doubles, i.e. pair<double, double>
typedef std::shared_ptr<std::vector<std::pair<double, double>>> shared_ptr_pdd_vector;


class lrgFileLoaderDataCreator : public lrgDataCreatorI
{
private:
    std::string m_filepath;
    shared_ptr_pdd_vector m_vec_ptr;
public:
    lrgFileLoaderDataCreator(std::string&  filepath, shared_ptr_pdd_vector vec_ptr);
    ~lrgFileLoaderDataCreator();
    virtual pdd_vector GetData();
};

#endif