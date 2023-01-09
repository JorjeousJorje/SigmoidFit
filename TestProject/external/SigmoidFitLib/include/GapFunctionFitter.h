#pragma once
#include <functional>
#include <utility>
#include <vector>

#include "FunctionFitter.h"
#include "InitialParametersCreator.h"
#include "SignalScaler.h"
#include "Utils.h"
#include "dataanalysis.h"

namespace CurveFitCpp
{
  class GapFunctionFitter : FunctionFitter
  {
  public:
    virtual ~GapFunctionFitter() = default;
    GapFunctionFitter() = delete;

    template <class... Args,
              std::enable_if_t<(... && std::is_same_v<Args, std::vector<double>>),
                               std::nullptr_t>  = nullptr>
    FitResult Fit(const std::size_t iGap,
                  const std::vector<double>& iSignalPart,
                  const Args&... iSignalParts)
    {
      auto x = GapXDataGenerator::GenerateXDataWithGap(iGap, iSignalPart.size(), sizeof ...(Args) + 1);
      const auto y = BuildSignalFromParts(iSignalPart, iSignalParts...);

      FitParameters fitParams = FunctionFitter::Fit(x, y, _func, *_paramsCreator);
      FitResult oFitResult = {fitParams, x, y};

      if (GlobalFitConfig::UNSCALE_SIGNAL)
        ResetScaling(oFitResult);
      return oFitResult;
    }

  protected:
    virtual void ResetScaling(FitResult& iFitResult) = 0;

    explicit GapFunctionFitter(std::shared_ptr<InitialParametersCreator> iParamsCreator,
                               FitFuncType iFunc):
      _paramsCreator{std::move(iParamsCreator)}, _func{std::move(iFunc)}
    {
    }


    std::shared_ptr<InitialParametersCreator> _paramsCreator;
    FitFuncType _func;
  };
}
