#include <cmath>
#include <string>

#include "interpolation.h"
#include "SigmoidFitter.h"


int main(int argc, char** argv)
{
  SigmoidFitter fitter{};
  // Consider signal parts and a gap between them
  constexpr std::size_t gap = 147;
  std::vector p1{ 1e5 + 0.7, 1e5 + 0.7, 1e5 + 0.9, 1e5 + 1.27, 1e5 + 1.81};
  std::vector p2{ 1e5 + 2.64, 1e5 + 3.37, 1e5 + 4.89, 1e5 + 5.3, 1e5 + 5.3 };

  {
    auto [params, x, y] = fitter.Fit(gap, p1, p2);

    std::cout << "Fit parameters" << std::endl;
    std::cout << params.spanValue << std::endl;
    std::cout << params.slopeValue << std::endl;
    std::cout << params.medianValue << std::endl;
    std::cout << params.minValue << std::endl;

    std::cout << "Errors" << std::endl;
    std::cout << params.errors.spanError << std::endl;
    std::cout << params.errors.slopeError << std::endl;
    std::cout << params.errors.medianError << std::endl;
    std::cout << params.errors.minError << std::endl;
  }
  return 0;
}
