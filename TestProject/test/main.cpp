#include "FittersHolder.h"

using namespace CurveFitCpp;

int main(int argc, char** argv)
{
  // Consider signal parts and a gap between them
  constexpr std::size_t gap = 147;
  std::vector p1{ 1e5 + 0.7, 1e5 + 0.7, 1e5 + 0.9, 1e5 + 1.27, 1e5 + 1.81};
  std::vector p2{ 1e5 + 2.64, 1e5 + 3.37, 1e5 + 4.89, 1e5 + 5.3, 1e5 + 5.3 };
  // Single sigmoid fitter
  {
    SigmoidGapFunctionFitter fitter{};
    auto [params, x, y] = fitter.Fit(gap, p1, p2);
    const auto paramsVec = params.params;
    const auto paramsErrorsVec = params.errors;


    const auto a = paramsVec[0];  // spanValue
    const auto c = paramsVec[1];  // slopeValue
    const auto x0 = paramsVec[2]; // medianValue
    const auto b = paramsVec[3];  // minValue
    std::cout << a << std::endl;
  }

  // Sigmoid combination fitter
  {
    Sigmoid2GapFunctionFitter fitter{};
    auto [params, x, y] = fitter.Fit(gap, p1, p2);
    const auto paramsVec = params.params;
    const auto paramsErrorsVec = params.errors;


    const auto a1 = paramsVec[0];  // spanValue1
    const auto c1 = paramsVec[1];  // slopeValue1
    const auto x10 = paramsVec[2]; // medianValue1
    const auto a2 = paramsVec[3];  // spanValue2
    const auto c2 = paramsVec[4];  // slopeValue2
    const auto x20 = paramsVec[5]; // medianValue2
    const auto b = paramsVec[6];   // minValue
    std::cout << a1 << std::endl;
  }


  // Also fitters can be used like that (without creating object):
  // Fits single sigmoid
  {
    auto [params, x, y] = fitters[FunctionName::Sigmoid]->Fit(gap, p1, p2);
  }
  // Fits sigmoid combination (named as "FunctionName::Sigmoid2")
  {
    auto [params, x, y] = fitters[FunctionName::Sigmoid2]->Fit(gap, p1, p2);
  }

  return 0;
}
