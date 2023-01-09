#pragma once
#include <interpolation.h>
#include "GlobalFitConfig.h"
#include "SignalScaler.h"
#include <functional>

namespace CurveFitCpp
{
  template <typename Signature>
  struct CountArgs;

  template <typename Ret, typename... Args>
  struct CountArgs<std::function<Ret(Args...)>> {
    static constexpr size_t value = sizeof...(Args);
  };

  struct FitParameters
  {
    std::vector<double> params{};
    std::vector<double> errors{};
  };

  struct FitResult
  {
    FitParameters fitParams{};
    std::vector<double> x{};
    std::vector<double> y{};
  };

  struct FitSettings
  {
    double eps{ 0 };
    double diffStep{ 1e-3 };
    alglib::ae_int_t maxIts{};
  };

  struct LsFitDetails
  {
    alglib::ae_int_t info{};
    alglib::lsfitstate state{};
    alglib::lsfitreport rep{};
  };

  template <typename T>
  std::vector<T> operator+(const std::vector<T>& iA, const std::vector<T>& iB)
  {
    std::vector<T> AB;
    AB.reserve(iA.size() + iB.size());
    AB.insert(AB.end(), iA.begin(), iA.end());
    AB.insert(AB.end(), iB.begin(), iB.end());
    return AB;
  }

  template <typename T>
  std::vector<T>& operator+=(std::vector<T>& oA, const std::vector<T>& iB)
  {
    oA.reserve(oA.size() + iB.size());
    oA.insert(oA.end(), iB.begin(), iB.end());
    return oA;
  }

  template <class... Args,
    std::enable_if_t<(... && std::is_same_v<Args, std::vector<double>>),
    std::nullptr_t> = nullptr>
  std::vector<double> BuildSignalFromParts(const std::vector<double>& iPart,
    const Args&... iParts)
  {
    std::vector oResultSignal{ iPart };
    if constexpr (sizeof ...(Args) > 0) {
      for (auto&& part : { iParts ... })
        oResultSignal += part;
    }
    return oResultSignal;
  }

  class GapXDataGenerator
  {
  public:
    static std::vector<double> GenerateXDataWithGap(const std::size_t iGap, const std::size_t iPartLength, const std::size_t iPartsNum)
    {
      std::vector<double> oXData(iPartsNum * iPartLength);
      if (iPartsNum == 1) {
        for (auto i = 0u; i < iPartLength; ++i)
          oXData[i] = static_cast<double>(i);
        return oXData;
      }

      for (auto pnm = 1u; pnm < iPartsNum + 1u; ++pnm) {
        for (auto i = 0u; i < iPartLength; ++i)
          oXData[i + (pnm - 1u) * iPartLength] = static_cast<double>(i + (pnm - 1u) * (iPartLength + iGap));
      }

      if (GlobalFitConfig::USE_SIGNAL_SCALING)
        GlobalFitConfig::minMaxScaler.ScaleSignal(oXData, 0, 100);

      return oXData;
    }
  };

}
