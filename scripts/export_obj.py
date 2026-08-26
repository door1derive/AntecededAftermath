"""
Export a simple OBJ file from the facility generator.

Usage (from project root):
  python -m scripts.export_obj --shape triangle --width 6 --height 4 --out docs/models/model.obj

This script imports the facility package and uses the obj_generator helper to produce an OBJ.
"""

from __future__ import annotations
import argparse
from typing import List, Tuple


def build_triangles(width: int, height: int, size: float = 1.0, spacing: float = 0.0, origin_col: int = 0, origin_row: int = 0, out_path: str = 'docs/models/generated_triangle.obj'):
    from facility.triangle import TriangleLayout
    from facility.obj_generator import write_obj_from_layout
    L = TriangleLayout(size=size, spacing=spacing, origin=(0.0, 0.0))
    tiles = L.rectangular_region(width=width, height=height, origin_col=origin_col, origin_row=origin_row)
    write_obj_from_layout(L, tiles, out_path, obj_name='TriangleFloor')


def build_hexes(width: int, height: int, size: float = 1.0, spacing: float = 0.0, origin_col: int = 0, origin_row: int = 0, out_path: str = 'docs/models/generated_hex.obj'):
    from facility.hexgrid import Layout as HexLayout
    from facility.obj_generator import write_obj_from_layout
    L = HexLayout(size=size, spacing=spacing, start_angle_deg=0.0, flat_top=False)
    tiles = L.rectangular_region(width=width, height=height, origin_col=origin_col, origin_row=origin_row, odd=True)
    write_obj_from_layout(L, tiles, out_path, obj_name='HexFloor')


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--shape', choices=('triangle', 'hex'), default='triangle')
    p.add_argument('--width', type=int, default=6)
    p.add_argument('--height', type=int, default=4)
    p.add_argument('--size', type=float, default=1.0)
    p.add_argument('--spacing', type=float, default=0.0)
    p.add_argument('--out', type=str, default='docs/models/model.obj')
    args = p.parse_args()

    if args.shape == 'triangle':
        build_triangles(args.width, args.height, size=args.size, spacing=args.spacing, out_path=args.out)
    else:
        build_hexes(args.width, args.height, size=args.size, spacing=args.spacing, out_path=args.out)

    print('Export complete: ' + args.out)

if __name__ == '__main__':
    main()
