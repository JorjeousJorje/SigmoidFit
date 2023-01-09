#pragma once
#include <algorithm>
#include <ap.h>
#include <cassert>
#include <cfenv>
#include <functional>

#include "FitFunctionsHolder.h"
#include "InitialParametersCreator.h"
#include "Utils.h"

namespace CurveFitCpp
{
  using FitFuncType = std::function<void(const alglib::real_1d_array& c,
                                         const alglib::real_1d_array& x,
                                         double& func,
                                         void*)>;

  class FunctionFitter
  {
  public:
    FitParameters Fit(const std::vector<double>& iX,
                             const std::vector<double>& iY,
                             const FitFuncType& iFunc,
                             const std::vector<double>& iP0 = {},
                             const std::vector<double>& iScale = {})
    {
      assert(iX.size() == iY.size());

      if (!iScale.empty() && !iP0.empty())
        assert(iP0.size() == iScale.size());

      FitParameters oFitParams;
      InitFitArrays(iX, iY, iP0, iScale);
      FitData(iFunc);

      oFitParams.params = std::vector<double>{
        _c.getcontent(), _c.getcontent() + iP0.size()
      };
      oFitParams.errors = std::vector<double>{
        _fitDetails.rep.errpar.getcontent(), _fitDetails.rep.errpar.getcontent() + iP0.size()
      };

      return oFitParams;
    }

    FitParameters Fit(const std::vector<double>& iX,
                             const std::vector<double>& iY,
                             const FitFuncType& iFunc,
                             InitialParametersCreator& iP0ParamsCreator)
    {
      assert(iX.size() == iY.size());
      const auto p0 = iP0ParamsCreator.GetInitialParams(iX, iY);
      const auto scale = iP0ParamsCreator.GetScaling();
      return Fit(iX, iY, iFunc, p0, scale);
    }

  private:
    void InitFitArrays(const std::vector<double>& iX,
                              const std::vector<double>& iY,
                              const std::vector<double>& iP0,
                              const std::vector<double>& iScale)
    {
      _x.setcontent(iX.size(), 1, iX.data());
      _y.setcontent(iY.size(), iY.data());
      _c.setcontent(iP0.size(), iP0.data());

      if (!iScale.empty())
        _s.setcontent(iScale.size(), iScale.data());
    };

    void FitData(const FitFuncType& iFunc)
    {
      lsfitcreatef(_x, _y, _c, _fitSettings.diffStep, _fitDetails.state);
      lsfitsetcond(_fitDetails.state, _fitSettings.eps, _fitSettings.maxIts);

      if (_s.getcontent())
        lsfitsetscale(_fitDetails.state, _s);

      const auto ppFunc = iFunc.target<void(*)(const alglib::real_1d_array& c,
                                               const alglib::real_1d_array& x,
                                               double& func,
                                               void*)>();
      lsfitfit(_fitDetails.state, *ppFunc);
      lsfitresults(_fitDetails.state, _fitDetails.info, _c, _fitDetails.rep);
    }

    LsFitDetails _fitDetails{};
    FitSettings _fitSettings{};
    alglib::real_2d_array _x{};
    alglib::real_1d_array _y{};
    alglib::real_1d_array _c{};
    alglib::real_1d_array _s{};
  };
}
