#include <iostream>
#include <vector>
#include "include/facility/objloader.hpp"
#include "include/facility/path_utils.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: facing_test <model.obj>\n";
        return 1;
    }
    std::string path = argv[1];
    facility::Mesh mesh;
    std::string err;
    if (!facility::load_obj(path, mesh, &err, /*convert_y_up_to_z_up=*/true)) {
        std::cerr << "OBJ load failed: " << err << "\n";
        return 2;
    }
    facility::compute_vertex_normals(mesh);
    std::cout << "Loaded mesh: verts=" << mesh.verts.size() << " tris=" << mesh.tris.size() << "\n";

    // pick some sample points: triangle centroids and slightly above them
    for (int ti = 0; ti < (int)mesh.tris.size() && ti < 10; ++ti) {
        auto t = mesh.tris[ti];
        auto a = mesh.verts[t[0]];
        auto b = mesh.verts[t[1]];
        auto c = mesh.verts[t[2]];
        facility::Vec3 centroid((a.x+b.x+c.x)/3.0, (a.y+b.y+c.y)/3.0, (a.z+b.z+c.z)/3.0);
        // compute average vertex normal
        facility::Vec3 nav((mesh.vnorms[t[0]].x + mesh.vnorms[t[1]].x + mesh.vnorms[t[2]].x) / 3.0,
                           (mesh.vnorms[t[0]].y + mesh.vnorms[t[1]].y + mesh.vnorms[t[2]].y) / 3.0,
                           (mesh.vnorms[t[0]].z + mesh.vnorms[t[1]].z + mesh.vnorms[t[2]].z) / 3.0);
        // test points: exactly at centroid (may be borderline), slightly above and slightly offset
        facility::Vec3 p_center = centroid;
        facility::Vec3 p_above = facility::Vec3(centroid.x + nav.x*0.01, centroid.y + nav.y*0.01, centroid.z + nav.z*0.01);
        facility::Vec3 p_offset = facility::Vec3(centroid.x + 0.1, centroid.y + 0.1, centroid.z + 0.1);
        bool c_center = facility::point_in_triangle_facing_test(mesh, ti, p_center, 1e-6);
        bool c_above = facility::point_in_triangle_facing_test(mesh, ti, p_above, 1e-6);
        bool c_offset = facility::point_in_triangle_facing_test(mesh, ti, p_offset, 1e-6);
        std::cout << "Tri " << ti << ": center=" << c_center << " above=" << c_above << " offset=" << c_offset << "\n";
    }

    return 0;
}
