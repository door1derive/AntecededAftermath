#define TINYGLTF_IMPLEMENTATION
// disable image loaders if you don't need textures; leave commented if you want textures
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"
#include "include/facility/gltf_helper.hpp"
#include <vector>
#include <iostream>
#include <cmath>

namespace facility {

using tinygltf::Model;
using tinygltf::Node;
using tinygltf::TinyGLTF;

// Helpers for 4x4 matrices (column-major)
static void mat4_identity(std::array<double,16>& M) {
    M.fill(0.0);
    M[0] = M[5] = M[10] = M[15] = 1.0;
}
static std::array<double,16> mat4_mul(const std::array<double,16>& A, const std::array<double,16>& B) {
    std::array<double,16> R{};
    // column-major multiply R = A * B
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            double v = 0.0;
            for (int k = 0; k < 4; ++k) {
                v += A[k*4 + r] * B[c*4 + k];
            }
            R[c*4 + r] = v;
        }
    }
    return R;
}

// Build translation matrix (column-major)
static std::array<double,16> mat4_from_trs(const std::array<double,3>& t, const std::array<double,4>& q, const std::array<double,3>& s) {
    // quaternion q = (x,y,z,w)
    double x = q[0], y = q[1], z = q[2], w = q[3];
    double x2 = x + x; double y2 = y + y; double z2 = z + z;
    double xx = x * x2; double yy = y * y2; double zz = z * z2;
    double xy = x * y2; double xz = x * z2; double yz = y * z2;
    double wx = w * x2; double wy = w * y2; double wz = w * z2;

    std::array<double,16> M{};
    M[0] = (1.0 - (yy + zz)) * s[0];
    M[1] = (xy + wz) * s[0];
    M[2] = (xz - wy) * s[0];
    M[3] = 0.0;

    M[4] = (xy - wz) * s[1];
    M[5] = (1.0 - (xx + zz)) * s[1];
    M[6] = (yz + wx) * s[1];
    M[7] = 0.0;

    M[8] = (xz + wy) * s[2];
    M[9] = (yz - wx) * s[2];
    M[10] = (1.0 - (xx + yy)) * s[2];
    M[11] = 0.0;

    M[12] = t[0];
    M[13] = t[1];
    M[14] = t[2];
    M[15] = 1.0;

    return M;
}

static std::array<double,16> mat4_from_array(const std::vector<double>& arr) {
    std::array<double,16> M{};
    if (arr.size() >= 16) {
        for (int i = 0; i < 16; ++i) M[i] = arr[i];
    } else {
        mat4_identity(M);
    }
    return M;
}

// Compose TRS from node (tinygltf) local transform
static std::array<double,16> node_local_matrix(const Node& node) {
    // tinygltf node: may have .matrix (16 double) OR translation, rotation, scale
    if (!node.matrix.empty()) {
        // the matrix in tinygltf is stored as std::vector<double>
        return mat4_from_array(node.matrix);
    } else {
        std::array<double,3> t{0.0,0.0,0.0};
        std::array<double,4> r{0.0,0.0,0.0,1.0};
        std::array<double,3> s{1.0,1.0,1.0};
        if (!node.translation.empty()) {
            t[0] = node.translation[0];
            t[1] = node.translation[1];
            t[2] = node.translation[2];
        }
        if (!node.rotation.empty()) {
            // tinygltf rotation is [x, y, z, w]
            r[0] = node.rotation[0];
            r[1] = node.rotation[1];
            r[2] = node.rotation[2];
            r[3] = node.rotation[3];
        }
        if (!node.scale.empty()) {
            s[0] = node.scale[0];
            s[1] = node.scale[1];
            s[2] = node.scale[2];
        }
        return mat4_from_trs(t, r, s);
    }
}

// Recursively compute world transforms. We avoid recursion depth issues by iterative stack.
static void compute_world_matrices(const Model& model, std::vector<std::array<double,16>>& out_worlds) {
    size_t N = model.nodes.size();
    out_worlds.resize(N);
    // initialize to identity
    for (size_t i = 0; i < N; ++i) mat4_identity(out_worlds[i]);

    // Find all root nodes (model.scenes[model.defaultScene].nodes or fallback)
    std::vector<int> roots;
    if (model.defaultScene >= 0 && model.defaultScene < (int)model.scenes.size()) {
        const auto& sc = model.scenes[model.defaultScene];
        for (int n : sc.nodes) roots.push_back(n);
    } else {
        // fallback: any node that isn't a child of another node -> root
        std::vector<char> is_child(N, 0);
        for (size_t i = 0; i < N; ++i) {
            for (int c : model.nodes[i].children) {
                if (c >= 0 && c < (int)N) is_child[c] = 1;
            }
        }
        for (size_t i = 0; i < N; ++i) if (!is_child[i]) roots.push_back((int)i);
    }

    // iterative DFS stack: pair(nodeIndex, parentWorldMatrix)
    struct StackItem { int idx; std::array<double,16> parentMat; };
    std::vector<StackItem> stack;
    for (int r : roots) {
        StackItem s{ r, {} };
        mat4_identity(s.parentMat);
        stack.push_back(s);
    }

    while (!stack.empty()) {
        StackItem cur = stack.back();
        stack.pop_back();
        const Node& node = model.nodes[cur.idx];
        std::array<double,16> local = node_local_matrix(node);
        std::array<double,16> world = mat4_mul(cur.parentMat, local);
        out_worlds[cur.idx] = world;

        for (int c : node.children) {
            StackItem child; child.idx = c; child.parentMat = world;
            stack.push_back(child);
        }
    }
}

// Decompose column-major matrix into TRS. Simple extraction: translation from last column,
// scale from column lengths, rotation from normalized columns.
static void decompose_matrix(const std::array<double,16>& M,
                             std::array<double,3>& out_t,
                             std::array<double,4>& out_q,
                             std::array<double,3>& out_s) {
    // translation
    out_t[0] = M[12];
    out_t[1] = M[13];
    out_t[2] = M[14];

    // column vectors
    std::array<double,3> col0{ M[0], M[1], M[2] };
    std::array<double,3> col1{ M[4], M[5], M[6] };
    std::array<double,3> col2{ M[8], M[9], M[10] };

    // scales
    auto len3 = [](const std::array<double,3>& v)->double { return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]); };
    out_s[0] = len3(col0);
    out_s[1] = len3(col1);
    out_s[2] = len3(col2);

    // normalize columns to get rotation matrix
    std::array<double,9> R{};
    if (out_s[0] > 0) { R[0] = col0[0]/out_s[0]; R[1] = col0[1]/out_s[0]; R[2] = col0[2]/out_s[0]; }
    if (out_s[1] > 0) { R[3] = col1[0]/out_s[1]; R[4] = col1[1]/out_s[1]; R[5] = col1[2]/out_s[1]; }
    if (out_s[2] > 0) { R[6] = col2[0]/out_s[2]; R[7] = col2[1]/out_s[2]; R[8] = col2[2]/out_s[2]; }

    // Convert rotation matrix to quaternion (x,y,z,w)
    double trace = R[0] + R[4] + R[8];
    double qx,qy,qz,qw;
    if (trace > 0.0) {
        double s = 0.5 / std::sqrt(trace + 1.0);
        qw = 0.25 / s;
        qx = (R[7] - R[5]) * s;
        qy = (R[2] - R[6]) * s;
        qz = (R[3] - R[1]) * s;
    } else {
        if (R[0] > R[4] && R[0] > R[8]) {
            double s = 2.0 * std::sqrt(1.0 + R[0] - R[4] - R[8]);
            qw = (R[7] - R[5]) / s;
            qx = 0.25 * s;
            qy = (R[1] + R[3]) / s;
            qz = (R[2] + R[6]) / s;
        } else if (R[4] > R[8]) {
            double s = 2.0 * std::sqrt(1.0 + R[4] - R[0] - R[8]);
            qw = (R[2] - R[6]) / s;
            qx = (R[1] + R[3]) / s;
            qy = 0.25 * s;
            qz = (R[5] + R[7]) / s;
        } else {
            double s = 2.0 * std::sqrt(1.0 + R[8] - R[0] - R[4]);
            qw = (R[3] - R[1]) / s;
            qx = (R[2] + R[6]) / s;
            qy = (R[5] + R[7]) / s;
            qz = 0.25 * s;
        }
    }
    out_q[0] = qx; out_q[1] = qy; out_q[2] = qz; out_q[3] = qw;
}

bool load_gltf_node_transforms(const std::string& path,
                               std::unordered_map<std::string, Transform>& out_node_transforms,
                               std::string* out_error) {
    TinyGLTF loader;
    Model model;
    std::string err, warn;
    bool ok = false;
    // loader loads both .gltf and .glb transparently
    ok = loader.LoadBinaryFromFile(&model, &err, &warn, path); // .glb
    if (!ok) {
        // try ASCII .gltf
        ok = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    }
    if (!warn.empty()) {
        // you can print warnings if you want
        std::cerr << "GLTF warn: " << warn << std::endl;
    }
    if (!ok) {
        if (out_error) *out_error = err.empty() ? "Failed to load model" : err;
        return false;
    }

    // compute world matrices
    std::vector<std::array<double,16>> worlds;
    compute_world_matrices(model, worlds);

    out_node_transforms.clear();
    for (size_t i = 0; i < model.nodes.size(); ++i) {
        const Node &n = model.nodes[i];
        if (n.name.empty()) continue; // skip anonymous nodes
        Transform t;
        t.matrix = worlds[i];
        std::array<double,3> tt; std::array<double,4> qq; std::array<double,3> ss;
        decompose_matrix(t.matrix, tt, qq, ss);
        t.translation = Vec3(tt[0], tt[1], tt[2]);
        t.rotation = qq;
        t.scale = Vec3(ss[0], ss[1], ss[2]);
        out_node_transforms[n.name] = t;
    }

    return true;
}

} // namespace facility
