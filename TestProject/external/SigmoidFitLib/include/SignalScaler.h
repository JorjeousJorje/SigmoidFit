#pragma once
#include <vector>
#include <algorithm>
#include <optional>

namespace CurveFitCpp
{
  class MinMaxScaler
  {
  public:
    void ScaleSignal(std::vector<double>& iSignal, const double iMin = 0., const double iMax = 1.)
    {
      scaledToMin = iMin;
      scaledToMax = iMax;

      auto [minIt, maxIt] = std::minmax_element(iSignal.begin(), iSignal.end());
      prevMin = *minIt;
      prevMax = *maxIt;

      const auto scaler = [=](const double iVal)
      {
        return ScaleValue(iVal, iMin, iMax);
      };
      std::transform(iSignal.begin(), iSignal.end(), iSignal.begin(), scaler);
    }
    void UnScaleSignal(std::vector<double>& iSignal)
    {
      const auto scaler = [=](const double iVal)
      {
        return UnScaleValue(iVal);
      };
      std::transform(iSignal.begin(), iSignal.end(), iSignal.begin(), scaler);

      scaledToMin = std::nullopt;
      scaledToMax = std::nullopt;
      prevMin = std::nullopt;
      prevMax = std::nullopt;
    }
    double ScaleValue(const double iVal,
                      const double iMinTo,
                      const double iMaxTo,
                      const double iMinFrom = 0.,
                      const double iMaxFrom = 1.) const
    {
      if (prevMin && prevMax)
        return (iVal - *prevMin) / (*prevMax - *prevMin) * (iMaxTo - iMinTo) + iMinTo;
      return (iVal - iMinFrom) / (iMaxFrom - iMinFrom) * (iMaxTo - iMinTo) + iMinTo;
    }

    double UnScaleValue(const double iVal) const
    {
      return (iVal - *scaledToMin) / (*scaledToMax - *scaledToMin) * (*prevMax - *prevMin) + *prevMin;
    }

    std::optional<double> scaledToMin{};
    std::optional<double> scaledToMax{};
    std::optional<double> prevMin{};
    std::optional<double> prevMax{};
  };

  class ValueScaler
  {
  public:

    void ScaleSignal(std::vector<double>& iSignal, const double iScaleVal)
    {
      const auto scaler = [=](const double iVal)
      {
        return ScaleValue(iVal, iScaleVal);
      };
      std::transform(iSignal.begin(), iSignal.end(), iSignal.begin(), scaler);
    }
    double ScaleValue(const double iVal,const double iScaleValue) const
    {
      return iVal / iScaleValue;
    }
  };
}
