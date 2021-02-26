# Linear Regression with C++

This programme was developed as part of the assignment in **Research Computing with C++** course at **UCL**. The aim is to write a piece of software than performs linear regression using **Eigen** library. There are many different ways to accomplish linear regression but this project uses the ‘normal equation’ and the ‘gradient descent’. Information about the solvers and code examples can be found in **Hands-On Machine Learning with Scikit-Learn & TensorFlow** by Aurelien Geron. 

# Build the project

We use **CMake** for building. To build and run the programme you need to navigate to **PHAS0100Assignment1** directory and create a "build" folder. If the "build" folder exists you can skip this section or you can delete the folder and proceed as follows:
```sh
~/PHAS0100Assignment1$ mkdir build
```
Then, get inside the build directory 
```sh
~/PHAS0100Assignment1$ cd build
```
and run
```sh
~/PHAS0100Assignment1/build$ cmake ..
```
and
```sh
~/PHAS0100Assignment1/build$ make
```

Wait a couple of seconds... and Voila! The programme is ready to run. 

# Running the programme

You can interact with the program through the command line interface by setting the appropriate arguments. When typing the --help or -h tag, you get useful information about the arguments and how to use them.
```sh
~/PHAS0100Assignment1/build$./bin/lrgFitDataApp --help
```

### Normal Equation
In the case of the normal equation, you just need to specify the **path** to the input file and the **solver** (normal).
```sh
~/PHAS0100Assignment1/build$./bin/lrgFitDataApp --file ../Testing/TestFiles/TestData1.txt --solver normal 
```
Along with the code there are some testing files inside PHAS0100Assignment1/Testing/TestFiles directory

### Gradient descent
There are three types of gradient descent:
* Batch gradient descent
* Stochastic gradient descent
* Mini-batch gradient descent

The software implements the first one. In that case, the user should specify two more arguments, **eta** and **iterations**. The idea behind gradient descent is to change the value parameters iteratively in order to compute the result. Eta specifies how big or small this change would be, while iterations specify the number of sequential runs.
```sh
~/PHAS0100Assignment1/build$./bin/lrgFitDataApp --file ../Testing/TestFiles/TestData1.txt --solver gradient --eta 0.1 --iterations 1000 
```
In this example, the values of eta and iterations are indicative. You can try different values depending on your needs. 

# File Format
Input files should have a very specific format. In that way, it is guaranteed that the programme will run without errors. Every file should have two values per line space-separated (X y). E.g.
```
0.170065 3.38151
0.796017 4.55925
1.48702 6.23399
```
**IMPORTANT**
Files should **NOT** have empty lines at the beginning or at the end.
Check the test files inside PHAS0100Assignment1/Testing/TestFiles directory and try to follow the same format, but under no circumstances add extra lines or other string values.
Note: The TestData0.txt is an empty file created for testing purposes.

Inside the directory there is a screenshot.png file that shows how the command line arguments should be used in order to test the TestData1.txt and TestData2.txt files. 
Normally, TestData1.txt should give t0 = 3 and t1 = 2 and TestData2.txt should give t0 = 2 and t1 = 3. 
Note: The programme may not be 100% accurate and the results may be affected by minor errors. 

# Testing
All the methods/functions used in this project were tested for a range of representative cases and edge cases with positive and negative testing. Testing code can be found inside Testing/lrgLeastSquaresTests.cpp. Tests can be run with **ctest** from the inside of the "build" folder.
```sh
~/PHAS0100Assignment1/build$ ctest 
```

License
----

BSD-3-Clause License 
