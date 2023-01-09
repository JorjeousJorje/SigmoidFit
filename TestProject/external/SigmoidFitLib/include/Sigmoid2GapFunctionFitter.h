#pragma once
#include "GapFunctionFitter.h"
#include "GlobalFitConfig.h"
#include "SigmoidGapFunctionFitter.h"
#include "dataanalysis.h"

namespace CurveFitCpp
{
  class Sigmoid2InitialParametersCreator : public SigmoidInitialParametersCreator
  {
    double _bScaling{1.};
    double _Sig1Max{1.};
    double _Sig2Max{1.};
  public:
    [[nodiscard]] std::vector<double> GetInitialParams(const std::vector<double>& iX,
                                                       const std::vector<double>& iY) override
    {
      const auto minIt = std::min_element(std::begin(iY), std::end(iY));
      std::vector<double> firstSigmoid{};
      std::copy(iY.begin(), minIt, std::back_inserter(firstSigmoid));
      std::vector<double> secondSigmoid{};
      std::copy(minIt, iY.end(), std::back_inserter(secondSigmoid));

      std::vector<double> x1{};
      std::copy_n(iX.begin(), std::distance(iY.begin(), minIt), std::back_inserter(x1));
      auto fitParamsFirstSigmoid = SigmoidInitialParametersCreator::GetInitialParams(x1, firstSigmoid);
      const auto sig1b = fitParamsFirstSigmoid.back();
      fitParamsFirstSigmoid.erase(fitParamsFirstSigmoid.end() - 1);
      _Sig1Max = currentMax;

      std::vector<double> x2{}; 
      std::copy(iX.begin() + std::distance(iY.begin(), minIt), iX.end(), std::back_inserter(x2));
      const auto fitParamsSecondSigmoid = SigmoidInitialParametersCreator::GetInitialParams(x2, secondSigmoid);
      std::vector<double> oResultFitParams{fitParamsFirstSigmoid};
      oResultFitParams.insert(oResultFitParams.end(), fitParamsSecondSigmoid.begin(),
                              fitParamsSecondSigmoid.end());
      _Sig2Max = currentMax;
      assert(oResultFitParams.size() == 7);
      _bScaling = oResultFitParams.back();
      return oResultFitParams;
    }

    [[nodiscard]] std::vector<double> GetScaling() const override
    {
      if (GlobalFitConfig::USE_SIGNAL_SCALING)
        return {
          _Sig1Max * 1e2,
          1.0,
          *GlobalFitConfig::minMaxScaler.scaledToMax,
          _Sig2Max * 1e2,
          1.0,
          *GlobalFitConfig::minMaxScaler.scaledToMax,
          _bScaling * 5
        };
      return {1e5, 1.0, 1e5, 1e5, 1.0, 1e5, 1e5}; // a1, c1, x10, a2, c2, x20, b - params order.
    }
  };

  class Sigmoid2GapFunctionFitter : public GapFunctionFitter
  {
  public:
    explicit Sigmoid2GapFunctionFitter() : GapFunctionFitter{ std::make_shared<Sigmoid2InitialParametersCreator>(), FitFunctionsHolder::Sigmoid2}
    {}

  private:
    void ResetScaling(FitResult& iFitResult) override
    {
      const auto x10 = iFitResult.fitParams.params[2];
      const auto x20 = iFitResult.fitParams.params[5];
      iFitResult.fitParams.params[2] = GlobalFitConfig::minMaxScaler.UnScaleValue(x10);
      iFitResult.fitParams.params[5] = GlobalFitConfig::minMaxScaler.UnScaleValue(x20);
      GlobalFitConfig::minMaxScaler.UnScaleSignal(iFitResult.x);
    }
  };
}
