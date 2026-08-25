"""
Tile/generic floor generator that supports hex and triangle layouts and can optionally
segment hex tiles into wedge pieces leaving a central hole.
"""
from typing import Iterable, List, Tuple, Dict
from .hexgrid import Axial, Layout as HexLayout
from .blender_adaptor import get_bpy, ensure_bpy
import math

EPS = 1e-6

class HexFloorGenerator:
    def __init__(self, layout):
        self.layout = layout

    def _collect_vertices_and_faces(self,
                                    tiles: Iterable,
                                    segmented: bool = False,
                                    inner_ratio: float = 0.4
                                    ) -> Tuple[List[Tuple[float,float,float]], List[Tuple[int,...]]]:
        """
        Generic tile collector:
        - If layout has attribute 'is_triangle' truthy, assume tiles are (col,row) tuples and call layout.tile_corners(tile).
        - Otherwise assume tiles are Axial and call layout.hex_corners(tile).
        Supports segmented hex mode where each hex is split into 6 wedge quads around an inner hex.
        """
        inner_ratio = max(0.0, min(0.9999, float(inner_ratio)))

        vert_index: Dict[Tuple[int,int], int] = {}
        verts: List[Tuple[float,float,float]] = []
        faces: List[Tuple[int,...]] = []

        def add_vert(x: float, y: float) -> int:
            key = (int(round(x / (EPS*100))), int(round(y / (EPS*100))))
            if key in vert_index:
                return vert_index[key]
            idx = len(verts)
            verts.append((x, y, 0.0))
            vert_index[key] = idx
            return idx

        is_triangle_layout = getattr(self.layout, "is_triangle", False)

        for t in tiles:
            if is_triangle_layout:
                corners2d = self.layout.tile_corners(t)
                face_indices = tuple(add_vert(x, y) for (x,y) in corners2d)
                faces.append(face_indices)
            else:
                outer = self.layout.hex_corners(t)
                if not segmented or inner_ratio <= 0.0:
                    face_indices = tuple(add_vert(x, y) for (x,y) in outer)
                    faces.append(face_indices)
                else:
                    n = len(outer)
                    # build inner vertices at half-step angles
                    inner = []
                    start = getattr(self.layout, "start_angle_deg", 0.0)
                    sign = 1.0 if getattr(self.layout, "ccw", True) else -1.0
                    # compute inner vertices
                    for i in range(n):
                        angle_deg = start + sign * (360.0 * (i + 0.5) / n)
                        a = math.radians(angle_deg)
                        ux = math.cos(a); uy = math.sin(a)
                        # center of this hex
                        cx = self.layout.hex_to_world(t)[0]
                        cy = self.layout.hex_to_world(t)[1]
                        ix = cx + ux * (self.layout.size * inner_ratio)
                        iy = cy + uy * (self.layout.size * inner_ratio)
                        inner.append((ix, iy))

                    # create wedge quads
                    for i in range(len(outer)):
                        o0 = outer[i]
                        o1 = outer[(i + 1) % len(outer)]
                        ii0 = inner[i]
                        ii1 = inner[(i + 1) % len(inner)]
                        face = (
                            add_vert(o0[0], o0[1]),
                            add_vert(o1[0], o1[1]),
                            add_vert(ii1[0], ii1[1]),
                            add_vert(ii0[0], ii0[1]),
                        )
                        faces.append(face)

        return verts, faces

    def create_floor_object(self,
                            tiles: Iterable,
                            name: str = "TileFloor",
                            segmented: bool = False,
                            inner_ratio: float = 0.4):
        """
        Create a mesh object in the current Blender scene.
        """
        bpy = get_bpy()
        ensure_bpy()
        verts, faces = self._collect_vertices_and_faces(tiles, segmented=segmented, inner_ratio=inner_ratio)

        mesh = bpy.data.meshes.new(f"{name}_Mesh")
        mesh.from_pydata(verts, [], faces)
        mesh.update()

        obj = bpy.data.objects.new(name, mesh)
        collection = bpy.context.collection
        collection.objects.link(obj)
        bpy.context.view_layer.objects.active = obj
        obj.select_set(True)
        return obj

    def create_floor_rectangle(self, q_min:int, q_max:int, r_min:int, r_max:int, name="TileFloor", segmented: bool = False, inner_ratio: float = 0.4):
        # convenience for axial ranges (hex only)
        tiles = self.layout.axial_rectangle(q_min, q_max, r_min, r_max)
        return self.create_floor_object(tiles, name=name, segmented=segmented, inner_ratio=inner_ratio)
