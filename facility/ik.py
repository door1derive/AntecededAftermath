"""
facility/ik.py

Two-link planar IK utilities with degree-based input/output.

Contains:
- two_link_ik_degrees(...)     -- a clearer, robust standard formulation (law of cosines).

The function returns (hip_deg, knee_deg, reachable).
"""
from __future__ import annotations
import math
from typing import Tuple, Optional


def _clamp(x: float, a: float, b: float) -> float:
    return max(a, min(b, x))


def _normalize_angle_rad(x: float) -> float:
    """Normalize radian angle to (-pi, pi]."""
    return (x + math.pi) % (2.0 * math.pi) - math.pi


def two_link_ik_degrees(angle_deg: float,
                        r: float,
                        l1: float,
                        l2: float,
                        elbow_up: Optional[bool] = True,
                        allow_unreachable: bool = True,
                        prev_angles: Optional[Tuple[float, float]] = None
                       ) -> Tuple[float, float, bool]:
    """
    Robust two-link planar IK with degree inputs/outputs using the law of cosines.

    Args:
      angle_deg: target direction in degrees (from +X, CCW positive).
      r: target distance (>= 0).
      l1, l2: link lengths (>0).
      elbow_up:
        - True: choose elbow-up (+acos) solution
        - False: choose elbow-down (-acos)
        - None: if prev_angles provided, pick solution closest to prev_angles; otherwise defaults to True
      allow_unreachable: if True clamp r into reachable range and return reachable=False; if False return reachable=False and no angles.
      prev_angles: optional (hip_deg, knee_deg) to choose continuous solution when elbow_up is None.

    Returns:
      (hip_deg, knee_deg, reachable)
        hip_deg: hip/base angle in degrees
        knee_deg: signed knee angle in degrees (positive = elbow_up direction)
        reachable: True if original r was reachable
    """
    if l1 <= 0.0 or l2 <= 0.0:
        raise ValueError("link lengths must be positive")

    angle = math.radians(angle_deg)

    # handle zero-distance target
    if r <= 0.0:
        # Hip points at desired direction; knee straight
        return angle_deg, 0.0, True

    max_r = l1 + l2
    min_r = abs(l1 - l2)
    reachable = True
    if r > max_r:
        reachable = False
        if allow_unreachable:
            r = max_r
        else:
            return 0.0, 0.0, False
    elif r < min_r:
        reachable = False
        if allow_unreachable:
            r = min_r
        else:
            return 0.0, 0.0, False

    # law of cosines for knee angle
    cos_t2 = (r * r - l1 * l1 - l2 * l2) / (2.0 * l1 * l2)
    cos_t2 = _clamp(cos_t2, -1.0, 1.0)
    t2_abs = math.acos(cos_t2)

    # two candidate knee angles (signed)
    t2_candidates = [t2_abs, -t2_abs]

    # compute corresponding hip angles for each candidate
    solutions = []
    for t2 in t2_candidates:
        s2 = math.sin(t2)
        c2 = math.cos(t2)
        k1 = l1 + l2 * c2
        k2 = l2 * s2
        t1 = angle - math.atan2(k2, k1)
        solutions.append((t1, t2))

    # choose solution based on elbow_up or prev_angles
    chosen = None
    if elbow_up is True:
        chosen = solutions[0]  # +t2_abs as 'elbow up' convention
    elif elbow_up is False:
        chosen = solutions[1]
    else:
        # elbow_up is None => choose solution closest to prev_angles if provided
        if prev_angles is not None:
            prev_hip_rad = math.radians(prev_angles[0])
            prev_knee_rad = math.radians(prev_angles[1])
            best = None
            best_dist = None
            for (t1_cand, t2_cand) in solutions:
                dh = _normalize_angle_rad(t1_cand - prev_hip_rad)
                dk = _normalize_angle_rad(t2_cand - prev_knee_rad)
                dist = dh * dh + dk * dk
                if best is None or dist < best_dist:
                    best = (t1_cand, t2_cand)
                    best_dist = dist
            chosen = best
        else:
            # default to elbow_up True
            chosen = solutions[0]

    t1, t2 = chosen
    hip_deg = math.degrees(t1)
    knee_deg = math.degrees(t2)
    return hip_deg, knee_deg, reachable
