#include <lrgExceptionMacro.h>
#include <iostream>
#include "lrgNormalEquationSolverStrategy.h"
#include "lrgGradientDescentSolverStrategy.h"
#include "lrgFileLoaderDataCreator.h"

// A function that shows how to use the app in the command line.
// Inspiration was taken from the official cplusplus website.
// http://www.cplusplus.com/articles/DEN36Up4/
static void how_to_use(std::string app)
{
    std::cerr << "Usage: " << app << " <option(s)>\n"
              << "Options:\n"
              << "\t-h,--help\t\t\tShows how to set the command line arguments.\n"
              << "\t-f,--file FILE\t\t\tSpecify the absolute path of the input file.\n"
              << "\t-s,--solver SOLVER\t\tSpecify the solver (normal or gradient)\n\n"
              << "\t-e,--eta ETA\t\t\tSpecify the eta parameter for the gradient solver.\n"
              << "\t-i,--iterations ITERATIONS\tSpecify the iterations for the gradient solver.\n\n"
              << "Examples: Inside the build directory run in command line\n"
              << "./bin/lrgFitDataApp -f <filepath> -s normal\n"
              << "./bin/lrgFitDataApp -f <filepath> -s gradient -e 0.1 -i 1000\n"
              << std::endl;
}

int main(int argc, char **argv)
{
    int returnStatus = EXIT_FAILURE;

    // Command line app expects 3 arguments.
    // 1. The app's name.
    // 2. The file path (-f <filepath>)
    // 3. The solver (-s <solver>)
    if (argc < 5)
    {
        how_to_use(argv[0]);
        return 1;
    }

    std::string filepath;
    std::string solver;
    std::string arg;
    double eta = 0;
    unsigned int iterations = 0;    

    // Iterator starts from one to check the options.
    for (size_t i = 1; i < argc; i++)
    {
        
        arg = argv[i];
        // This if-statement runs only if the user gives the right value of arguments and one of them is the -h or --help.
        if ((arg == "-h") || (arg == "--help"))
        {
            how_to_use(argv[0]);
            return 0;
        }
        else if ((arg == "-f") || (arg == "--file"))
        {
            //Check that there is a path after the --file/-f option.
            if (i + 1 < argc)
            {
                // use ++i to take the path instead of the option parameter.
                // Also by increment i we save one iteration.
                filepath = argv[++i];
            }
        }
        else if ((arg == "-s") || (arg == "--solver"))
        {
            //Check that there is a solver after the --solver/-s option.
            if (i + 1 < argc)
            {
                // use ++i to take the solver name instead of the option parameter.
                solver = argv[++i];
                
            }
        }
        else if ((arg == "-e") || (arg == "--eta"))
        {
            //Check that there is an eta value after the --eta/-e option.
            if (i + 1 < argc)
            {
                // use ++i to take the solver name instead of the option parameter.
                // Convert string to double with atof()
                eta = std::atof(argv[++i]);
            }
        }
        else if ((arg == "-i") || (arg == "--iterations"))
        {
            //Check that there is an eta value after the --iteartions/-i option.
            if (i + 1 < argc)
            {
                // use ++i to take the solver name instead of the option parameter.
                // Convert string to int with atoi()
                iterations = std::atoi(argv[++i]);
            }
        }
    }

    //Check if the solver has the right values (gradient or parameter).
    if(! (solver == "normal" || solver == "gradient")){
        std::cerr << "Invalid arguments for --solver." << std::endl;
    }

    try
    {
        // Get the data from the given file and put the inside vector (vec).
        pair_vector_double vec;
        auto vec_ptr = std::make_unique<pair_vector_double>(vec);
        lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));
        vec = data.GetData();

        // use FitData() of lrgNormalEquationSolverStrategy.
        if (solver == "normal")
        {
            // According to Eigen documentation, it's better to use pointers to create objects that use Eigen::Matrices.
            lrgNormalEquationSolverStrategy strategy;
            auto solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
            pair_double thetas = solver->FitData(vec);
            std::cout << "t0: " << thetas.first << ", t1: " << thetas.second << std::endl;
        }
        // use FitData() of lrgGradientDescentSolverStrategy.
        else if (solver == "gradient")
        {
            // According to Eigen documentation, it's better to use pointers to create objects that use Eigen::Matrices.
            lrgGradientDescentSolverStrategy strategy(eta, iterations);
            auto solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
            pair_double thetas = solver->FitData(vec);
            std::cout << "t0: " << thetas.first << ", t1: " << thetas.second << std::endl;
        }
        
        
        returnStatus = EXIT_SUCCESS;
    }
    catch (lrg::Exception &e)
    {
        std::cerr << "Caught lrg::Exception: " << e.GetDescription() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Caught std::exception: " << e.what() << std::endl;
    }

    return returnStatus;
}
