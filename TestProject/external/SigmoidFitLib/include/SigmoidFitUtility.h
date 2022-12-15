#pragma once
#include <vector>

namespace SigmoidFitUtility
{
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

  class GapXDataGenerator
  {
  public:
    static std::vector<double> GenerateXDataWithGap(std::size_t iGap, std::size_t iPartLength, std::size_t iPartsNum);
  };
}
