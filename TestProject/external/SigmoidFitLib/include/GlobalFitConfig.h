#pragma once
#include "SignalScaler.h"


class GlobalFitConfig
{
public:
  static constexpr bool USE_SIGNAL_SCALING{ true };
  static constexpr bool UNSCALE_SIGNAL{ false };
  static inline CurveFitCpp::MinMaxScaler minMaxScaler{};
  static inline CurveFitCpp::ValueScaler valueScaler{};
};
