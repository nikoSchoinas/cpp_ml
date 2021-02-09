#include "catch.hpp"
#include "lrgCatchMain.h"
#include "lrgLinearDataCreator.h"


TEST_CASE( "lrgLinearDataCreator class instantiation", "[lrgLinearDataCreator]" ) {
  lrgLinearDataCreator d;
} 

TEST_CASE( "lrgLinearDataCreator: number of returned items", "[lrgLinearDataCreator]" ) {
  // Create a vector vec.
  std::vector<std::pair<double, double>> vec;
  
  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<std::vector<std::pair<double, double>>>(vec);
  
  // Other inputs
  double t0 = 4;
  double t1 = 6;
  unsigned int size = 10;

  // Create object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  REQUIRE(vec.size() == size);
}

TEST_CASE( "lrgLinearDataCreator: distribution check", "[lrgLinearDataCreator]" ) {
  // Create a vector vec.
  std::vector<std::pair<double, double>> vec;
  
  // Create a pointer that points to vec.
  auto vec_ptr = std::make_unique<std::vector<std::pair<double, double>>>(vec);
  
  // Other inputs
  double t0 = 4;
  double t1 = 6;
  unsigned int size = 10;

  // Create object.
  lrgLinearDataCreator data(t0, t1, size, std::move(vec_ptr));
  vec = data.GetData();

  // We need to sort the data with respect to x. 
  // In that way, we can access the minimum and the maximum and apply the expression 1/2(min+max).
  // This expression was found in Wikipedia and it gives the mean value of a uniform distribution.

  std::sort(vec.begin(), vec.end());

  // a variable to keep the sum of all x-values inside the vector. 
  double sum = 0; 

  for (auto item : vec) // we could also use std::accumulate but we had to provide implementation of the + operator because our data comes in pairs.
  {
    sum += item.first; //first corresponds to x and second to y
  }

  double real_mean = sum/vec.size();
  double approx_mean = (vec.front().first + vec.back().first)/2;

  // It is rational to think that real and approximate mean values will not be the same. 
  // In order to run the test, we set an error rate of 10%. 
  // That means that the difference between real_mean and approx_mean will not be higher than 0.1.
  // It may fail sometimes. 
  REQUIRE(abs(real_mean - approx_mean) < 0.1);
} 