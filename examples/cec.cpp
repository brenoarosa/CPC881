#include <iostream>
#include <pagmo/pagmo.hpp>

using namespace pagmo;

int main()
{
    // 1 - Instantiate a pagmo problem constructing it from a UDP
    // (user defined problem).
    problem prob{cec2014(5, 10)};

    // 2 - Instantiate a pagmo algorithm
    algorithm algo{cmaes(100)};

    // 3 - Instantiate an archipelago with 16 islands having each 20 individuals
    archipelago archi{8, algo, prob, 20};

    // 4 - Run the evolution in parallel on the 16 separate islands 10 times.
    archi.evolve(1000);

    // 5 - Wait for the evolutions to be finished
    archi.wait();

    // 6 - Print the fitness of the best solution in each island
    unsigned int f = 0;
    for (const auto &isl : archi) {
        f += isl.get_population().champion_f().at(0);
    }
    f /= 8;

    std::cout << f << std::endl;
}
