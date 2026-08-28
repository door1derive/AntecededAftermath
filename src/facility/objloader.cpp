#include "include/facility/objloader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <cmath>

namespace facility {

namespace {
    // Parse a single face index token: "v", "v/vt", "v//vn", "v/vt/vn"
    bool parse_face_index(const std::string& token, int& out_v_idx) {
        if (token.empty()) return false;
        std::size_t pos = token.find('/');
        std::string vpart = (pos == std::string::npos) ? token : token.substr(0, pos);
        try {
            int idx = std::stoi(vpart);
            out_v_idx = idx - 1; // OBJ 1-based -> 0-based
            return true;
        } catch (...) {
            return false;
        }
    }

    // rotate from Y-up to Z-up: (x,y,z) -> (x,z,-y)
    inline Vec3 y_up_to_z_up(const Vec3& v) { return Vec3(v.x, v.z, -v.y); }
}

bool load_obj(const std::string& path, Mesh& mesh, std::string* out_error, bool convert_y_up_to_z_up) {
    mesh.verts.clear();
    mesh.tris.clear();
    mesh.vnorms.clear();

    std::ifstream in(path);
    if (!in.is_open()) {
        if (out_error) *out_error = "Failed to open file: " + path;
        return false;
    }

    std::string line;
    while (std::getline(in, line)) {
        std::size_t i = 0;
        while (i < line.size() && std::isspace((unsigned char)line[i])) ++i;
        if (i >= line.size()) continue;
        if (line[i] == '#') continue;

        std::istringstream ss(line.substr(i));
        std::string tok;
        ss >> tok;
        if (tok == "v") {
            double x,y,z;
            ss >> x >> y >> z;
            Vec3 p(x,y,z);
            if (convert_y_up_to_z_up) p = y_up_to_z_up(p);
            mesh.verts.emplace_back(p);
        } else if (tok == "f") {
            std::vector<int> idxs;
            std::string ft;
            while (ss >> ft) {
                int vid = 0;
                if (!parse_face_index(ft, vid)) {
                    if (out_error) *out_error = "Failed to parse face index: " + ft;
                    return false;
                }
                if (vid < 0 || vid >= static_cast<int>(mesh.verts.size())) {
                    if (out_error) *out_error = "Face index out of range: " + ft;
                    return false;
                }
                idxs.push_back(vid);
            }
            if (idxs.size() < 3) continue;
            for (size_t k = 1; k + 1 < idxs.size(); ++k) {
                mesh.tris.push_back({ idxs[0], idxs[k], idxs[k+1] });
            }
        }
    }

    return true;
}

void compute_vertex_normals(Mesh& mesh, bool normalize) {
    mesh.vnorms.assign(mesh.verts.size(), Vec3(0.0,0.0,0.0));
    auto sub = [](const Vec3& a, const Vec3& b) { return Vec3(a.x-b.x, a.y-b.y, a.z-b.z); };
    auto cross = [](const Vec3& a, const Vec3& b) {
        return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    };
    auto add_inplace = [](Vec3& a, const Vec3& b) { a.x += b.x; a.y += b.y; a.z += b.z; };
    auto len = [](const Vec3& a) { return std::sqrt(a.x*a.x + a.y*a.y + a.z*a.z); };

    for (const auto& tri : mesh.tris) {
        const Vec3& A = mesh.verts[tri[0]];
        const Vec3& B = mesh.verts[tri[1]];
        const Vec3& C = mesh.verts[tri[2]];
        Vec3 e1 = sub(B, A);
        Vec3 e2 = sub(C, A);
        Vec3 fn = cross(e1, e2); // area-weighted normal (magnitude = 2*area)
        double fl = len(fn);
        if (fl <= 1e-12) continue;
        add_inplace(mesh.vnorms[tri[0]], fn);
        add_inplace(mesh.vnorms[tri[1]], fn);
        add_inplace(mesh.vnorms[tri[2]], fn);
    }

    if (normalize) {
        for (auto &vn : mesh.vnorms) {
            double L = len(vn);
            if (L > 0.0) {
                vn.x /= L; vn.y /= L; vn.z /= L;
            } else {
                // fallback normal: Z-up
                vn = Vec3(0.0, 0.0, 1.0);
            }
        }
    }
}

} // namespace facility
