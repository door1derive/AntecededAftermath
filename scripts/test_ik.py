"""
Simple tests comparing two_link_ik_degrees() against a few scenarios.

Run from the repository root:
  python -m scripts.test_ik
"""
from facility.ik import two_link_ik_degrees

cases = [
    # (angle_deg, r, l1, l2)
    (0.0, 6.0, 3.0, 3.0),   # fully extended along +X
    (0.0, 4.0, 3.0, 3.0),
    (45.0, 4.0, 3.0, 3.0),
    (120.0, 1.0, 1.2, 0.5), # near inner reach
    (270.0, 0.0, 1.0, 1.0), # zero distance
]

for ang, r, l1, l2 in cases:
    hip_deg, knee_deg, reachable = two_link_ik_degrees(ang, r, l1, l2, elbow_up=True, allow_unreachable=True)
    print(f"Case angle={ang} r={r} l1={l1} l2={l2}")
    print(f"  result -> hip={hip_deg:.6f}, knee={knee_deg:.6f}, reachable={reachable}")
    print()
