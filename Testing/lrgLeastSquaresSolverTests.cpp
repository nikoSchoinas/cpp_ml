#include "catch.hpp"
#include "lrgCatchMain.h"
#include "lrgLinearDataCreator.h"
#include "lrgNormalEquationSolverStrategy.h"
#include "lrgGradientDescentSolverStrategy.h"
#include "lrgFileLoaderDataCreator.h"

// To check different cases of FitData() (lrgNormalEquationSolverStrategy class) we need to use the same code again and again.
// So, it is better to create a function
pair_double checkFitDataNormal(const double &t0, const double &t1, const unsigned int &size)
{
  // Create a vector vec.
  pair_vector_double vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  // Create object by calling the constructor.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));

  // Put the data inside vec.
  vec = data.GetData();

  // According to Eigen documentation, it's better to use pointers to create objects that use Eigen::Matrices.
  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  // Delete objects.
  solver.release();
  vec_ptr.release();

  return thetas;
}

// We also create a function to check FitData() method of lrgGradientDescentSolverStrategy class.
// We will use different cases by passing different function arguments. 
pair_double checkFitDataGradient(const double &t0, const double &t1, const unsigned int &size, double &eta, unsigned int &iterations)
{
  // Create a vector vec.
  pair_vector_double vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  // Create object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  // According to Eigen documentation, it's better to use pointers to create objects that use Eigen::Matrices.
  lrgGradientDescentSolverStrategy strategy(eta, iterations);
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  // Delete objects.
  solver.release();
  vec_ptr.release();

  return thetas;
}

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

TEST_CASE("lrgLinearDataCreator: negative test, GetData() (empty constructor)", "[lrgLinearDataCreator]")
{
  // Create a vector vec.
  pair_vector_double vec;

  // Create object.
  lrgLinearDataCreator data;

  CHECK_THROWS(vec = data.GetData());

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

  // A variable to keep the sum of all x-values inside the vector.
  double sum = 0;

  for (auto item : vec)
  {
    // item.first corresponds to x value of the pair.
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



/************************************** BEGINNING OF FitData() TESTING () *******************************************/

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), positive thetas, size: 10", "[lrgNormalEquationSolverStrategy]")
{
  // Inputs
  double t0 = 4.3;
  double t1 = 6.7;
  unsigned int size = 10;

  pair_double thetas = checkFitDataNormal(t0, t1, size);
  
  // ********************** NOTE ******************
  // Due to random noise the error is expected to be quite high.
  // The value 0.3 was determined by inspecting the result of the code example in "Hands-On Machine Learning" book.
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), negative thetas, size:20", "[lrgNormalEquationSolverStrategy]")
{

  double t0 = -2.9;
  double t1 = -8.7;
  unsigned int size = 20;

  pair_double thetas = checkFitDataNormal(t0, t1, size);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), negative t0, positive t1, size:30", "[lrgNormalEquationSolverStrategy]")
{

  double t0 = -1.2;
  double t1 = 3;
  unsigned int size = 30;

  pair_double thetas = checkFitDataNormal(t0, t1, size);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), positive t0, negative t1, size:50", "[lrgNormalEquationSolverStrategy]")
{
  double t0 = 5.6;
  double t1 = -3.7;
  unsigned int size = 50;

  pair_double thetas = checkFitDataNormal(t0, t1, size);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), zero thetas, size:100", "[lrgNormalEquationSolverStrategy]")
{

  double t0 = 0;
  double t1 = 0;
  unsigned int size = 100;

  pair_double thetas = checkFitDataNormal(t0, t1, size);

  // lower error
  REQUIRE((abs(thetas.first - t0) < 0.1 && abs(thetas.second - t1) < 0.1));
}

TEST_CASE("lrgNormalEquationSolverStrategy: negative test, check FitData(), zero X, size:15", "[lrgNormalEquationSolverStrategy]")
{

  pair_vector_double vec;

  double t0 = 1.1;
  double t1 = 6.7;
  unsigned int size = 15;

  // Custom-made vector.
  for (size_t i = 0; i < size; i++)
  {
    // We do not include random noise.
    // If X is zero, y will be always t0.
    vec.push_back(std::make_pair(0, t0));
  }

  lrgNormalEquationSolverStrategy strategy;
  auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  
  pair_double thetas;
  
  // When the whole X vector is zero we ask from Eigen to compute the inverse of a zero matrix.
  // That's impossible and that's why Eigen returns nan or inf.
  // In such a case we throw an exception inside the FitData() method, which we can catch with the CHECK_THROWS.
  CHECK_THROWS(thetas = solver->FitData(vec));
  
  solver.release();
}
/************************************** END OF FitData() TESTING ******************************************************************/

/************************************** BEGINNING OF FitData() TESTING (lrgGradientDescentSolverStrategy) *******************************************/

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData() (with setters), positive thetas, size: 10, eta:0.02/0.1/0.5, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  // Create a vector vec.
  pair_vector_double vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  // Other inputs
  double t0 = 4.3;
  double t1 = 6.7;
  unsigned int size = 10;

  // Create object by calling the constructor.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  double eta = 0.02;
  unsigned int iterations = 1000;

  // According to Eigen documentation, it's better to use pointers to create objects that use Eigen::Matrices.
  lrgGradientDescentSolverStrategy strategy(eta, iterations);
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  // ******************** NOTE **********************
  // Due to random noise the error is expected to be quite high.
  // The value 0.3 was determined by inspecting the result of the code example in "Hands-On Machine Learning" book.
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta by calling the setter.
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

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), positive thetas, size: 10, eta:0.02/0.1/0.5, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{
  // A variable to store the results.
  pair_double thetas;

  // Other inputs
  double t0 = 4.3;
  double t1 = 6.7;
  unsigned int size = 10;
  unsigned int iterations = 1000;
  double eta;

  eta = 0.02;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.1;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  eta = 0.5;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), negative thetas, size: 50, eta:0.03/0.05/0.07, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{
  pair_double thetas;

  double t0 = -3.3;
  double t1 = -5.7;
  unsigned int size = 50;
  unsigned int iterations = 1000;
  double eta; 

  eta = 0.03;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  eta = 0.05;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  eta = 0.07;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), negative t0, positive t1, size: 100, eta:0.01/0.12/0.015, iterations:10000", "[lrgGradientDescentSolverStrategy]")
{

  pair_double thetas;

  double t0 = -2.3;
  double t1 = 8.7;
  unsigned int size = 100;
  unsigned int iterations = 10000;
  double eta;


  eta = 0.01;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.012;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  eta = 0.015;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), positive t0, negative t1, size: 100, eta:0.3/0.5, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  pair_double thetas;

  double t0 = 7.3;
  double t1 = -4.7;
  unsigned int size = 100;
  unsigned int iterations = 1000;
  double eta;

  eta = 0.3;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.5;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), zero thetas, size: 100, eta:0.4/0.5, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  pair_double thetas;

  double t0 = 0.0;
  double t1 = 0.0;
  unsigned int size = 100;
  unsigned int iterations = 1000;
  double eta;

  eta = 0.4;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

  // Try different eta.
  eta = 0.5;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));

}


TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), zero X, size: 100, eta:0.1, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  pair_vector_double vec;

  double t0 = 1.1;
  double t1 = 6.7;
  unsigned int size = 15;
  double eta = 0.1;
  unsigned int iterations = 1000;

  // Custom-made vector.
  for (size_t i = 0; i < size; i++)
  {
    // We do not include random noise.
    // If X is zero, y will be always t0.
    vec.push_back(std::make_pair(0, t0));
  }

  // Use the empty constructor.
  lrgGradientDescentSolverStrategy strategy(eta, iterations);
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pair_double thetas = solver->FitData(vec);

  //If all elements of X are zero then the Gradient Batch can predict just the t0 value.
  //This is happening because of the matrix multiplication between X and thetas_mat (see lrgGradientDescentSolverStrategy.cpp)
  //A change is happening only to the first element of the gradient matrix (t0). e.g.
  
  //   X    thetas     gradient
  // ----------------------------
  // | 1 0 | | t0 | --> | t0 |
  // | 1 0 | | t1 | --> | t0 | 
  //  .                 .
  //  .                 .    
  //  .                 .

  REQUIRE(abs(thetas.first - t0) < 0.3);

  // Delete objects.
  solver.release();
} 

TEST_CASE("lrgGradientDescentSolverStrategy: negative test, check FitData() empty constructor", "[lrgGradientDescentSolverStrategy]")
{

  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);

  double t0 = 2.1;
  double t1 = 4.7;
  unsigned int size = 15;
  
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  // We use the empty constructor.
  lrgGradientDescentSolverStrategy strategy;
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);

  // When using the empty constructor we do not set the attributes (eta and iterations)
  //In such a case FitData() throws an error because it cannot procceed to the computation.
  pair_double thetas; 
  CHECK_THROWS(thetas = solver->FitData(vec));

  // Delete objects.
  solver.release();
  vec_ptr.release();

}

TEST_CASE("lrgFileLoaderDataCreator: check GetData() TestData1.txt", "[lrgFileLoaderDataCreator]")
{
  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);
  
  // ************* WARNING ***************
  // We placed the files inside /Testing/TestFiles directory.
  // We assume that the executable will be run inside the bin directory (./lgrLeastSquaresSolverTests)
  // In that way ctest run smoothly. 
  // If you want to test another file you need to provide the absolute path.
  std::string filepath = "../../Testing/TestFiles/TestData1.txt";
  lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));
  vec = data.GetData();



  // Check the number of rows, the first and the last element.
  REQUIRE(

      (
          vec.size() == 1000 &&
          vec.front().first == 0.170065 && vec.front().second == 3.38151 &&
          vec.back().first == 1.04707 && vec.back().second == 5.42941

          )

  );
}

// Same test as before but for TestData2.txt.
TEST_CASE("lrgFileLoaderDataCreator: check GetData() TestData2.txt", "[lrgFileLoaderDataCreator]")
{
  
  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);
  
  std::string filepath = "../../Testing/TestFiles/TestData2.txt";
  lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));
  vec = data.GetData();


  REQUIRE(

      (
          vec.size() == 1000 &&
          vec.front().first == 0.170065 && vec.front().second == 2.55157 &&
          vec.back().first == 1.04707 && vec.back().second == 5.47648

          )

  );

}

TEST_CASE("lrgFileLoaderDataCreator: negative testm check GetData() (wrong path)", "[lrgFileLoaderDataCreator]")
{
  pair_vector_double vec;
  auto vec_ptr = std::make_unique<pair_vector_double>(vec);
  
  // We give a path that doen't exist. 
  std::string filepath = "../Testing/TestFiles/NOT_EXISTING_FILE.txt";
  lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));

  // In the case that the file doesn't exist or the path is wrong GetData() throws an error.
  CHECK_THROWS(vec = data.GetData());

} 