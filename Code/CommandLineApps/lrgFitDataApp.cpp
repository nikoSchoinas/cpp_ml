#include <lrgExceptionMacro.h>
#include <iostream>
#include <fstream>
#include "lrgNormalEquationSolverStrategy.h"
#include "lrgGradientDescentSolverStrategy.h"
#include "lrgFileLoaderDataCreator.h"

// A function that shows how to use the app in the command line.
// Inspiration was taken from the official cplusplus website.
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

    // Command line app expects 2 or 5 or 9 arguments.

    // Case of 2 arguments:
    //---------------------
    // 1. The app's name.
    // 2. The --help/-h tag.

    // Case of 5 arguments:
    //----------------------
    // 1. The app's name.
    // 2. The --file/-f tag.
    // 3. The actual file path (<filepath>)
    // 4. The --solver/-s tag.
    // 5. The solver keyword (gradient or normal).

    //Case of 9 arguments:
    //---------------------
    // 1-5: see above
    // 6. The --eta/-e tag.
    // 7. The value of eta (<eta>)
    // 8. The --iterations/-i tag.
    // 9. The number of iterations (<iterations>) 

    // In any other case respond with the help message.

    if (argc != 2 && argc != 5 && argc != 9)
    {
        how_to_use(argv[0]);
        return 1;
    }

    std::string filepath;
    std::string solver;
    std::string arg;
    double eta = 0;
    unsigned int iterations = 0;    

    // Iterator starts from one to check the options and skips the app's name.
    for (size_t i = 1; i < argc; i++)
    {
        
        arg = argv[i];

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
            //Check that there is an eta value after the --iterations/-i option.
            if (i + 1 < argc)
            {
                // use ++i to take the solver name instead of the option parameter.
                // Convert string to int with atoi()
                iterations = std::atoi(argv[++i]);
            }
        }
    }

    //Check if the solver has the right values (gradient or normal).
    if(! (solver == "normal" || solver == "gradient")){
        std::cerr << "Invalid arguments for --solver." << std::endl;
    }

    try
    {
        // A variable that stores the number of lines in the file. 
        // It is usefull to check if the file was read correctly.
        int num_lines = 0;

        // Open file.
        std::ifstream input_file;
        input_file.open(filepath, std::ios::in);

        // Check if the file is readable.
        // good() method is false if any of the eofbit, failbit or badbit is set.
        // In that case, throw an error.
        if (! input_file.good()) 
        {
            throw std::ios_base::failure("Something went wrong with the input file...");
        }
        // If the file is readable, then count how many lines it has.
        // We will use that information to check that the file was read correctly.  
        else if (input_file.is_open())
        {
            std::string line;
            // Get the number of lines.
            while (getline(input_file, line))
            {
                num_lines++;
            }
            
        }
        // Close the file.
        input_file.close();
        

        // Get the data from the given file and put it inside vector (vec).
        // data is an object of type lrgFileLoaderDataCreator and has a shared_ptr as one of its attributes.
        // data_ptr is another shared_ptr that points to data. 
        // We had to use shared pointers to achieve this implementation.
        // vec_ptr is also a shared_ptr but it could be a unique_ptr, too.
        // Shared pointers go as follow:  [the arrow (-->) means a "points to" relationship]
        // Initially, vec_ptr --> vec
        // then, data.m_vec_ptr --> vec
        // then, data_ptr --> data and abstractly we could say that data_ptr --> vec
        // vec_ptr seems to be useles, but actually creates the ownership between a pointer and the vector (vec)
        // Next, that ownership is passed to the data object.      
        pdd_vector vec;
        auto vec_ptr = std::make_shared<pdd_vector>(vec);
        lrgFileLoaderDataCreator data(filepath, std::move(vec_ptr));
        std::shared_ptr<lrgDataCreatorI> data_ptr = std::make_shared<lrgFileLoaderDataCreator>(data);
        vec = data_ptr->GetData();

        // One way to check if the file was read correctly is to check 
        // if the vector size equals the number of lines. 
        if (vec.size() != num_lines)
        {
            throw std::ios_base::failure("Something went wrong with the input file...");
        }
         

        // use FitData() of lrgNormalEquationSolverStrategy.
        if (solver == "normal")
        {
            // In that case we can use unique pointers.
            // This is a case of how polymorphism can be used.
            lrgNormalEquationSolverStrategy strategy;
            std::unique_ptr<lrgLinearModelSolverStrategyI> solver = std::make_unique<lrgNormalEquationSolverStrategy>(strategy);
            pdd thetas = solver->FitData(vec);
            std::cout << "t0: " << thetas.first << ", t1: " << thetas.second << std::endl;
        }
        // use FitData() of lrgGradientDescentSolverStrategy.
        else if (solver == "gradient")
        {
            // In that case we can use unique pointers.
            // This is a another case of how polymorphism can be used.
            lrgGradientDescentSolverStrategy strategy(eta, iterations);
            std::unique_ptr<lrgLinearModelSolverStrategyI> solver = std::make_unique<lrgGradientDescentSolverStrategy>(strategy);
            pdd thetas = solver->FitData(vec);
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
