#pragma once
#include <map>
#include "GapFunctionFitter.h"
#include "SigmoidGapFunctionFitter.h"
#include "Sigmoid2GapFunctionFitter.h"

namespace CurveFitCpp
{
  enum class FunctionName
  {
    Sigmoid,
    Sigmoid2
  };
  using GapFitterPtr = std::shared_ptr<CurveFitCpp::GapFunctionFitter>;


  static inline std::map<FunctionName, GapFitterPtr> fitters = {
    {FunctionName::Sigmoid, std::make_shared<CurveFitCpp::SigmoidGapFunctionFitter>()},
    {FunctionName::Sigmoid2, std::make_shared<CurveFitCpp::Sigmoid2GapFunctionFitter>()}

  };
}