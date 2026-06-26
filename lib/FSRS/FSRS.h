#pragma once

enum Grade : int { AGAIN = 1, HARD, GOOD, EASY };

class Scheduler {
 public:
  float w[21] = {0.212,  1.2931, 2.3065, 8.2956, 6.4133, 0.8334, 3.0194, 0.001,  1.8722, 0.1666, 0.796,
                 1.4835, 0.0614, 0.2629, 1.6483, 0.6014, 1.8729, 0.5425, 0.0912, 0.0658, 0.1542};  // Parameters
  float dr;                                                                                        // Desired Retention

  void review(float& s, float& d, unsigned long millisSince, bool isFirst, bool isSameDay, Grade g);
  float getInterval(float s);

  Scheduler(float dr = 0.9) : dr(dr) {};
};
