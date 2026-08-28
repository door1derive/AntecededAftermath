#include "include/facility/path_utils.hpp"
#include <cmath>

namespace facility {

static Vec3 sub(const Vec3& a, const Vec3& b) { return Vec3(a.x-b.x, a.y-b.y, a.z-b.z); }
static Vec3 add(const Vec3& a, const Vec3& b) { return Vec3(a.x+b.x, a.y+b.y, a.z+b.z); }
static Vec3 mul(const Vec3& a, double s) { return Vec3(a.x*s, a.y*s, a.z*s); }
static double dot(const Vec3& a, const Vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static Vec3 cross(const Vec3& a, const Vec3& b) { return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }
static double len(const Vec3& a) { return std::sqrt(dot(a,a)); }
static Vec3 normalize_or(const Vec3& a, const Vec3& fallback) { double L = len(a); return (L>0.0) ? mul(a,1.0/L) : fallback; }

bool build_triangle_planes(const Mesh& mesh, int tri_idx, Plane& surface_plane, std::vector<Plane>& side_planes) {
    if (tri_idx < 0 || tri_idx >= static_cast<int>(mesh.tris.size())) return false;
    auto t = mesh.tris[tri_idx];
    Vec3 v0 = mesh.verts[t[0]];
    Vec3 v1 = mesh.verts[t[1]];
    Vec3 v2 = mesh.verts[t[2]];
    // face normal
    Vec3 fn = cross(sub(v1,v0), sub(v2,v0));
    fn = normalize_or(fn, Vec3(0,0,1));
    surface_plane.p = v0;
    surface_plane.n = fn; // points outward from plane; caller may want inside as >= 0 above

    // ensure vertex normals exist
    if (mesh.vnorms.size() != mesh.verts.size()) return false;

    side_planes.clear();
    side_planes.reserve(3);
    // edges: (v0,v1,n0+n1), (v1,v2,n1+n2), (v2,v0,n2+n0)
    std::array<std::pair<int,int>,3> edges = {{{{t[0],t[1]}},{{t[1],t[2]}},{{t[2],t[0]}}}};
    for (int ei = 0; ei < 3; ++ei) {
        int ia = edges[ei].first;
        int ib = edges[ei].second;
        Vec3 A = mesh.verts[ia];
        Vec3 B = mesh.verts[ib];
        Vec3 edge_dir = sub(B, A);
        edge_dir = normalize_or(edge_dir, Vec3(1,0,0));
        Vec3 navg = add(mesh.vnorms[ia], mesh.vnorms[ib]);
        navg = normalize_or(navg, fn);
        Vec3 side_n = cross(edge_dir, navg);
        side_n = normalize_or(side_n, fn);
        // we want side_n to point inward (so that dot(side_n, centroid - A) >= 0), check and flip if needed
        Vec3 centroid = mul(add(add(v0,v1),v2), 1.0/3.0);
        double test = dot(side_n, sub(centroid, A));
        if (test < 0.0) {
            side_n = mul(side_n, -1.0);
        }
        Plane pl; pl.p = A; pl.n = side_n;
        side_planes.push_back(pl);
    }

    return true;
}

bool point_in_triangle_facing_test(const Mesh& mesh, int tri_idx, const Vec3& point, double eps) {
    Plane surface;
    std::vector<Plane> side_planes;
    if (!build_triangle_planes(mesh, tri_idx, surface, side_planes)) return false;
    // surface check: we require point be above the surface (dot(n, p - p0) >= -eps)
    double sdist = dot(surface.n, sub(point, surface.p));
    if (sdist < -eps) return false;
    // side planes: require dot(n, p - p_edge) >= -eps for all three
    for (const Plane &pl : side_planes) {
        double d = dot(pl.n, sub(point, pl.p));
        if (d < -eps) return false;
    }
    return true;
}

} // namespace facility
