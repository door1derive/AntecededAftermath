#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>

// Minimal Vec3/Quat/Transform types (adapt to your engine types)
struct Vec3 { double x,y,z; Vec3():x(0),y(0),z(0){} Vec3(double X,double Y,double Z):x(X),y(Y),z(Z){} };
struct Transform {
    Vec3 translation;
    std::array<double,4> rotation; // quaternion x,y,z,w
    Vec3 scale;
    std::array<double,16> matrix; // column-major world matrix
};

// Helpers ---------------------------------------------------------
static Vec3 add(const Vec3&a,const Vec3&b){return Vec3(a.x+b.x,a.y+b.y,a.z+b.z);}
static Vec3 sub(const Vec3&a,const Vec3&b){return Vec3(a.x-b.x,a.y-b.y,a.z-b.z);}
static Vec3 mul(const Vec3&a,double s){return Vec3(a.x*s,a.y*s,a.z*s);}
static double dot(const Vec3&a,const Vec3&b){return a.x*b.x+a.y*b.y+a.z*b.z;}
static Vec3 cross(const Vec3&a,const Vec3&b){return Vec3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);}
static double len(const Vec3&a){return std::sqrt(dot(a,a));}
static Vec3 normalize(const Vec3&a){ double L=len(a); return (L>0)? mul(a,1.0/L):Vec3(0,0,1); }

// quaternion from axis-angle (axis must be normalized)
static std::array<double,4> quat_from_axis_angle(const Vec3& axis, double angle_rad) {
    double h = std::sin(0.5*angle_rad);
    return { axis.x*h, axis.y*h, axis.z*h, std::cos(0.5*angle_rad) };
}
static std::array<double,4> quat_mul(const std::array<double,4>& a, const std::array<double,4>& b) {
    // (ax,ay,az,aw) * (bx,by,bz,bw)
    return {
        a[3]*b[0] + a[0]*b[3] + a[1]*b[2] - a[2]*b[1],
        a[3]*b[1] - a[0]*b[2] + a[1]*b[3] + a[2]*b[0],
        a[3]*b[2] + a[0]*b[1] - a[1]*b[0] + a[2]*b[3],
        a[3]*b[3] - a[0]*b[0] - a[1]*b[1] - a[2]*b[2]
    };
}
// rotate vector v by quaternion q
static Vec3 quat_rotate(const std::array<double,4>& q, const Vec3& v) {
    // v' = q * (v_quat) * q^{-1}
    // optimized version:
    Vec3 qv(q[0], q[1], q[2]);
    Vec3 t = mul(cross(qv, v), 2.0 * q[3]);
    t = add(t, mul(cross(qv, t), 2.0));
    t = add(t, mul(v, 1.0));
    // above formula is a common optimization; fallback safe option uses full math if needed
    // but we'll use this approximate formula assuming unit quaternion
    return t;
}

// Convert degrees <-> radians
static double deg2rad(double d) { return d * (M_PI / 180.0); }
static double rad2deg(double r) { return r * (180.0 / M_PI); }

// rotate an object's world transform about point 'pivot' by quaternion 'rot'.
// We operate on the object's world matrix (column-major) directly for simplicity.
// If your engine stores local transforms, convert appropriately (see notes).
static void rotate_object_about_point(std::array<double,16>& worldMat, const Vec3& pivot, const std::array<double,4>& rotQuat) {
    // Build rotation matrix R (column-major) from quaternion
    double x = rotQuat[0], y = rotQuat[1], z = rotQuat[2], w = rotQuat[3];
    double x2 = x + x, y2 = y + y, z2 = z + z;
    double xx = x*x2, yy = y*y2, zz = z*z2;
    double xy = x*y2, xz = x*z2, yz = y*z2;
    double wx = w*x2, wy = w*y2, wz = w*z2;
    std::array<double,16> R{};
    R[0] = 1.0 - (yy + zz); R[4] = xy - wz;       R[8]  = xz + wy;       R[12] = 0.0;
    R[1] = xy + wz;       R[5] = 1.0 - (xx + zz); R[9]  = yz - wx;       R[13] = 0.0;
    R[2] = xz - wy;       R[6] = yz + wx;       R[10] = 1.0 - (xx + yy); R[14] = 0.0;
    R[3] = 0.0;           R[7] = 0.0;           R[11] = 0.0;           R[15] = 1.0;
    // Build T(-pivot), T(pivot) matrices (column-major)
    std::array<double,16> Tneg{}, Tpos{};
    for(int i=0;i<16;++i){ Tneg[i]=0; Tpos[i]=0; }
    Tneg[0]=Tneg[5]=Tneg[10]=Tneg[15]=1.0;
    Tpos[0]=Tpos[5]=Tpos[10]=Tpos[15]=1.0;
    Tneg[12] = -pivot.x; Tneg[13] = -pivot.y; Tneg[14] = -pivot.z;
    Tpos[12] = pivot.x;  Tpos[13] = pivot.y;  Tpos[14] = pivot.z;
    // Compute M' = Tpos * R * Tneg * worldMat
    // We multiply matrices in column-major order using a small helper
    auto mul4 = [](const std::array<double,16>& A, const std::array<double,16>& B) {
        std::array<double,16> M{};
        for (int c = 0; c < 4; ++c) for (int r = 0; r < 4; ++r) {
            double v = 0.0;
            for (int k = 0; k < 4; ++k) v += A[k*4 + r] * B[c*4 + k];
            M[c*4 + r] = v;
        }
        return M;
    };
    std::array<double,16> M1 = mul4(R, Tneg);
    std::array<double,16> M2 = mul4(Tpos, M1);
    std::array<double,16> M3 = mul4(M2, worldMat);
    worldMat = M3;
}

// from-to angle in plane: compute signed angle of vector v relative to basis (right, forward) by atan2
static double signed_angle_in_plane(const Vec3& v, const Vec3& right, const Vec3& forward) {
    double xr = dot(v, right);
    double xf = dot(v, forward);
    return std::atan2(xf, xr); // note: returns angle where 0 corresponds to +right, positive toward +forward
}

// Example driver: apply two-link IK to two mesh objects (thigh and shin) so ankle anchor reaches target
// nodes: a map<string,Transform> from your GLTF loader (world transforms for nodes/meshes/empties)
// names: hip_anchor_name, knee_anchor_name, ankle_anchor_name, thigh_node_name, shin_node_name
// plane_normal: the rotation axis normal for the limb (e.g., Vec3{0,0,1} for Z-up). Must be unit.
void apply_two_link_ik_to_objects(std::unordered_map<std::string,Transform>& nodes,
                                  const std::string& hip_anchor_name,
                                  const std::string& knee_anchor_name,
                                  const std::string& ankle_anchor_name,
                                  const std::string& thigh_node_name,
                                  const std::string& shin_node_name,
                                  const Vec3& target_world,
                                  const Vec3& plane_normal,
                                  bool elbow_up_preference,
                                  std::pair<double,double>* prev_angles_deg // optional (hip_deg, knee_deg) for continuity
                                  )
{
    using std::string;
    // find anchors and object world transforms
    if (nodes.count(hip_anchor_name)==0 || nodes.count(knee_anchor_name)==0 || nodes.count(ankle_anchor_name)==0) {
        std::cerr << \"Missing anchors\\n\"; return;
    }
    if (nodes.count(thigh_node_name)==0 || nodes.count(shin_node_name)==0) {
        std::cerr << \"Missing mesh nodes\\n\"; return;
    }
    Transform& hip_anchor = nodes[hip_anchor_name];
    Transform& knee_anchor = nodes[knee_anchor_name];
    Transform& ankle_anchor = nodes[ankle_anchor_name];
    Transform& thigh_node = nodes[thigh_node_name]; // assume this stores world matrix in .matrix
    Transform& shin_node = nodes[shin_node_name];

    Vec3 hip_pos = hip_anchor.translation;
    Vec3 knee_pos = knee_anchor.translation;
    Vec3 ankle_pos = ankle_anchor.translation;

    // compute segment lengths from anchors
    double l1 = len(sub(knee_pos, hip_pos)); // thigh length
    double l2 = len(sub(ankle_pos, knee_pos)); // shin length
    if (l1 <= 1e-6 || l2 <= 1e-6) {
        std::cerr << \"Degenerate segment length\\n\"; return;
    }

    // Build an orthonormal basis for the limb plane.
    // Choose 'right' as any vector orthogonal to plane_normal; to be stable, use world X if not parallel.
    Vec3 up = plane_normal;
    Vec3 world_x{1,0,0};
    Vec3 right = cross(up, world_x);
    if (len(right) < 1e-6) right = cross(up, Vec3(0,1,0)); // fallback
    right = normalize(right);
    Vec3 forward = normalize(cross(right, up)); // forward completes right-handed basis

    // Vector from hip to target, projected into plane
    Vec3 v_ht = sub(target_world, hip_pos);
    // project onto plane: remove component along up
    Vec3 v_proj = sub(v_ht, mul(up, dot(v_ht, up)));
    double r = len(v_proj);
    if (r < 1e-9) r = 0.0;

    // compute angle in plane relative to right axis
    double ang_rad = signed_angle_in_plane(v_proj, right, forward);
    double ang_deg = rad2deg(ang_rad);

    // Call two-link IK (degrees)
    // Note: two_link_ik_degrees signature: (angle_deg, r, l1, l2, elbow_up:bool|None, allow_unreachable, prev_angles)
    bool allow_unreachable = true;
    std::pair<double,double> prev = prev_angles_deg ? *prev_angles_deg : std::pair<double,double>(0.0,0.0);

    // Here we assume you have facility::two_link_ik_degrees available in scope.
    double hip_deg, knee_deg; bool reachable;
    {
        // wrap call - replace with actual function call; this is pseudocode placeholder
        // e.g., std::tie(hip_deg, knee_deg, reachable) = facility::two_link_ik_degrees(ang_deg, r, l1, l2, elbow_up, allow_unreachable, prev_angles);
        // Because we can't call it here, assume function exists and fill example:
        // For demonstration only: pretend it's reachable and angles zero
        // Replace the next line with the real call.
        std::tie(hip_deg, knee_deg, reachable) = std::make_tuple(ang_deg*0.0, 0.0, true);
    }

    // compute world rotation quaternions for hip and knee rotations (about up axis)
    std::array<double,4> hip_rot = quat_from_axis_angle(up, deg2rad(hip_deg));
    std::array<double,4> knee_rot = quat_from_axis_angle(up, deg2rad(knee_deg));

    // Apply hip rotation: rotate thigh_node.world around hip_pos by hip_rot
    rotate_object_about_point(thigh_node.matrix, hip_pos, hip_rot);

    // After rotating thigh, the knee anchor and shin_node world transforms should be updated accordingly in your scene graph.
    // In a simple setup where nodes map to world matrices directly, you should recompute knee world position before rotating shin.
    // Here we assume you've updated knee_pos and shin_node.matrix after hip rotation (engine-specific).
    // For demonstration, we rotate shin_node about knee_pos by knee_rot:
    rotate_object_about_point(shin_node.matrix, knee_pos, knee_rot);

    // Save angles for continuity
    if (prev_angles_deg) *prev_angles_deg = std::make_pair(hip_deg, knee_deg);
}