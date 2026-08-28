#pragma once

#include <string>
#include <vector>
#include <array>

namespace facility {

struct Vec3 {
    double x,y,z;
    Vec3() : x(0),y(0),z(0) {}
    Vec3(double X,double Y,double Z) : x(X),y(Y),z(Z) {}
};

struct Mesh {
    std::vector<Vec3> verts;               // vertex positions (0-based)
    std::vector<std::array<int,3>> tris;   // triangles (indices into verts)
    std::vector<Vec3> vnorms;             // per-vertex normals (computed)
};

// Load a minimal subset of OBJ: supports lines beginning with:
//  v x y z    (vertex)
//  f i j k ... (face indices; supports polygons and many index formats: v, v/vt, v//vn, v/vt/vn)
// - if convert_y_up_to_z_up is true, positions are rotated from Y-up to Z-up (x,y,z) -> (x,z,-y)
// Returns true on success and fills mesh. mesh.vnorms is left empty unless compute_vertex_normals is called.
bool load_obj(const std::string& path, Mesh& mesh, std::string* out_error = nullptr, bool convert_y_up_to_z_up = false);

// Compute per-vertex normals by accumulating area-weighted face normals.
// Optionally normalize to unit length (default true).
// After this call mesh.vnorms.size() == mesh.verts.size() and is valid.
void compute_vertex_normals(Mesh& mesh, bool normalize = true);

} // namespace facility
