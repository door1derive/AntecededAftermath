"""
Quick example demonstrating importing facility.mathlib and using helpers.

Run from the project root:
  python -m scripts.example_floor

This script does not require Blender; it's a small test harness for mathlib.
"""

from facility import mathlib

def main():
    a = (0.0, 0.0)
    b = (1.0, 1.0)
    v = mathlib.vect(a, b)
    print("vect a->b:", v)
    print("length:", mathlib.mag(v))
    print("normalized:", mathlib.normalize(v))

if __name__ == "__main__":
    main()
