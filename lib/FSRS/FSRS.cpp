#include "FSRS.h"

#include <Arduino.h>
#include <Logging.h>

#include <algorithm>
#include <cmath>

// Based on https://expertium.github.io/Algorithm.html#intervals

namespace FSRS {

void Scheduler::review(Memory& c, Grade g) {
  const float t = (millis() - c.lastReviewMillis) / (1000 * 60 * 60 * 24);  // convert to days
  const float exponent = -1 / w[19];
  const float factor = std::pow(0.9, exponent) - 1;
  const float r = std::pow((1 + factor * t / c.s), -w[19]);

  LOG_DBG("FLASHCARDS", "Difficulty: %f", c.d);
  LOG_DBG("FLASHCARDS", "Stability: %f", c.s);
  LOG_DBG("FLASHCARDS", "Interval: %f", getInterval(c));

  // Stability
  if (c.isFirstReview()) {
    switch (g) {
      case AGAIN:
        c.s = w[0];
        break;
      case HARD:
        c.s = w[1];
        break;
      case GOOD:
        c.s = w[2];
        break;
      case EASY:
        c.s = w[3];
        break;
    }
  } else {
    if (c.isSameDay()) {
      float fG = std::expf(w[17] * (g - 3 + w[18]));
      const float fS = std::pow(c.s, -w[19]);

      float sInc = fG * fS;

      // Good and easy review cannot decrease stability
      if (g == GOOD || g == EASY) {
        sInc = std::max(sInc, 1.0f);
      }

      c.s *= sInc;

    } else {
      if (g == AGAIN) {
        const auto fD = std::pow(c.d, -w[12]);
        const auto fS = std::pow(c.s + 1, w[13]) - 1;
        const auto fR = std::expf(w[14] * (1 - r));

        const auto newS = w[11] * fD * fS * fR;

        c.s = std::min(newS, c.s);
      } else {
        const auto fD = 11 - c.d;
        const auto fS = std::pow(c.s, -w[9]);
        const auto fR = std::expf(w[10] * (1 - r)) - 1;

        float mul;

        if (g == GOOD || g == EASY) {
          mul = w[16];
        } else {
          mul = w[15];
        }

        const auto sInc = 1 + mul * fD * fS * fR;
        c.s *= sInc;
      }
    }
  }

  // Difficulty
  if (c.isFirstReview()) {
    LOG_DBG("FLASHCARDS", "first review");
    c.d = w[4] - std::expf(w[5] * (g - 1)) + 1;
  } else {
    auto dDelta = -w[4] * (g - 3);
    dDelta *= (10 - c.d) / 9;
    c.d += dDelta;
    auto d0 = w[4] - std::expf(w[5] * (4 - 1)) + 1;
    c.d = w[7] * d0 + (1 - w[7]) * c.d;
  }

  c.d = std::clamp(c.d, 1.0f, 10.0f);

  c.reviews++;
  c.lastReviewMillis = millis();

  LOG_DBG("FLASHCARDS", "Difficulty: %f", c.d);
  LOG_DBG("FLASHCARDS", "Stability: %f", c.s);
  LOG_DBG("FLASHCARDS", "Interval: %f", getInterval(c));
}

float Scheduler::getInterval(Memory c) {
  const float exponent = -1 / w[20];
  const float factor = std::pow(0.9, exponent) - 1;
  return (c.s / (factor)) * (std::pow(dr, exponent));
}
};  // namespace FSRS
