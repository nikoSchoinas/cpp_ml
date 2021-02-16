#include "lrgNormalEquationSolverStrategy.h"
#include <array>
#include <Eigen/Dense>
#include <unsupported/Eigen/NonLinearOptimization>


// It is more preferable that this method receives a pointer instead of the actual vector.
// However, in the exercise sheet the method's declaration was set like this. 
pair_double lrgNormalEquationSolverStrategy::FitData(pair_vector_double vec)
{
    // We are going to use the size of the vector many times, so we create a variable.
    int size = vec.size();

    // An array that will keep the y-values of the pair vector.
    double array_y[size];
    // An array that will keep the x-values of the pair vector.
    double array_x[size][2];

    for (size_t i = 0; i < size; i++)
    {
        // According to "Hands-On Machine Learning", the first column of X-matrix must have ones.
        // Also X-matrix must be 2D to do valid matrix multiplications.
        array_x[i][0] = 1;
        // Fill the second column of array_x with the x-values of the pair vector (vec).
        array_x[i][1] = vec[i].first;
        // Fill the array_y.
        array_y[i] = vec[i].second;
    }


    // Create an Eigen::Matrix (y) by using the allocated memory that array_y had.
    Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> > y(array_y,size,1);
    // Do the same with arra_x. ;
    // RowMajor tells the map function to store the eigen::matrix row by row.
    Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> > X(array_x[0],size,2);

    // An Eigen:matrix to store the result. We know the dimentions in advance. They are always 2x1.
    Eigen::MatrixXd thetas_mat(2,1);
    thetas_mat = (X.transpose() * X).inverse() * X.transpose() * y;

    pair_double thetas = std::make_pair(thetas_mat(0), thetas_mat(1));
    return thetas;

}

lrgNormalEquationSolverStrategy::lrgNormalEquationSolverStrategy(/* args */) {
    
}

lrgNormalEquationSolverStrategy::~lrgNormalEquationSolverStrategy() {}