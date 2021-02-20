#include <lrgExceptionMacro.h>
#include <iostream>

// A function that shows how to use the app in the command line.
// Inspiration was taken from the official cplusplus website.
// http://www.cplusplus.com/articles/DEN36Up4/
static void how_to_use(std::string app)
{
    std::cerr << "Usage: " << app << " <option(s)>\n"
              << "Options:\n"
              << "\t-h,--help\t\tShows how to set the command line arguments.\n"
              << "\t-f,--file FILE\t\tSpecify the absolute path of the input file.\n"
              << "\t-s,--solver SOLVER\tSpecify the solver (linear or gradient)\n\n"
              << "Example: Inside the build directory run in command line\n"
              << "./bin/lrgFitDataApp -f <filepath> -s <solver>\n"
              << std::endl;
}

int main(int argc, char **argv)
{
    int returnStatus = EXIT_FAILURE;

    // Command line app expects 3 arguments.
    // 1. The app's name.
    // 2. The file path (-f <filepath>)
    // 3. The solver (-s <solver>)
    if (argc < 5 || argc > 5)
    {
        how_to_use(argv[0]);
        return 1;
    }

    std::string filepath;
    std::string solver;

    // Iterator starts from one to check the options.
    for (size_t i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        // This block runs only if the user gives the right value of arguments (5) and one of them is the -h or --help.
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
            else
            {
                std::cerr << "You missed an argument after --file parameter." << std::endl;
                return 1;
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
            else
            {
                std::cerr << "You missed an argument after --solver parameter." << std::endl;
                return 1;
            }
        }
    }
            std::cerr << filepath << " " << solver << std::endl;

    try
    {

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
