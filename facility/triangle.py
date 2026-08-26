"""
TriangleLayout: equilateral triangular tiling utilities.

Tile coordinates are integer (col, row) pairs. Orientation alternates by parity:
- If (col + row) % 2 == 0 the triangle points "up", otherwise "down".
Center placement:
- side length = size
- height h = sqrt(3)/2 * size
- tile centers at: x = col * (size / 2), y = row * h
This layout makes neighboring triangles share edges.
"""

from __future__ import annotations
from dataclasses import dataclass
from typing import Tuple, List
import math

@dataclass
class TriangleLayout:
    size: float = 1.0                 # side length of each equilateral triangle
    spacing: float = 0.0              # fractional extra spacing (0.0 = touching)
    origin: Tuple[float, float] = (0.0, 0.0)
    # marker for generator detection
    is_triangle: bool = True

    def __post_init__(self):
        self.h = (math.sqrt(3.0) / 2.0) * self.size
        self._effective_size = self.size * (1.0 + self.spacing)
        self._effective_h = (math.sqrt(3.0) / 2.0) * self._effective_size

    def tile_center(self, tile: Tuple[int,int]) -> Tuple[float, float]:
        col, row = tile
        x = col * (self._effective_size / 2.0)
        y = row * self._effective_h
        ox, oy = self.origin
        return (ox + x, oy + y)

    def tile_orientation_up(self, tile: Tuple[int,int]) -> bool:
        col, row = tile
        return ((col + row) % 2) == 0

    def tile_corners(self, tile: Tuple[int,int]) -> List[Tuple[float,float]]:
        cx, cy = self.tile_center(tile)
        s = self._effective_size
        h = (math.sqrt(3.0) / 2.0) * s

        two_thirds = (2.0 * h) / 3.0
        one_third = h / 3.0

        if self.tile_orientation_up(tile):
            v_top = (cx + 0.0, cy + two_thirds)
            v_bl = (cx - s/2.0, cy - one_third)
            v_br = (cx + s/2.0, cy - one_third)
            return [v_top, v_bl, v_br]
        else:
            v_bottom = (cx + 0.0, cy - two_thirds)
            v_tr = (cx + s/2.0, cy + one_third)
            v_tl = (cx - s/2.0, cy + one_third)
            return [v_bottom, v_tr, v_tl]

    def rectangular_region(self, width: int, height: int, origin_col: int = 0, origin_row: int = 0) -> List[Tuple[int,int]]:
        out = []
        for col in range(origin_col, origin_col + width):
            for row in range(origin_row, origin_row + height):
                out.append((col, row))
        return out
