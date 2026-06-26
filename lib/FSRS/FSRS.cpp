#include "FSRS.h"

#include <algorithm>
#include <cmath>

// Based on https://expertium.github.io/Algorithm.html#intervals

void Scheduler::review(float& s, float& d, unsigned long millisSince, bool isFirst, bool isSameDay, Grade g) {
  const float t = millisSince / (1000 * 60 * 60 * 24);  // convert to days
  const float exponent = -1 / w[19];
  const float factor = std::pow(0.9, exponent) - 1;
  const float r = std::pow((1 + factor * t / s), -w[19]);

  // Stability
  if (isFirst) {
    switch (g) {
      case AGAIN:
        s = w[0];
        break;
      case HARD:
        s = w[1];
        break;
      case GOOD:
        s = w[2];
        break;
      case EASY:
        s = w[3];
        break;
    }
  } else {
    if (isSameDay) {
      const float fG = std::expf(w[17] * (g - 3 + w[18]));
      const float fS = std::pow(s, -w[19]);

      float sInc = fG * fS;

      // Good and easy review cannot decrease stability
      if (g == GOOD || g == EASY) {
        sInc = std::max(sInc, 1.0f);
      }

      s *= sInc;
    } else {
      if (g == AGAIN) {
        const auto fD = std::pow(d, -w[12]);
        const auto fS = std::pow(s + 1, w[13]) - 1;
        const auto fR = std::expf(w[14] * (1 - r));

        const auto newS = w[11] * fD * fS * fR;

        s = std::min(newS, s);
      } else {
        const auto fD = 11 - d;
        const auto fS = std::pow(s, -w[9]);
        const auto fR = std::expf(w[10] * (1 - r)) - 1;

        float mul;

        if (g == GOOD || g == EASY) {
          mul = w[16];
        } else {
          mul = w[15];
        }

        const auto sInc = 1 + mul * fD * fS * fR;
        s *= sInc;
      }
    }
  }

  // Difficulty
  if (isFirst) {
    d = w[4] - std::expf(w[5] * (g - 1)) + 1;
  } else {
    auto dDelta = -w[4] * (g - 3);
    dDelta *= (10 - d) / 9;
    d += dDelta;
    auto d0 = w[4] - std::expf(w[5] * (4 - 1)) + 1;
    d = w[7] * d0 + (1 - w[7]) * d;
  }

  d = std::clamp(d, 1.0f, 10.0f);
}

float Scheduler::getInterval(float s) {
  const float exponent = -1 / w[20];
  const float factor = std::pow(0.9, exponent) - 1;
  return (s / (factor)) * (std::pow(dr, exponent));
}
