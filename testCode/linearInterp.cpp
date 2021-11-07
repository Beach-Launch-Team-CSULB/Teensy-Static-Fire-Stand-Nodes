#include <iostream>
#include <array>

double linearInter(const double& x, const std::array<double,5>& a, const std::array<double,5> b)
{
    double output{};

    for(std::size_t i{}; i<(a.size()-1); ++i)
    {
        if((x >= a.at(i)) && (x <= a.at(i+1)))
        {
            double num{b.at(i+1)-b.at(i)};
            double dem{a.at(i+1)-a.at(i)};
            output = b.at(i) + (x - a.at(i))*(num/dem);
        }
    }
    return output;
}

int main()
{
    std::array<double,5> lookup1{0, 1, 2, 3, 4};
    std::array<double,5> lookup2{0, 10, 20, 30, 40};

    double x{2.5};

    std::cout << linearInter(x,lookup1,lookup2) << '\n';

    return 0;
}