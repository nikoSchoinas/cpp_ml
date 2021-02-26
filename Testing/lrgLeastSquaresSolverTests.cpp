#include "catch.hpp"
#include "lrgCatchMain.h"
#include "lrgLinearDataCreator.h"
#include "lrgNormalEquationSolverStrategy.h"
#include "lrgGradientDescentSolverStrategy.h"
#include "lrgFileLoaderDataCreator.h"

// To check different cases of FitData() (lrgNormalEquationSolverStrategy class) we need to use the same code again and again.
// So, it is better to create a function
pdd checkFitDataNormal(const double &t0, const double &t1, const unsigned int &size)
{
  // data is an object of type lrgLinearDataCreator and has a shared_ptr as one of its attributes.
  // data_ptr is another shared_ptr that points to data. 
  // We had to use shared pointers to achieve this implementation.
  // vec_ptr is also a shared_ptr but it could be a unique_ptr, too.
  // Shared pointers go as follow:  [the arrow (-->) means a "points to" relationship]
  // Initially, vec_ptr --> vec
  // then, data.m_vec_ptr --> vec
  // then, data_ptr --> data and abstractly we could say that data_ptr --> vec
  // vec_ptr seems to be useles, but actually creates the ownership between a pointer and the vector (vec)
  // Next, that ownership is passed to the data object. 

  // Create a vector vec.
  pdd_vector vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_shared<pdd_vector>(vec);

  // Create an object that randomly generates the data by calling the constructor.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));

  // Create a shared pointer that points to data object.
  std::shared_ptr<lrgDataCreatorI> data_ptr = std::make_shared<lrgLinearDataCreator>(data);

  // Put the data inside vec.
  vec = data_ptr->GetData();

  // Do the same with the solver.
  // Create an object and then a pointer that points to that object.
  lrgNormalEquationSolverStrategy strategy;
  std::unique_ptr<lrgLinearModelSolverStrategyI> solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  pdd thetas = solver->FitData(vec);

  // Delete objects/release pointers.
  solver.release();

  return thetas;
}

// We also create a function to check FitData() method of lrgGradientDescentSolverStrategy class.
// We will use different cases by passing different function arguments. 
pdd checkFitDataGradient(const double &t0, const double &t1, const unsigned int &size, double &eta, unsigned int &iterations)
{
  // Create a vector vec.
  pdd_vector vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_shared<pdd_vector>(vec);

  // Create an object that randomly generates the data by calling the constructor.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));

  // We use shared pointers as follows:
  // data_ptr --> data <-- vec_ptr

  // Create a shared pointer that points to data object.
  std::shared_ptr<lrgDataCreatorI> data_ptr = std::make_shared<lrgLinearDataCreator>(data);

  // Put the data inside vec.
  vec = data_ptr->GetData();


  // According to Eigen documentation, it's better to use pointers to create objects that use Eigen::Matrices.
  lrgGradientDescentSolverStrategy strategy(eta, iterations);
  std::unique_ptr<lrgLinearModelSolverStrategyI> solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pdd thetas = solver->FitData(vec);

  // Delete objects/release pointers.
  solver.release();

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
  pdd_vector vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_shared<pdd_vector>(vec);

  // Other inputs
  double t0 = 4;
  double t1 = 6;
  unsigned int size = 10;

  // Create a pointer that points to data object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  std::shared_ptr<lrgDataCreatorI> data_ptr = std::make_shared<lrgLinearDataCreator>(data);
  vec = data_ptr->GetData();

  REQUIRE(vec.size() == size);
}

TEST_CASE("lrgLinearDataCreator: distribution check", "[lrgLinearDataCreator]")
{
  // Create a vector vec.
  pdd_vector vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_shared<pdd_vector>(vec);

  // Other inputs
  double t0 = 4;
  double t1 = 6;
  unsigned int size = 10;

  // Create a pointer that points to data object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  std::shared_ptr<lrgDataCreatorI> data_ptr = std::make_shared<lrgLinearDataCreator>(data);
  vec = data_ptr->GetData();

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

  // Compute the real mean value.
  double real_mean = sum / vec.size();

  // Compute an approximation of mean values according to the expression 1/2*(min+max).
  double approx_mean = (vec.front().first + vec.back().first) / 2;

  // It is rational to think that real and approximate mean values will not be the same.
  // In order to run the test, we set an error rate.
  // That means that the difference between real_mean and approx_mean will not be higher than 0.1.
  // It may fail sometimes.
  REQUIRE(abs(real_mean - approx_mean) < 0.1);
}

TEST_CASE("lrgLinearDataCreator: negative test, GetData() (empty constructor)", "[lrgLinearDataCreator]")
{
  // Create a vector vec.
  pdd_vector vec;

  // Create object.
  lrgLinearDataCreator data;

  // When using the empty constructor for lrgLinearDataCreator, then all the attributes are set to zero.
  // In that case the GetData() method cannot compute random pairs and throws an error.
  CHECK_THROWS(vec = data.GetData());

}

TEST_CASE("lrgLinearDataCreator: negative test, GetData() (zero vector size)", "[lrgLinearDataCreator]")
{
  pdd_vector vec;
  auto vec_ptr = std::make_shared<pdd_vector>(vec);

  double t0 = 4;
  double t1 = 6;

  // This is the line that causes the exception
  unsigned int size = 0;

  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  std::shared_ptr<lrgDataCreatorI> data_ptr = std::make_shared<lrgLinearDataCreator>(data);

  // GetData() method throws an error if the vector size is set to zero.
  CHECK_THROWS(vec = data_ptr->GetData());

}

/************************************** BEGINNING OF FitData() TESTING () *******************************************/
// We test different values of thetas with different vector sizes.

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), positive thetas, size: 10", "[lrgNormalEquationSolverStrategy]")
{
  // Inputs
  double t0 = 4.3;
  double t1 = 6.7;
  unsigned int size = 10;

  pdd thetas = checkFitDataNormal(t0, t1, size);
  
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

  pdd thetas = checkFitDataNormal(t0, t1, size);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), negative t0, positive t1, size:30", "[lrgNormalEquationSolverStrategy]")
{

  double t0 = -1.2;
  double t1 = 3;
  unsigned int size = 30;

  pdd thetas = checkFitDataNormal(t0, t1, size);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), positive t0, negative t1, size:50", "[lrgNormalEquationSolverStrategy]")
{
  double t0 = 5.6;
  double t1 = -3.7;
  unsigned int size = 50;

  pdd thetas = checkFitDataNormal(t0, t1, size);

  REQUIRE((abs(thetas.first - t0) < 0.3 && abs(thetas.second - t1) < 0.3));
}

TEST_CASE("lrgNormalEquationSolverStrategy: check FitData(), zero thetas, size:100", "[lrgNormalEquationSolverStrategy]")
{

  double t0 = 0;
  double t1 = 0;
  unsigned int size = 100;

  pdd thetas = checkFitDataNormal(t0, t1, size);

  // Result is expected to be very close to zero so we can reduce the error to 0.1 .
  REQUIRE((abs(thetas.first - t0) < 0.1 && abs(thetas.second - t1) < 0.1));
}

TEST_CASE("lrgNormalEquationSolverStrategy: negative test, check FitData(), zero X, size:15", "[lrgNormalEquationSolverStrategy]")
{

  pdd_vector vec;

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
  std::unique_ptr<lrgLinearModelSolverStrategyI> solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
  
  pdd thetas;
  
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
  pdd_vector vec;

  // Create a pointer that points to vec.
  auto vec_ptr = std::make_shared<pdd_vector>(vec);

  // Other inputs
  double t0 = 4.3;
  double t1 = 6.7;
  unsigned int size = 10;

  // Create object by calling the constructor.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  std::shared_ptr<lrgDataCreatorI> data_ptr = std::make_shared<lrgLinearDataCreator>(data);
  vec = data_ptr->GetData();

  double eta = 0.02;
  unsigned int iterations = 1000;

  // Create an oject.
  lrgGradientDescentSolverStrategy strategy(eta, iterations);

  // In that case, solver is of type lrgGradientDescentSolverStrategy and not lrgLinearModelSolverStrategyI.
  // We need to use setters and lrgLinearModelSolverStrategyI has no such methods.
  auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pdd thetas = solver->FitData(vec);

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

  // Delete objects/release pointers.
  solver.release();
}

TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), positive thetas, size: 10, eta:0.02/0.1/0.5, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{
  // A variable to store the results.
  pdd thetas;

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
  pdd thetas;

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

  pdd thetas;

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

  pdd thetas;

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

  pdd thetas;

  double t0 = 0.0;
  double t1 = 0.0;
  unsigned int size = 100;
  unsigned int iterations = 1000;
  double eta;

  eta = 0.4;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.1 && abs(thetas.second - t1) < 0.1));

  // Try different eta.
  eta = 0.5;
  thetas = checkFitDataGradient(t0,t1,size,eta,iterations);
  REQUIRE((abs(thetas.first - t0) < 0.1 && abs(thetas.second - t1) < 0.1));

}


TEST_CASE("lrgGradientDescentSolverStrategy: check FitData(), zero X, size: 100, eta:0.1, iterations:1000", "[lrgGradientDescentSolverStrategy]")
{

  pdd_vector vec;

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
  std::unique_ptr<lrgLinearModelSolverStrategyI> solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
  pdd thetas = solver->FitData(vec);

  // If all elements of X are zero then the Gradient Batch can predict just the t0 value.
  // This is happening because of the matrix multiplication between X and thetas_mat (see lrgGradientDescentSolverStrategy.cpp)
  // A change is happening only to the first element of the gradient matrix (t0). e.g.
  
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

  pdd_vector vec;
  auto vec_ptr = std::make_shared<pdd_vector>(vec);

  double t0 = 2.1;
  double t1 = 4.7;
  unsigned int size = 15;
  
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  // We use the empty constructor.
  lrgGradientDescentSolverStrategy strategy;
  std::unique_ptr<lrgLinearModelSolverStrategyI> solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);

  // When using the empty constructor we do not set the attributes (eta and iterations)
  // In that case FitData() throws an error because it cannot proceed to the computation.
  pdd thetas; 
  CHECK_THROWS(thetas = solver->FitData(vec));

  // Delete objects.
  solver.release();

}

TEST_CASE("lrgFileLoaderDataCreator: check GetData() TestData1.txt", "[lrgFileLoaderDataCreator]")
{
  pdd_vector vec;
  auto vec_ptr = std::make_shared<pdd_vector>(vec);
  
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
  
  pdd_vector vec;
  auto vec_ptr = std::make_shared<pdd_vector>(vec);
  
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

TEST_CASE("lrgFileLoaderDataCreator: negative test check GetData() (wrong path)", "[lrgFileLoaderDataCreator]")
{
  pdd_vector vec;
  auto vec_ptr = std::make_shared<pdd_vector>(vec);
  
  // We give a path that doesn't exist. 
  std::string filepath = "../Testing/TestFiles/NOT_EXISTING_FILE.txt";
  lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));

  // In the case that the file doesn't exist or the path is wrong GetData() throws an error.
  CHECK_THROWS(vec = data.GetData());

} 

TEST_CASE("lrgFileLoaderDataCreator: negative test check GetData() (empty file)", "[lrgFileLoaderDataCreator]")
{
  pdd_vector vec;
  auto vec_ptr = std::make_shared<pdd_vector>(vec);
  
  // Path to an empty file. 
  std::string filepath = "../Testing/TestFiles/TestData0.txt.txt";
  lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));

  // In the case of an empty file, GetData() throws an error.
  CHECK_THROWS(vec = data.GetData());

} 