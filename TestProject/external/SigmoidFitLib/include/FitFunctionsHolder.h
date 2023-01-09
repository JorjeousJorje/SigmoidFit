#pragma once
#include <ap.h>
#include <cfenv>

class FitFunctionsHolder
{
public:
  static void SigmoidFunc(const alglib::real_1d_array& iC, const alglib::real_1d_array& x, double& func, void*)
  {
    const auto& a = iC[0];
    const auto& c = iC[1];
    const auto& x0 = iC[2];
    const auto& b = iC[3];
    func = a / (1.0 + std::exp(c * (x[0] - x0))) + b;
  }

  static void Sigmoid2(const alglib::real_1d_array& c, const alglib::real_1d_array& x, double& func, void*)
  {
    const auto& a1 = c[0];
    const auto& c1 = c[1];
    const auto& x10 = c[2];
    const auto& a2 = c[3];
    const auto& c2 = c[4];
    const auto& x20 = c[5];
    const auto& b = c[6];
    func = a1 / (1. + std::exp(c1 * (x[0] - x10))) + a2 / (1. + std::exp(c2 * (x[0] - x20))) + b;
  }

  static void Sigmoid2Grad(const alglib::real_1d_array& c, const alglib::real_1d_array& x, double& func, alglib::real_1d_array& grad, void*)
  {
    const auto& a1 = c[0];
    const auto& c1 = c[1];
    const auto& x10 = c[2];
    const auto& a2 = c[3];
    const auto& c2 = c[4];
    const auto& x20 = c[5];
    const auto& b = c[6];

    const auto expc1 = std::exp(c1 * (x[0] - x10));
    const auto expc2 = std::exp(c2 * (x[0] - x20));
    const auto denom1 = 1. + expc1;
    const auto denom2 = 1. + expc1;
    func = a1 / denom1 + a2 / denom2 + b;

    grad[0] = 1. / denom1;
    grad[1] = -a1 * (x[0] - x10) * expc1 / denom1 * denom1;
    grad[2] = a1 * c1 / expc1 - a1 * c1 / expc1 * expc1;

    grad[3] = 1. / denom2;
    grad[4] = -a2 * (x[0] - x20) * expc2 / denom2 * denom2;
    grad[5] = a2 * c2 / expc2 - a2 * c2 / expc2 * expc2;
    grad[6] = 1.;

  }

};
