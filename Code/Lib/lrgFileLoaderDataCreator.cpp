#include "lrgFileLoaderDataCreator.h"
#include <fstream>
#include <iostream>

lrgFileLoaderDataCreator::lrgFileLoaderDataCreator(std::string&  filepath, pair_vector_double_ptr vec) : m_filepath(filepath)
{
    m_vec = std::move(vec);
}

lrgFileLoaderDataCreator::~lrgFileLoaderDataCreator()
{
}

pair_vector_double lrgFileLoaderDataCreator::GetData(){

    // We do not need try-catch block because C++ iostreams do not throw exceptions. 
    std::ifstream m_file(m_filepath);

    double x;
    double y;

    while (m_file >> x >> y)
    {
        m_vec->push_back(std::make_pair(x, y));
    }

    return (*m_vec);
}