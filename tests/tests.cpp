#include <stdio.h>
#include <vector>
#include <pagmo/problems/cec2014.hpp>
#include <limits>


int main() {

    unsigned dimensions = 10;
    std::vector<double> x_origin(dimensions, 0);
    std::vector<double> x_min(dimensions, 0);
    double f_origin, f_min;

    for (unsigned func_num = 1; func_num < 31; func_num++) {
        pagmo::cec2014 prob = pagmo::cec2014(func_num, dimensions);

        x_min = prob.m_origin_shift;
        x_min.resize(dimensions);

        f_origin = prob.fitness(x_origin)[0];
        f_min = prob.fitness(x_min)[0];

        std::cout.precision(std::numeric_limits<double>::max_digits10);
        std::cout << "Function [" << func_num << "]:"
                  << "\tminimum = " << f_min
                  << "\torigin = " << f_origin
                  << "\n";

    }

    return 0;
}
