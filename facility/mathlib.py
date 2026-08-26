"""
Small procedural math library with vector helpers.

This intentionally uses simple procedural functions (not OOP) so you can call:
  from facility import mathlib
  mathlib.vect(a, b)
and step through code one function at a time.

Functions:
- vect(a, b): returns vector from a to b (b - a)
- add(a, b): vector addition
- add_inplace(a, b): returns new tuple sum (keeps style simple)
- scale(v, s): multiply vector by scalar
- dot(a, b)
- mag(v)
- normalize(v)
- cross3d(a, b): 3D cross product
- rotate2d(v, degrees, clockwise=False)
- avg(vlist): average of vectors
- approx_equal(a, b, tol=1e-6): scalar approx equality
"""

from __future__ import annotations
from typing import Tuple, Iterable, List
import math

Vector = Tuple[float, ...]


def vect(a: Vector, b: Vector) -> Vector:
    """Return vector b - a (component-wise). Vectors may be 2D or 3D."""
    return tuple(b[i] - a[i] for i in range(min(len(a), len(b))))


def add(a: Vector, b: Vector) -> Vector:
    """Component-wise addition (works for equal-length vectors)."""
    length = max(len(a), len(b))
    return tuple((a[i] if i < len(a) else 0.0) + (b[i] if i < len(b) else 0.0) for i in range(length))


def scale(v: Vector, s: float) -> Vector:
    """Scale vector by scalar s."""
    return tuple(vi * s for vi in v)


def dot(a: Vector, b: Vector) -> float:
    """Dot product (works for equal-length vectors)."""
    return sum((a[i] if i < len(a) else 0.0) * (b[i] if i < len(b) else 0.0) for i in range(max(len(a), len(b))))


def mag(v: Vector) -> float:
    """Magnitude / length of vector."""
    return math.sqrt(dot(v, v))


def normalize(v: Vector, eps: float = 1e-9) -> Vector:
    """Return normalized vector; if length is zero return zeros of same dimension."""
    m = mag(v)
    if m <= eps:
        return tuple(0.0 for _ in v)
    return tuple(vi / m for vi in v)


def cross3d(a: Vector, b: Vector) -> Vector:
    """3D cross product. If vectors shorter than 3, missing components are treated as 0."""
    ax, ay, az = (a + (0.0, 0.0, 0.0))[:3]
    bx, by, bz = (b + (0.0, 0.0, 0.0))[:3]
    return (ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx)


def rotate2d(v: Vector, degrees: float, clockwise: bool = False) -> Vector:
    """Rotate a 2D vector by degrees. If v has more than 2 components, z is preserved."""
    angle = math.radians(degrees)
    if clockwise:
        angle = -angle
    x, y = (v[0] if len(v) > 0 else 0.0, v[1] if len(v) > 1 else 0.0)
    cx = math.cos(angle)
    sx = math.sin(angle)
    rx = cx * x - sx * y
    ry = sx * x + cx * y
    if len(v) > 2:
        return (rx, ry) + tuple(v[2:])
    return (rx, ry)


def avg(vlist: Iterable[Vector]) -> Vector:
    """Average a list of equal-length vectors. Returns vector with same dimension as first item."""
    vlist = list(vlist)
    if not vlist:
        return ()
    dim = len(vlist[0])
    sums = [0.0] * dim
    for v in vlist:
        for i in range(dim):
            sums[i] += (v[i] if i < len(v) else 0.0)
    n = len(vlist)
    return tuple(s / n for s in sums)


def approx_equal(a: float, b: float, tol: float = 1e-6) -> bool:
    """Return whether two floats are approximately equal."""
    return abs(a - b) <= tol
