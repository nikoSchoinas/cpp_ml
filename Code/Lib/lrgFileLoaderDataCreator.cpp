#include "lrgFileLoaderDataCreator.h"
#include <fstream>

lrgFileLoaderDataCreator::lrgFileLoaderDataCreator(std::string&  filepath, pair_vector_double_ptr vec) : m_filepath(filepath)
{
    m_vec = std::move(vec);
}

lrgFileLoaderDataCreator::~lrgFileLoaderDataCreator()
{
}

pair_vector_double lrgFileLoaderDataCreator::GetData(){

    // We do not need try-catch block because C++ iostreams do not throw exceptions. 
    std::ifstream m_file;
    m_file.open(m_filepath, std::ios::in);
    
    // We manually throw an exception if the file doesn't exist. 
    if (! m_file)
    {
        throw std::ios_base::failure("Reading file failed...");
    }
    

    double x;
    double y;

    while (m_file >> x >> y)
    {
        m_vec->push_back(std::make_pair(x, y));
    }

    if (! m_vec->size() > 0)
    {
        throw std::length_error("Vector is empty. Something went wrong when reading the input file...");
    }
    

    return (*m_vec);
}