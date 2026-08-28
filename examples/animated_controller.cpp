#include <iostream>
#include <vector>
#include <optional>
#include <utility>
#include <cmath>
#include "include/facility/ik.hpp"

// Animated IK controller example
// Builds a sequence of target polar positions (angle_deg, r) and calls
// facility::two_link_ik_degrees with elbow_choice=0 (auto) and prev_angles
// set each frame to demonstrate continuity (avoid elbow flips).
//
// Build (example):
//   g++ -std=c++17 -Iinclude src/facility/ik.cpp examples/animated_controller.cpp -O2 -o build/animated_controller
// Run:
//   ./build/animated_controller > ik_output.csv
// Then inspect ik_output.csv (contains comma-separated: frame,angle_deg,r,hip_deg,knee_deg,reachable)

int main() {
    using facility::two_link_ik_degrees;
    using facility::IKResult;

    const double l1 = 3.0;
    const double l2 = 3.0;

    // Prepare a sequence of targets that sweep around and vary radius.
    std::vector<std::pair<double,double>> targets;
    for (int step = 0; step < 360; step += 5) {
        double angle = static_cast<double>(step); // degrees
        // radius oscillates between 4.0 and 7.0 (goes slightly beyond reach=6.0)
        double r = 5.5 + 1.5 * std::sin(angle * M_PI / 180.0 * 2.0);
        targets.emplace_back(angle, r);
    }

    // Header for CSV
    std::cout << "frame,angle_deg,r,hip_deg,knee_deg,reachable" << std::endl;

    std::optional<std::pair<double,double>> prev_angles = std::nullopt;
    int frame = 0;
    for (auto &t : targets) {
        double angle_deg = t.first;
        double r = t.second;

        // elbow_choice = 0 => choose the solution; pass prev_angles to prefer continuity
        IKResult res = two_link_ik_degrees(angle_deg, r, l1, l2, 0, true, prev_angles);

        std::cout << frame << "," << angle_deg << "," << r << "," << res.hip_deg << "," << res.knee_deg << "," << (res.reachable ? 1 : 0) << std::endl;

        // update previous angles for next iteration
        prev_angles = std::make_pair(res.hip_deg, res.knee_deg);
        ++frame;
    }

    return 0;
}
