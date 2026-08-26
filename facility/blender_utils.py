"""
Blender utility helpers.

Keep all direct `bpy` usage here so other modules (scripts/add-ons) can remain
testable and importable outside Blender. This module uses facility.blender_adaptor
to check for bpy availability and provide helpful runtime errors when used outside Blender.

Functions provided (small, extendable set):
- create_mesh_object(name, verts, faces) -> bpy.Object
- link_object(obj, collection=None)
- set_active_object(obj)
- create_material(name, color=(r,g,b,a))
- assign_material(obj, material)
- remove_object(obj)  # safe unlink and delete
- safe_edit_toggle()
- apply_scale()
- bevel_vertices(offset)
- get_verts_edges_polys(obj=None)
- select_object(obj_or_name, active=True, deselect_others=True)

"""
from typing import List, Tuple, Optional
from .blender_adaptor import get_bpy, ensure_bpy


def create_mesh_object(name: str, verts: List[Tuple[float, float, float]], faces: List[Tuple[int, ...]]):
    """
    Create a new mesh object from plain python lists of verts and faces and link it
    into the active collection. Returns the created object.

    verts: list of (x, y, z)
    faces: list of tuples of vertex indices (n-gons allowed)
    """
    bpy = get_bpy()
    ensure_bpy()
    mesh = bpy.data.meshes.new(f"{name}_Mesh")
    mesh.from_pydata(verts, [], faces)
    mesh.update()

    obj = bpy.data.objects.new(name, mesh)
    collection = bpy.context.collection
    collection.objects.link(obj)
    # set active and select only this object
    bpy.context.view_layer.objects.active = obj
    for o in bpy.context.view_layer.objects:
        o.select_set(False)
    obj.select_set(True)
    return obj


def link_object(obj, collection=None):
    """Link an existing object to the given collection (or active collection if None)."""
    bpy = get_bpy()
    ensure_bpy()
    if collection is None:
        collection = bpy.context.collection
    if obj.name not in collection.objects:
        collection.objects.link(obj)


def set_active_object(obj):
    """Set given object as active and select it."""
    bpy = get_bpy()
    ensure_bpy()
    bpy.context.view_layer.objects.active = obj
    for o in bpy.context.view_layer.objects:
        o.select_set(False)
    obj.select_set(True)


def create_material(name: str, color: Tuple[float, float, float, float] = (1.0, 1.0, 1.0, 1.0), use_nodes: bool = True):
    """Create (or reuse) a material and return it. color is RGBA 0..1."""
    bpy = get_bpy()
    ensure_bpy()
    mat = bpy.data.materials.get(name)
    if mat is None:
        mat = bpy.data.materials.new(name=name)
    if use_nodes:
        mat.use_nodes = True
        nodes = mat.node_tree.nodes
        principled = nodes.get("Principled BSDF")
        if principled is None:
            principled = nodes.new("ShaderNodeBsdfPrincipled")
            principled.location = (0, 0)
        principled.inputs["Base Color"].default_value = color
    else:
        mat.diffuse_color = color
    return mat


def assign_material(obj, material):
    """Assign material to object (replace existing materials). `material` can be name or material object."""
    bpy = get_bpy()
    ensure_bpy()
    mat = material
    if isinstance(material, str):
        mat = bpy.data.materials.get(material)
        if mat is None:
            raise ValueError(f"Material named '{material}' not found")
    if obj.data is None:
        raise ValueError("Object has no data to assign material to")
    obj.data.materials.clear()
    obj.data.materials.append(mat)


def remove_object(obj):
    """Unlink and delete an object from the current scene/collection."""
    bpy = get_bpy()
    ensure_bpy()
    for coll in list(obj.users_collection):
        coll.objects.unlink(obj)
    if bpy.context.view_layer.objects.active == obj:
        bpy.context.view_layer.objects.active = None
    bpy.data.objects.remove(obj, do_unlink=True)


def safe_edit_toggle():
    """Toggle edit mode (enter/exit) safely."""
    bpy = get_bpy()
    ensure_bpy()
    bpy.ops.object.editmode_toggle()


def apply_scale():
    bpy = get_bpy()
    ensure_bpy()
    bpy.ops.object.transform_apply(location = False, rotation = False, scale = True)


def bevel_vertices(offset = 0.5):
    bpy = get_bpy()
    ensure_bpy()
    safe_edit_toggle()
    bpy.ops.mesh.bevel(offset = offset, offset_pct=0, affect='VERTICES')
    safe_edit_toggle()


def get_verts_edges_polys(obj=None):
    bpy = get_bpy()
    ensure_bpy()
    if obj is None:
        obj = bpy.context.object
    ret_verts = [(v.co.x, v.co.y, v.co.z) for v in obj.data.vertices]
    ret_edges = [e.key for e in obj.data.edges]
    ret_polys = [tuple(p.vertices) for p in obj.data.polygons]
    return ret_verts, ret_edges, ret_polys


def select_object(obj_or_name, active=True, deselect_others=True):
    bpy = get_bpy()
    ensure_bpy()
    if isinstance(obj_or_name, str):
        obj = bpy.data.objects.get(obj_or_name)
    else:
        obj = obj_or_name
    if obj is None:
        raise ValueError("Object not found for selection")
    if deselect_others:
        for o in bpy.context.view_layer.objects:
            o.select_set(False)
    obj.select_set(True)
    if active:
        bpy.context.view_layer.objects.active = obj
