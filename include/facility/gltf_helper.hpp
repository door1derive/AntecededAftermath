#pragma once
#include <string>
#include <unordered_map>
#include <array>

#include "include/facility/objloader.hpp" // Vec3

namespace facility {

// A simple transform container
struct Transform {
    Vec3 translation{0.0,0.0,0.0};
    // quaternion in order (x, y, z, w)
    std::array<double,4> rotation{0.0,0.0,0.0,1.0};
    Vec3 scale{1.0,1.0,1.0};
    // 4x4 column-major matrix suitable for OpenGL shader uniforms
    std::array<double,16> matrix{}; // column-major: m[col*4 + row]
};

// Load nodes and compute world transforms.
// - path: path to .glb or .gltf
// - out_node_transforms: map node name -> Transform (only nodes with a non-empty name are inserted)
// Returns true on success, false on error with an error string in out_error (if provided).
bool load_gltf_node_transforms(const std::string& path,
                               std::unordered_map<std::string, Transform>& out_node_transforms,
                               std::string* out_error = nullptr);

} // namespace facility