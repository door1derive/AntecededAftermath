#pragma once

#include "include/facility/objloader.hpp"
#include <vector>

namespace facility {

struct Plane {
    Vec3 p; // point on plane
    Vec3 n; // unit normal pointing inward (into triangle area)
};

// Build four planes for a triangle: surface plane and three side planes aligned with vertex normals.
// - mesh: source mesh with computed vertex normals (mesh.vnorms must be filled)
// - tri_idx: triangle index
// - side_planes: output vector of three side planes (edge planes), and surface plane
// Returns true on success.
bool build_triangle_planes(const Mesh& mesh, int tri_idx, Plane& surface_plane, std::vector<Plane>& side_planes);

// Test whether a point is considered "above" the triangle according to the 4-plane test.
// A small epsilon is used for stability (default 1e-6). The point should be slightly offset along
// the vertex normal before testing if you want to avoid exact-surface degeneracy.
bool point_in_triangle_facing_test(const Mesh& mesh, int tri_idx, const Vec3& point, double eps = 1e-6);

} // namespace facility
