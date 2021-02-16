#include "catch.hpp"
#include "lrgCatchMain.h"
#include "lrgLinearDataCreator.h"
#include "lrgNormalEquationSolverStrategy.h"

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

  for (auto item : vec) // we could also use std::accumulate but we had to provide implementation of the + operator because our data comes in pairs.
  {
    sum += item.first; //first corresponds to x and second to y
  }

  double real_mean = sum / vec.size();
  double approx_mean = (vec.front().first + vec.back().first) / 2;

  // It is rational to think that real and approximate mean values will not be the same.
  // In order to run the test, we set an error rate of 10%.
  // That means that the difference between real_mean and approx_mean will not be higher than 0.1.
  // It may fail sometimes.
  REQUIRE(abs(real_mean - approx_mean) < 0.1);
}

/************************************** BEGINNING OF FitData() TESTING ******************************************************************/
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
  REQUIRE(abs(thetas.first - t0) < 0.3);
  REQUIRE(abs(thetas.second - t1) < 0.3);
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

  REQUIRE(abs(thetas.first - t0) < 0.3);
  REQUIRE(abs(thetas.second - t1) < 0.3);
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

  REQUIRE(abs(thetas.first - t0) < 0.3);
  REQUIRE(abs(thetas.second - t1) < 0.3);
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

  REQUIRE(abs(thetas.first - t0) < 0.3);
  REQUIRE(abs(thetas.second - t1) < 0.3);
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
  REQUIRE(abs(thetas.first - t0) < 0.01);
  REQUIRE(abs(thetas.second - t1) < 0.01);
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
    // If X is zero, y will be always t1.
    vec.push_back(std::make_pair(0, t1));
  }

  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  solver.release();

  // When the whole X vector is zero we ask from Eigen to compute the inverse of a zero matrix.
  // That's impossible and that's why Eigen returns nan or inf.
  REQUIRE((std::isnan(thetas.first) || std::isinf(thetas.first)));
  REQUIRE((std::isnan(thetas.second) || std::isinf(thetas.second)));
}

/************************************** END OF FitData() TESTING ******************************************************************/