#include "lrgNormalEquationSolverStrategy.h"
#include <Eigen/Dense>
#include <unsupported/Eigen/NonLinearOptimization>

// Constructor
lrgNormalEquationSolverStrategy::lrgNormalEquationSolverStrategy() {}

// Destructor
lrgNormalEquationSolverStrategy::~lrgNormalEquationSolverStrategy() {}

// It is more preferable that this method receives a pointer instead of the actual vector.
// However, in the exercise sheet the method's declaration was set like this.
pdd lrgNormalEquationSolverStrategy::FitData(pdd_vector vec)
{
    // We are going to use the size of the vector many times, so we create a variable.
    int vec_size = vec.size();

    // An array that will keep the y-values of the pair vector.
    double array_y[vec_size];

    // An array that will keep the x-values of the pair vector.
    double array_x[vec_size][2];

    for (size_t i = 0; i < vec_size; i++)
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
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> y(array_y, vec_size, 1);

    // Do the same with array_x.
    // RowMajor tells the map function to store the Eigen::Matrix row by row.
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X(array_x[0], vec_size, 2);

    // An Eigen:Matrix to store the result. We know the dimensions in advance. They are always 2x1.
    Eigen::MatrixXd thetas_mat(2, 1);

    // Do the linear algebra according to "Hand-On Machine Learning"
    thetas_mat = (X.transpose() * X).inverse() * X.transpose() * y;

    pdd thetas = std::make_pair(thetas_mat(0), thetas_mat(1));

    // When the whole X vector is zero we ask from Eigen to compute the inverse of a zero matrix.
    // That's impossible and that's why Eigen returns nan or inf.
    // In that case, throw an exception. 
    if (std::isnan(thetas.first) || std::isinf(thetas.first) || std::isnan(thetas.second) || std::isinf(thetas.second))
    {
        throw std::logic_error("Invalid values for thetas...");
    }

    return thetas;
}