#include "lrgGradientDescentSolverStrategy.h"
#include <Eigen/Dense>
#include <unsupported/Eigen/NonLinearOptimization>
#include <random>
#include <functional>

// Passing values by reference.
// eta defines how big or small the change in thetas value will be.
// iterations is the number of times that the gradient batch will run. 
lrgGradientDescentSolverStrategy::lrgGradientDescentSolverStrategy(double &eta, unsigned int &iterations)
{
    m_eta = eta;
    m_iterations = iterations;
}

// Empty constructor
lrgGradientDescentSolverStrategy::lrgGradientDescentSolverStrategy()
{
    m_eta = 0;
    m_iterations = 0;
}

// Destructor
lrgGradientDescentSolverStrategy::~lrgGradientDescentSolverStrategy() {}

// Setters
void lrgGradientDescentSolverStrategy::SetEta(double &eta)
{
    m_eta = eta;
}

void lrgGradientDescentSolverStrategy::SetIterations(unsigned int &iterations)
{
    m_iterations = iterations;
}

// A method that puts data from vector inside Eigen::Matrices and perform linear algebra computations.
// returns a pdd, i.e. a pair of doubles pair<double, double>
pdd lrgGradientDescentSolverStrategy::FitData(pdd_vector vec)
{
    // Check if eta and iterations are set. If they are zero we cannot proceed.
    // They can be zero in two cases:
    // a) user create an object with the empty constructor and forgot to set attribute values.
    // b) user didn't call the programme appropriately throught the command line interface.
    if (m_eta == 0 || m_iterations == 0)
    {
        throw std::invalid_argument("Invalid arguments for eta and/or iterations...");
    }

    // This part of the code is the same as the FitData() method in lrgNormalEquationSolverStrategy class.
    //-----------------------------------------------------------------------------------------------------
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
    // RowMajor tells the Eigen::Map function to store the Eigen::Matrix row by row.
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X(array_x[0], vec_size, 2);
    //-----------------------------------------------------------------------------------------------------

    // An array that stores the random values of thetas.
    // We use that array to create the Eigen::Matrix for thetas.
    double array_thetas[2];

    // Normal distribution with mean:0.0 and standard deviation:1.0
    std::normal_distribution<double> distribution(0.0, 1.0);
    std::mt19937_64 mt64;
    auto rand_theta = std::bind(distribution, mt64);

    // Give random initial values to thetas and add them to the array.
    array_thetas[0] = rand_theta();
    array_thetas[1] = rand_theta();

    // Map array_thetas to an Eigen::Matrix (thetas) and use the already allocated memory.
    // thetas_mat is always a 2x1 array.
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> thetas_mat(array_thetas, 2, 1);

    // An Eigen::Matrix to store the gradients.
    // gradients is always a 2x1 array.
    Eigen::MatrixXd gradients(2, 1);

    // Tweak parameters iteratively in order to compute the result.
    // Followed the code example in "Hands-On Machine Learning" p.115 .
    for (size_t i = 0; i < m_iterations; i++)
    {
        gradients = 2.0 / vec_size * X.transpose() * (X * thetas_mat - y);
        thetas_mat = thetas_mat - m_eta * gradients;
    }

    pdd thetas = std::make_pair(thetas_mat(0), thetas_mat(1));
    return thetas;
}