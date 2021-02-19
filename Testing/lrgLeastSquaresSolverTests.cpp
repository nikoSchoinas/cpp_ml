#include "catch.hpp"
#include "lrgCatchMain.h"
#include "lrgLinearDataCreator.h"
#include "lrgNormalEquationSolverStrategy.h"
#include "lrgGradientDescentSolverStrategy.h"
#include "lrgFileLoaderDataCreator.h"

TEST_CASE("lrgLinearDataCreator,lrgNormalEquationSolverStrategy class instantiation", "[lrg]")
{
  lrgLinearDataCreator d;
  lrgNormalEquationSolverStrategy s;
}

TEST_CASE("lrgLinearDataCreator: number of returned items", "[lrgLinearDataCreator]")
{
  // Create a vector vec.
  pair_vector_double vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  // Other inputs
  double t0 = 4;
  double t1 = 6;
  unsigned int size = 10;

  // Create object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  REQUIRE(vec.size() == size);
}

TEST_CASE("lrgLinearDataCreator: distribution check", "[lrgLinearDataCreator]")
{
  // Create a vector vec.
  pair_vector_double vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  // Other inputs
  double t0 = 4;
  double t1 = 6;
  unsigned int size = 10;

  // Create object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  // We need to sort the data with respect to x.
  // In that way, we can access the minimum and the maximum and apply the expression 1/2*(min+max).
  // This expression was found in Wikipedia and it gives the mean value of a uniform distribution.

  std::sort(vec.begin(), vec.end());

  // a variable to keep the sum of all x-values inside the vector.
  double sum = 0;

  for (auto item : vec)
  {
    //first corresponds to x value of the pair.
    sum += item.first;
  }

  double real_mean = sum / vec.size();
  double approx_mean = (vec.front().first + vec.back().first) / 2;

  // It is rational to think that real and approximate mean values will not be the same.
  // In order to run the test, we set an error rate of 10%.
  // That means that the difference between real_mean and approx_mean will not be higher than 0.1.
  // It may fail sometimes.
  REQUIRE(abs(real_mean - approx_mean) < 0.1);
}

/************************************** BEGINNING OF FitData() TESTING (lrgNormalEquationSolverStrategy) *******************************************/
/* The first test has comments. The rest are the same with different parameters. */

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), positive thetas, size: 10", "[lrgNormalEquationSolverStrategy]")
{

  // Create a vector vec.
  pair_vector_double vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  // Other inputs
  double t0 = 4.3;
  double t1 = 6.7;
  unsigned int size = 10;

  // Create object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  // According to Eigen documentation, it's better to use pointers to create objects that use Eigen::Matrices.
  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  // Delete objects.
  solver.release();
  vec_ptr.release();

  // Due to random noise the error is expected to be quite high.
  // The value 0.3 was determined by inspecting the result of the code example in "Hands-On Machine Learning" book.
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), negative thetas, size:20", "[lrgNormalEquationSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = -2.9;
  double t1 = -8.7;
  unsigned int size = 20;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  solver.release();
  vec_ptr.release();

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), negative t0, positive t1, size:30", "[lrgNormalEquationSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = -1.2;
  double t1 = 3;
  unsigned int size = 30;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  solver.release();
  vec_ptr.release();

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), positive t0, negative t1, size:50", "[lrgNormalEquationSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = 5.6;
  double t1 = -3.7;
  unsigned int size = 50;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  solver.release();
  vec_ptr.release();

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), zero thetas, size:100", "[lrgNormalEquationSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = 0;
  double t1 = 0;
  unsigned int size = 100;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  solver.release();
  vec_ptr.release();

  // lower error
  REQUIRE((abs(thetas.first - t0) < 0.1 && abs(thetas.second - t1) < 0.1));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), zero X, size:15", "[lrgNormalEquationSolverStrategy]")
{

  pair_vector_double vec;

  double t0 = 1.1;
  double t1 = 6.7;
  unsigned int size = 15;

  // Custom made vector.
  for (size_t i = 0; i < size; i++)
  {
    // We do not include random noise.
    // If X is zero, y will be always t0.
    vec.push_back(std::make_pair(0, t0));
  }

  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  solver.release();

  // When the whole X vector is zero we ask from Eigen to compute the inverse of a zero matrix.
  // That's impossible and that's why Eigen returns nan or inf.
  REQUIRE(((std::isnan(thetas.first) || std::isinf(thetas.first)) && (std::isnan(thetas.second) || std::isinf(thetas.second))));
}
/************************************** END OF FitData() TESTING ******************************************************************/

/************************************** BEGINNING OF FitData() TESTING (lrgGradientDescentSolverStrategy) *******************************************/
/* The first test has comments. The rest are the same with different parameters. */

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), positive thetas, size: 10, eta:0.02/0.1/0.5, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  // Create a vector vec.
  pair_vector_double vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  // Other inputs
  double t0 = 4.3;
  double t1 = 6.7;
  unsigned int size = 10;

  // Create object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  double eta = 0.02;
  unsigned int iterations = 1000;

  // According to Eigen documentation, it's better to use pointers to create objects that use Eigen::Matrices.
  lrgGradientDescentSolverStrategy strategy(eta, iterations);
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  // Due to random noise the error is expected to be quite high.
  // The value 0.3 was determined by inspecting the result of the code example in "Hands-On Machine Learning" book.
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.1;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  eta = 0.5;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Delete objects.
  solver.release();
  vec_ptr.release();
}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), negative thetas, size: 50, eta:0.03/0.05/0.07, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = -3.3;
  double t1 = -5.7;
  unsigned int size = 50;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  double eta = 0.03;
  unsigned int iterations = 1000;

  lrgGradientDescentSolverStrategy strategy;
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);

  // Use setters with pointers.
  solver->SetEta(eta);
  solver->SetIterations(iterations);
  pair_double thetas = solver->FitData(vec);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.05;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  eta = 0.07;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Delete objects.
  solver.release();
  vec_ptr.release();
}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), negative t0, positive t1, size: 100, eta:0.01/0.12/0.015, iterations:10000", "[lrgGradientDescentSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = -2.3;
  double t1 = 8.7;
  unsigned int size = 100;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  double eta = 0.01;
  unsigned int iterations = 10000;

  lrgGradientDescentSolverStrategy strategy;
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);

  // Use setters with pointers.
  solver->SetEta(eta);
  solver->SetIterations(iterations);
  pair_double thetas = solver->FitData(vec);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.012;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  eta = 0.015;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Delete objects.
  solver.release();
  vec_ptr.release();
}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), positive t0, negative t1, size: 100, eta:0.3/0.5/0.7, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = 7.3;
  double t1 = -4.7;
  unsigned int size = 100;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  double eta = 0.3;
  unsigned int iterations = 1000;

  lrgGradientDescentSolverStrategy strategy;
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);

  // Use setters with pointers.
  solver->SetEta(eta);
  solver->SetIterations(iterations);
  pair_double thetas = solver->FitData(vec);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.5;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  eta = 0.07;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Delete objects.
  solver.release();
  vec_ptr.release();
}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), zero thetas, size: 100, eta:0.4/0.5, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = 0.0;
  double t1 = 0.0;
  unsigned int size = 100;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  double eta = 0.4;
  unsigned int iterations = 1000;

  // Use the constructor to set the attributes.
  lrgGradientDescentSolverStrategy strategy(eta, iterations);
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.5;
  solver->SetEta(eta);
  thetas = solver->FitData(vec);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
  ;

  // Delete objects.
  solver.release();
  vec_ptr.release();
}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), zero X, size: 100, eta:0.1, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  pair_vector_double vec;

  double t0 = 1.1;
  double t1 = 6.7;
  unsigned int size = 15;

  // Custom made vector.
  for (size_t i = 0; i < size; i++)
  {
    // We do not include random noise.
    // If X is zero, y will be always t0.
    vec.push_back(std::make_pair(0, t0));
  }

  // Use the empty constructor.
  lrgGradientDescentSolverStrategy strategy;
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  /*  If all elements of X are zero then the Gradient Batch can predict just the t0 value.
 *  This is happening because of the matrix multiplication between X and thetas_mat (see lrgGradientDescentSolverStrategy.cpp)
 *  A change is happening only to the first element of the gradient matrix (t0). e.g.
 * 
 *   X    thetas     gradient
 *----------------------------
 * | 1 0 | | t0 | --> | t0 |
 * | 1 0 | | t1 | --> | t0 | 
 *    .                 .
 *    .                 .    
 *    .                 .
 */

  REQUIRE(abs(thetas.first - t0) < 0.3);

  // Delete objects.
  solver.release();
}

TEST_CASE("lrgFileLoaderDataCreator: check GetData() TestData1.txt", "[lrgFileLoaderDataCreator]")
{
  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  // ************* WARNING ***************
  // We placed the TestFiles inside the build directory.
  // If you want to test another file you need to provide the absolute path.
  std::string filepath = "TestData1.txt";
  lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));
  vec = data.GetData();

  // In that case we know that the vector vec isn't empty and the program probably won't crash.
  if (vec.size() > 0)
  {
    // Check the number of rows, the first and the last element.
    REQUIRE(

        (
            vec.size() == 1000 &&
            vec.front().first == 0.170065 && vec.front().second == 3.38151 &&
            vec.back().first == 1.04707 && vec.back().second == 5.42941

            )

    );
  }
}

TEST_CASE("lrgFileLoaderDataCreator: check GetData() TestData2.txt", "[lrgFileLoaderDataCreator]")
{
  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);
  

  // ************* WARNING ***************
  // We placed the TestFiles inside the build directory.
  // If you want to test another file you need to provide the absolute path.
  std::string filepath = "TestData2.txt";
  lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));
  vec = data.GetData();

  // In that case we know that the vector vec isn't empty and the program probably won't crash.
  if (vec.size() > 0)
  {
    // Check the number of rows, the first and the last element.
    REQUIRE(

        (
            vec.size() == 1000 &&
            vec.front().first == 0.170065 && vec.front().second == 2.55157 &&
            vec.back().first == 1.04707 && vec.back().second == 5.47648

            )

    );
  }
}