#include "facility/ik.hpp"

namespace facility {

static double clampd(double x, double a, double b) {
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

static double normalize_angle_rad(double x) {
    const double two_pi = 2.0 * M_PI;
    x = std::fmod(x + M_PI, two_pi);
    if (x < 0) x += two_pi;
    return x - M_PI;
}

IKResult two_link_ik_degrees(double angle_deg,
                             double r,
                             double l1,
                             double l2,
                             int elbow_choice,
                             bool allow_unreachable,
                             const std::optional<std::pair<double,double>>& prev_angles) {
    if (l1 <= 0.0 || l2 <= 0.0) throw std::invalid_argument("link lengths must be positive");

    const double deg2rad = M_PI / 180.0;
    const double rad2deg = 180.0 / M_PI;

    double angle = angle_deg * deg2rad;

    if (r <= 0.0) {
        return IKResult{ angle_deg, 0.0, true };
    }

    double max_r = l1 + l2;
    double min_r = std::fabs(l1 - l2);
    bool reachable = true;
    if (r > max_r) {
        reachable = false;
        if (allow_unreachable) r = max_r;
        else return IKResult{0.0, 0.0, false};
    } else if (r < min_r) {
        reachable = false;
        if (allow_unreachable) r = min_r;
        else return IKResult{0.0, 0.0, false};
    }

    double cos_t2 = (r*r - l1*l1 - l2*l2) / (2.0 * l1 * l2);
    cos_t2 = clampd(cos_t2, -1.0, 1.0);
    double t2_abs = std::acos(cos_t2);

    double t2_cands[2] = { t2_abs, -t2_abs };
    double sols_t1[2];
    double sols_t2[2];

    for (int i = 0; i < 2; ++i) {
        double t2 = t2_cands[i];
        double s2 = std::sin(t2);
        double c2 = std::cos(t2);
        double k1 = l1 + l2 * c2;
        double k2 = l2 * s2;
        double t1 = angle - std::atan2(k2, k1);
        sols_t1[i] = t1;
        sols_t2[i] = t2;
    }

    int chosen_idx = 0;
    if (elbow_choice == 1) chosen_idx = 0;
    else if (elbow_choice == -1) chosen_idx = 1;
    else {
        // choose based on prev_angles if provided, otherwise default to elbow-up (index 0)
        if (prev_angles.has_value()) {
            double ph = prev_angles->first * deg2rad;
            double pk = prev_angles->second * deg2rad;
            double bestd = std::numeric_limits<double>::infinity();
            for (int i = 0; i < 2; ++i) {
                double dh = normalize_angle_rad(sols_t1[i] - ph);
                double dk = normalize_angle_rad(sols_t2[i] - pk);
                double d = dh*dh + dk*dk;
                if (d < bestd) { bestd = d; chosen_idx = i; }
            }
        } else {
            chosen_idx = 0;
        }
    }

    double hip_deg = sols_t1[chosen_idx] * rad2deg;
    double knee_deg = sols_t2[chosen_idx] * rad2deg;
    return IKResult{ hip_deg, knee_deg, reachable };
}

} // namespace facility
