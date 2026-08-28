#pragma once

#include <cmath>
#include <optional>
#include <stdexcept>

namespace facility {

struct IKResult {
    double hip_deg;   // hip/base angle in degrees
    double knee_deg;  // signed knee angle in degrees (positive = elbow-up)
    bool reachable;   // whether the original target distance was within reach
};

// elbow_choice:  1 = elbow-up, -1 = elbow-down, 0 = choose (use prev_angles if provided)
// prev_angles: optional pair (hip_deg, knee_deg) used when elbow_choice == 0 to pick the
//               solution closest to the previous pose (helps avoid flips during animation).
// Notes: angles are degrees in/out. Uses double precision. Requires C++17 for std::optional.
IKResult two_link_ik_degrees(double angle_deg,
                             double r,
                             double l1,
                             double l2,
                             int elbow_choice = 1,
                             bool allow_unreachable = true,
                             const std::optional<std::pair<double,double>>& prev_angles = std::nullopt);

} // namespace facility
