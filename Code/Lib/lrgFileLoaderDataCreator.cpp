#include "lrgFileLoaderDataCreator.h"
#include <fstream>

// Constructor follows RAII pattern. 
// filepath is the path of the file that contains the data.
// vec is a unique pointer that points to the vector that will be filled with the data.
lrgFileLoaderDataCreator::lrgFileLoaderDataCreator(std::string&  filepath, shared_ptr_pdd_vector vec_ptr) : m_filepath(filepath)
{
    m_vec_ptr = std::move(vec_ptr);
}

// An empty constructor made for testing purposes.
// It cannot be used in the main app because there are no setters.
lrgFileLoaderDataCreator::~lrgFileLoaderDataCreator()
{
}

// A method that copies X an y values from file and place them inside a vector.
pdd_vector lrgFileLoaderDataCreator::GetData(){

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

    // Copy values from file to vector.
    while (m_file >> x >> y)
    {
        m_vec_ptr->push_back(std::make_pair(x, y));
    }

    // Add a second check in case something went wrong while reading the file.
    // If the reading failed then the vector should be empty.
    if (! m_vec_ptr->size() > 0)
    {
        throw std::length_error("Vector is empty. Something went wrong when reading the input file...");
    }
    
    m_file.close();

    return (*m_vec_ptr);
}