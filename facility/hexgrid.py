"""
Hexagonal grid utilities (pure Python, no Blender dependency).

Design choices:
- Axial coordinates (q, r) for hex addressing.
- World coordinates are (x, y). Convention: 0° = +X (standard math).
- Support pointy-top (flat_top=False) and flat-top (flat_top=True).
- `size` is center->vertex distance; `spacing` is fractional extra gap between tiles.
"""

from __future__ import annotations
from dataclasses import dataclass
from typing import Tuple, List, Iterable, Optional
import math

@dataclass(frozen=True)
class Axial:
    q: int
    r: int

    def to_cube(self) -> Tuple[int, int, int]:
        x = self.q
        z = self.r
        y = -x - z
        return (x, y, z)

    @staticmethod
    def from_cube(x: int, y: int, z: int) -> "Axial":
        return Axial(q=x, r=z)

    def neighbors(self) -> List["Axial"]:
        deltas = [(+1, 0), (+1, -1), (0, -1), (-1, 0), (-1, +1), (0, +1)]
        return [Axial(self.q + dq, self.r + dr) for dq, dr in deltas]


def cube_round(x: float, y: float, z: float) -> Tuple[int, int, int]:
    rx = round(x); ry = round(y); rz = round(z)
    x_diff = abs(rx - x); y_diff = abs(ry - y); z_diff = abs(rz - z)
    if x_diff > y_diff and x_diff > z_diff:
        rx = -ry - rz
    elif y_diff > z_diff:
        ry = -rx - rz
    else:
        rz = -rx - ry
    return (int(rx), int(ry), int(rz))


def lerp(a: float, b: float, t: float) -> float:
    return a + (b - a) * t


def cube_lerp(a: Tuple[int,int,int], b: Tuple[int,int,int], t: float):
    return (lerp(a[0], b[0], t), lerp(a[1], b[1], t), lerp(a[2], b[2], t))

@dataclass
class Layout:
    size: float = 1.0                 # center -> vertex distance
    spacing: float = 0.0              # fractional extra spacing (0.0 = touching)
    origin: Tuple[float, float] = (0.0, 0.0)
    start_angle_deg: float = 0.0      # 0° at +X (conventional)
    corner_count: int = 6             # vertices per regular polygon (6 for hex)
    ccw: bool = True
    flat_top: bool = False            # False => pointy-top, True => flat-top

    def __post_init__(self):
        self._effective_size = self.size * (1.0 + self.spacing)
        self._corner_unit_vectors = self._compute_corner_unit_vectors()

    def _compute_corner_unit_vectors(self) -> List[Tuple[float, float]]:
        vecs = []
        sign = 1.0 if self.ccw else -1.0
        for i in range(self.corner_count):
            angle_deg = self.start_angle_deg + sign * (360.0 * i / self.corner_count)
            a = math.radians(angle_deg)
            vx = math.cos(a); vy = math.sin(a)
            vecs.append((vx, vy))
        return vecs

    def hex_to_world(self, h: Axial) -> Tuple[float, float]:
        """
        Convert axial (q, r) to world (x, y).
        Uses standard axial-to-pixel formulas for pointy or flat tops.
        """
        k = self._effective_size / self.size if self.size != 0 else 1.0
        if not self.flat_top:
            x = (math.sqrt(3) * (h.q + h.r / 2.0)) * self.size * k
            y = (3.0 / 2.0 * h.r) * self.size * k
        else:
            x = (3.0 / 2.0 * h.q) * self.size * k
            y = (math.sqrt(3) * (h.r + h.q / 2.0)) * self.size * k
        ox, oy = self.origin
        return (ox + x, oy + y)

    def world_to_hex(self, x: float, y: float) -> Axial:
        ox, oy = self.origin
        k = self._effective_size / self.size if self.size != 0 else 1.0
        px = (x - ox) / (self.size * k)
        py = (y - oy) / (self.size * k)
        if not self.flat_top:
            q = (math.sqrt(3)/3 * px - 1.0/3 * py)
            r = (2.0/3 * py)
        else:
            q = (2.0/3 * px)
            r = (math.sqrt(3)/3 * py - 1.0/3 * px)
        x_c = q; z_c = r; y_c = -x_c - z_c
        rx, ry, rz = cube_round(x_c, y_c, z_c)
        return Axial.from_cube(rx, ry, rz)

    def hex_corners(self, h: Axial) -> List[Tuple[float, float]]:
        cx, cy = self.hex_to_world(h)
        return [(cx + vx * self.size, cy + vy * self.size) for (vx, vy) in self._corner_unit_vectors]

    def hex_edge_centers(self, h: Axial) -> List[Tuple[float, float]]:
        corners = self.hex_corners(h)
        n = len(corners)
        return [((corners[i][0] + corners[(i+1)%n][0]) / 2.0, (corners[i][1] + corners[(i+1)%n][1]) / 2.0) for i in range(n)]

    def hex_keypoint(self, h: Axial, key: str = "center", index: Optional[int] = None) -> Tuple[float, float]:
        if key in ("center", "c"):
            return self.hex_to_world(h)
        if key in ("vertex", "corner", "v"):
            if index is None:
                raise ValueError("vertex index required when key='vertex'")
            return self.hex_corners(h)[index % self.corner_count]
        if key in ("edge", "edge_center", "e"):
            if index is None:
                raise ValueError("edge index required when key='edge'")
            return self.hex_edge_centers(h)[index % self.corner_count]
        raise ValueError(f"unknown keypoint '{key}'")

    def linedraw(self, a: Axial, b: Axial) -> List[Axial]:
        ac = a.to_cube(); bc = b.to_cube()
        N = max(abs(ac[0]-bc[0]), abs(ac[1]-bc[1]), abs(ac[2]-bc[2]))
        if N == 0:
            return [a]
        results: List[Axial] = []
        for i in range(N + 1):
            t = 0.0 if N == 0 else (i / float(N))
            cx, cy, cz = cube_lerp(ac, bc, t)
            rx, ry, rz = cube_round(cx, cy, cz)
            results.append(Axial.from_cube(rx, ry, rz))
        return results

    def polyline_from_keypoints(self, chain: Iterable[Tuple[Axial, str, Optional[int]]]) -> List[Tuple[float, float]]:
        pts: List[Tuple[float, float]] = []
        for h, key, idx in chain:
            if key in ("center", "c"):
                pts.append(self.hex_to_world(h))
            elif key in ("vertex", "corner", "v"):
                if idx is None: raise ValueError("vertex key requires index")
                pts.append(self.hex_keypoint(h, "vertex", idx))
            elif key in ("edge", "edge_center", "e"):
                if idx is None: raise ValueError("edge key requires index")
                pts.append(self.hex_keypoint(h, "edge", idx))
            else:
                raise ValueError(f"unknown key '{key}' in chain")
        return pts

    def axial_rectangle(self, q_min: int, q_max: int, r_min: int, r_max: int) -> List[Axial]:
        return [Axial(q, r) for q in range(q_min, q_max+1) for r in range(r_min, r_max+1)]

    # -----------------------
    # Offset coordinate helpers for rectangular honeycomb blocks
    # -----------------------
    @staticmethod
    def _offset_to_axial_col_row(col: int, row: int, odd: bool = True) -> Axial:
        r = row - ((col - (col & 1)) // 2) if odd else row - ((col + (col & 1)) // 2)
        q = col
        return Axial(q, r)

    @staticmethod
    def _offset_to_axial_row_col(col: int, row: int, odd: bool = True) -> Axial:
        q = col - ((row - (row & 1)) // 2) if odd else col - ((row + (row & 1)) // 2)
        r = row
        return Axial(q, r)

    def rectangular_region(self, width: int, height: int, origin_col: int = 0, origin_row: int = 0, odd: bool = True) -> List[Axial]:
        out: List[Axial] = []
        if not self.flat_top:
            for col in range(origin_col, origin_col + width):
                for row in range(origin_row, origin_row + height):
                    out.append(self._offset_to_axial_col_row(col, row, odd=odd))
        else:
            for row in range(origin_row, origin_row + height):
                for col in range(origin_col, origin_col + width):
                    out.append(self._offset_to_axial_row_col(col, row, odd=odd))
        return out

    def hex_ring(self, center: Axial, radius: int) -> List[Axial]:
        if radius == 0:
            return [center]
        results: List[Axial] = []
        cube = center.to_cube()
        directions_cube = [(+1, -1, 0), (+1, 0, -1), (0, +1, -1), (-1, +1, 0), (-1, 0, +1), (0, -1, +1)]
        x = cube[0] + directions_cube[4][0] * radius
        y = cube[1] + directions_cube[4][1] * radius
        z = cube[2] + directions_cube[4][2] * radius
        for side in range(6):
            for step in range(radius):
                results.append(Axial.from_cube(x, y, z))
                dx, dy, dz = directions_cube[side]
                x += dx; y += dy; z += dz
        return results
