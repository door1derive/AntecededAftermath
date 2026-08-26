"""
Read a simple command file describing primitives to place and generate an OBJ floor.

Command syntax (one per line):
  add_triangle(scale=10.0, bevel=0.25, orientation_deg=-90, translate=(0,0,0), name='tri1')

Supported commands (initial):
  - add_triangle(...)

The script collects geometry from commands and writes a single OBJ using facility.obj_generator.

Usage:
  python -m scripts.command_floor --commands examples/floor_commands.txt --out docs/models/command_floor.obj

This is intentionally small; we can extend with more primitives and transforms later.
"""

from __future__ import annotations
import argparse
import ast
import math
from typing import List, Tuple, Dict

from facility.obj_generator import write_obj_from_tiles


def parse_command_line(line: str):
    """Parse a line like `add_triangle(scale=10.0, bevel=0.25)` into (cmd_name, kwargs)
    Uses ast to safely parse literals.
    """
    try:
        node = ast.parse(line, mode='eval').body
    except Exception as e:
        raise RuntimeError(f"Failed to parse line: {line!r}: {e}")
    if not isinstance(node, ast.Call):
        raise RuntimeError(f"Line is not a function call: {line!r}")
    # function name
    if isinstance(node.func, ast.Name):
        cmd_name = node.func.id
    else:
        raise RuntimeError(f"Unsupported function target in line: {line!r}")
    kwargs = {}
    for kw in node.keywords:
        if kw.arg is None:
            raise RuntimeError("Positional args not supported; use keyword args only")
        try:
            kwargs[kw.arg] = ast.literal_eval(kw.value)
        except Exception as e:
            raise RuntimeError(f"Failed to evaluate value for {kw.arg}: {e}")
    return cmd_name, kwargs


# ---------- geometry helpers ----------

def triangle_unit_vertices_pointing_up():
    """Return 3 2D vertices (x,y) for an equilateral triangle of side length 1 centered at origin and pointing +Y."""
    s = 1.0
    h = math.sqrt(3.0) / 2.0 * s
    # centroid at origin: vertices at (0, 2h/3), (-s/2, -h/3), (s/2, -h/3)
    v0 = (0.0, 2.0 * h / 3.0)
    v1 = (-s / 2.0, -h / 3.0)
    v2 = (s / 2.0, -h / 3.0)
    return [v0, v1, v2]


def rotate_point(p: Tuple[float, float], deg: float) -> Tuple[float, float]:
    a = math.radians(deg)
    ca = math.cos(a); sa = math.sin(a)
    x, y = p
    return (ca * x - sa * y, sa * x + ca * y)


def scale_points(points: List[Tuple[float, float]], scale: float) -> List[Tuple[float, float]]:
    return [(x * scale, y * scale) for (x, y) in points]


def translate_points(points: List[Tuple[float, float]], tr: Tuple[float, float]) -> List[Tuple[float, float]]:
    tx, ty = tr
    return [(x + tx, y + ty) for (x, y) in points]


def beveled_polygon_from_convex(points: List[Tuple[float, float]], bevel: float) -> List[Tuple[float, float]]:
    """Given a convex polygon (CCW vertex order), return a new polygon with corners cut off by 'bevel' length along edges.
    For each original vertex v_i with neighbors v_prev and v_next, compute points along edges from v_i towards v_prev and v_next
    at distance 'bevel' from v_i, and use those points instead of the sharp corner.

    Note: bevel is absolute distance. If bevel is too large relative to edge lengths it will be clamped.
    """
    n = len(points)
    if bevel <= 0.0:
        return points[:]
    out: List[Tuple[float, float]] = []
    for i in range(n):
        v = points[i]
        v_prev = points[(i - 1) % n]
        v_next = points[(i + 1) % n]
        # vector from v to neighbor
        def vec(a, b):
            return (b[0] - a[0], b[1] - a[1])
        def length(u):
            return math.hypot(u[0], u[1])
        def norm(u):
            L = length(u)
            if L == 0: return (0.0, 0.0)
            return (u[0] / L, u[1] / L)
        e1 = vec(v, v_prev)  # toward prev
        e2 = vec(v, v_next)  # toward next
        l1 = length(e1)
        l2 = length(e2)
        # clamp bevel to half-edge
        b1 = min(bevel, l1 * 0.4999)
        b2 = min(bevel, l2 * 0.4999)
        n1 = norm(e1)
        n2 = norm(e2)
        p1 = (v[0] + n1[0] * b1, v[1] + n1[1] * b1)
        p2 = (v[0] + n2[0] * b2, v[1] + n2[1] * b2)
        # order: p2 then p1 to keep polygon CCW (since p2 is along edge to next)
        out.append(p2)
        out.append(p1)
    return out


# ---------- scene assembly ----------

class SceneBuilder:
    def __init__(self):
        self.verts: List[Tuple[float, float, float]] = []
        self.faces: List[Tuple[int, ...]] = []
        # simple dedupe map keyed by rounded position
        self._vmap: Dict[Tuple[int, int, int], int] = {}

    def _add_vertex(self, v: Tuple[float, float, float]) -> int:
        key = (int(round(v[0] * 1000000)), int(round(v[1] * 1000000)), int(round(v[2] * 1000000)))
        if key in self._vmap:
            return self._vmap[key]
        idx = len(self.verts)
        self.verts.append(v)
        self._vmap[key] = idx
        return idx

    def add_polygon(self, points2d: List[Tuple[float, float]], z: float = 0.0):
        # assumes CCW points2d
        idxs = []
        for (x, y) in points2d:
            idxs.append(self._add_vertex((x, y, z)))
        self.faces.append(tuple(idxs))

    def write_obj(self, path: str, obj_name: str = 'Scene'):
        write_obj_from_tiles(self.verts, self.faces, path, obj_name=obj_name, mtl_name=None)


# ---------- command handlers ----------

def handle_add_triangle(builder: SceneBuilder, **kwargs):
    # supported kwargs: scale (float), bevel (float), orientation_deg (float), translate (tuple x,y,z), name
    scale = float(kwargs.get('scale', 1.0))
    bevel = float(kwargs.get('bevel', 0.0))
    orientation_deg = float(kwargs.get('orientation_deg', -90.0))  # default face +X
    translate = kwargs.get('translate', (0.0, 0.0, 0.0))
    if isinstance(translate, (list, tuple)):
        tx, ty, tz = translate[0], translate[1], translate[2] if len(translate) > 2 else 0.0
    else:
        tx = ty = tz = 0.0

    base = triangle_unit_vertices_pointing_up()
    pts = scale_points(base, scale)
    pts = [rotate_point(p, orientation_deg) for p in pts]
    pts = translate_points(pts, (tx, ty))
    if bevel > 0:
        pts = beveled_polygon_from_convex(pts, bevel)
    builder.add_polygon(pts, z=tz)


COMMAND_HANDLERS = {
    'add_triangle': handle_add_triangle,
}


def run_command_file(path: str, out: str):
    builder = SceneBuilder()
    with open(path, 'r') as f:
        lines = f.readlines()
    for raw in lines:
        line = raw.strip()
        if not line or line.startswith('#'):
            continue
        cmd_name, kwargs = parse_command_line(line)
        handler = COMMAND_HANDLERS.get(cmd_name)
        if handler is None:
            raise RuntimeError(f"Unknown command: {cmd_name}")
        handler(builder, **kwargs)
    builder.write_obj(out)
    print(f"Wrote OBJ to {out}; verts={len(builder.verts)} faces={len(builder.faces)}")


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--commands', default='examples/floor_commands.txt')
    p.add_argument('--out', default='docs/models/command_floor.obj')
    args = p.parse_args()
    run_command_file(args.commands, args.out)


if __name__ == '__main__':
    main()
