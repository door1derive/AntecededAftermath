#include <iostream>
#include "include/facility/ik.hpp"

int main() {
    using facility::two_link_ik_degrees;
    using facility::IKResult;

    struct Case { double ang; double r; double l1; double l2; };
    Case cases[] = {
        {0.0, 6.0, 3.0, 3.0},
        {0.0, 4.0, 3.0, 3.0},
        {45.0, 4.0, 3.0, 3.0},
        {120.0, 1.0, 1.2, 0.5},
        {270.0, 0.0, 1.0, 1.0}
    };

    for (auto &c : cases) {
        IKResult r = two_link_ik_degrees(c.ang, c.r, c.l1, c.l2, 1, true, std::nullopt);
        std::cout << "Case angle=" << c.ang << " r=" << c.r << " l1=" << c.l1 << " l2=" << c.l2 << "\n";
        std::cout << "  result -> hip=" << r.hip_deg << ", knee=" << r.knee_deg << ", reachable=" << r.reachable << "\n\n";
    }

    return 0;
}
