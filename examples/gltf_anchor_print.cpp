#include <iostream>
#include "include/facility/gltf_helper.hpp"

int main() {
    std::unordered_map<std::string, facility::Transform> nodes;
    std::string err;
    bool ok = facility::load_gltf_node_transforms("facility/adam.glb", nodes, &err);
    if (!ok) {
        std::cerr << \"Failed to load adam.glb: \" << err << std::endl;
        return 1;
    }
    std::cout << \"Found \" << nodes.size() << \" named nodes\\n\";
    for (auto &kv : nodes) {
        const std::string &name = kv.first;
        const auto &T = kv.second;
        std::cout << \"Node: \" << name
                  << \"  pos=\" << T.translation.x << \",\" << T.translation.y << \",\" << T.translation.z
                  << \"  rot=(\" << T.rotation[0] << \",\" << T.rotation[1] << \",\" << T.rotation[2] << \",\" << T.rotation[3] << \")\"
                  << \"  scale=\" << T.scale.x << \",\" << T.scale.y << \",\" << T.scale.z << std::endl;
    }
    // Example: look up an anchor named \"anchor_hand_l\" if present
    auto it = nodes.find(\"anchor_hand_l\");
    if (it != nodes.end()) {
        auto &hand = it->second;
        // Use hand.matrix / hand.translation as your IK target
    }
    return 0;
}