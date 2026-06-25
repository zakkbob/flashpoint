#pragma once

namespace FSRS {

enum Grade : int { AGAIN = 1, HARD, GOOD, EASY };

struct Memory {
  float s = 1;       // Stability
  float d = 6.4133;  // Difficulty
  int reviews = 0;
  unsigned long lastReviewMillis = 0;

  bool isFirstReview() { return reviews == 0; };
  bool isSameDay() { return false; };
};

class Scheduler {
 private:
  float w[21] = {0.212,  1.2931, 2.3065, 8.2956, 6.4133, 0.8334, 3.0194, 0.001,  1.8722, 0.1666, 0.796,
                 1.4835, 0.0614, 0.2629, 1.6483, 0.6014, 1.8729, 0.5425, 0.0912, 0.0658, 0.1542};  // Parameters
  float dr;                                                                                        // Desired Retention
 public:
  void review(Memory& c, Grade g);
  float getInterval(Memory c);

  Scheduler(float dr = 0.9) : dr(dr) {};
};
};  // namespace FSRS
