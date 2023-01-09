#pragma once
#include "FitFunctionsHolder.h"
#include "GapFunctionFitter.h"
#include "GlobalFitConfig.h"

namespace CurveFitCpp
{
  class SigmoidInitialParametersCreator : public InitialParametersCreator
  {
  public:
    double currentMax{1.};
    double bScaling{1.};
    std::vector<double> GetInitialParams(const std::vector<double>& iX, const std::vector<double>& iY) override
    {
      if (iX.empty() && iY.empty())
        return { 1.0, 1.0, 1.0, 1.0 };

      const auto [min, max] = std::minmax_element(iY.begin(), iY.end());
      currentMax = *max;
      const auto a = *max - *min;
      const auto c = iY.back() - iY.front() > 0 ? -_initialSlopeValue : _initialSlopeValue;
      const auto diff = max - min;
      int x0Idx;
      if (diff <= 0)
        x0Idx = std::distance(iY.begin(), max) + std::abs(diff) / 2;
      else
        x0Idx = std::distance(iY.begin(), min) + std::abs(diff) / 2;
      auto x0 = iX[x0Idx];
      const auto b = *min;
      bScaling = b;
      return {a, c, x0, b};
    }

    std::vector<double> GetScaling() const override
    {
      if (GlobalFitConfig::USE_SIGNAL_SCALING)
        return {
          currentMax * 1e2,
          1.0,
          *GlobalFitConfig::minMaxScaler.scaledToMax,
          bScaling * 5
      };
      return {1e5, 1.0, 1e5, 1e5}; // a, c, x0, b - params order.
    }

  private:
    double _initialSlopeValue{0.3};
  };

  class SigmoidGapFunctionFitter : public GapFunctionFitter
  {
  public:
    explicit SigmoidGapFunctionFitter() : GapFunctionFitter{ std::make_shared <SigmoidInitialParametersCreator>(), FitFunctionsHolder::SigmoidFunc}
    {}

  private:
    void ResetScaling(FitResult& iFitResult) override
    {
      const auto x0 = iFitResult.fitParams.params[2];
      iFitResult.fitParams.params[2] = GlobalFitConfig::minMaxScaler.UnScaleValue(x0);
      GlobalFitConfig::minMaxScaler.UnScaleSignal(iFitResult.x);
    }
  };
}
