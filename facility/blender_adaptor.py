"""
Small adapter to centralize Blender imports and provide helpful errors.
Importing this module outside Blender will not fatal-crash the rest of your code;
calls that require bpy will raise a runtime error with guidance.
"""

try:
    import bpy
    from mathutils import Vector, Color
except Exception:
    bpy = None
    Vector = None
    Color = None


def ensure_bpy():
    if bpy is None:
        raise RuntimeError(
            "Blender 'bpy' not available. This operation must be run inside Blender's bundled Python."
        )


def get_bpy():
    ensure_bpy()
    return bpy
