#pragma once
#include <vector>
#include "GlobalFitConfig.h"

namespace CurveFitCpp
{
  class InitialParametersCreator
  {
  public:
    virtual ~InitialParametersCreator() = default;
    [[nodiscard]] virtual std::vector<double> GetInitialParams(const std::vector<double>&,
      const std::vector<double>&) = 0;
    [[nodiscard]] virtual std::vector<double> GetScaling() const = 0;
  };
}
