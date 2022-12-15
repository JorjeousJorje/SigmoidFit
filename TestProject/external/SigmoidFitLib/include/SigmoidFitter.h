#pragma once
#include <algorithm>
#include <optional>
#include <utility>

#include "interpolation.h"
#include "SigmoidFitUtility.h"

using RealArray1D = alglib::real_1d_array;

struct SigmoidFitParameters
{
  struct FitError
  {
    double spanError{};
    double slopeError{};
    double medianError{};
    double minError{};
  };

  double spanValue{};
  double slopeValue{};
  double medianValue{};
  double minValue{};
  FitError errors{};
};


struct FitResult
{
  FitResult() = delete;
  SigmoidFitParameters params{};
  std::vector<double> x;
  std::vector<double> y;
};

struct FitSettings
{
  double eps{1.0e-5};
  double diffStep{1.0e-5};
  alglib::ae_int_t maxIts{};
};

struct LsFitDetails
{
  alglib::ae_int_t info{};
  alglib::lsfitstate state{};
  alglib::lsfitreport rep{};
};

struct ScaleInfo
{
  double spanValueScaleFactor{1.0};
  double slopeValueScaleFactor{1.0};
  double medianValueScaleFactor{1.0};
  double minValueScaleFactor{1.0};
};

class SigmoidFitter
{
  FitResult Fit(std::vector<double> iX, std::vector<double> iY,
                const SigmoidFitParameters& iC);

public:
  template <class... Args,
            std::enable_if_t<(... && std::is_same_v<Args, std::vector<double>>),
                             std::nullptr_t>  = nullptr>
  FitResult Fit(const std::size_t iGap, const std::vector<double>& iPart,
                       const Args&... iParts)
  {
    std::vector resultSignal{iPart};
    if constexpr (sizeof ...(Args) > 0) {
      for (auto&& part : {iParts ...})
        SigmoidFitUtility::operator+=(resultSignal, part);
    }


    auto xData = SigmoidFitUtility::GapXDataGenerator::GenerateXDataWithGap(iGap, iPart.size(), sizeof ...(Args) + 1);

    const auto [min, max] = std::minmax_element(resultSignal.begin(), resultSignal.end());
    const auto initialSpanValue = *max - *min;
    const auto initialSlopeValue = resultSignal.back() - resultSignal.front() > 0 ? 0.05 : -0.05;
    const auto initialMedianValue = initialSpanValue / 2.f;
    const auto initialMinValue = *min;

    const SigmoidFitParameters initialParams = {
      initialSpanValue,
      initialSlopeValue,
      initialMedianValue,
      initialMinValue,
      {}
    };

    return Fit(std::move(xData), std::move(resultSignal), initialParams);
  }

  FitSettings& GetFitSettings();
  const FitSettings& GetFitSettings() const;
  const SigmoidFitParameters& GetFitParams() const;
  void SetScaling(const ScaleInfo&);

private:
  std::optional<ScaleInfo> _scaleInfo{{1e5, 1.0, 1e5, 1e5}};
  SigmoidFitParameters _fitParams{};
  LsFitDetails _lsFitDetails{};
  FitSettings _settings{};

  alglib::real_2d_array _x{};
  alglib::real_1d_array _y{};
  alglib::real_1d_array _c{};

  void FitData();
  void InitFitArrays(const std::vector<double>& iX, const std::vector<double>& iY, const SigmoidFitParameters& iC);
  static void SigmoidFuncFit(const RealArray1D& c, const RealArray1D& x, double& func, void* ptr);
};
