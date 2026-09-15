#include "godot_ros/demos/joint_state_publisher.hpp"

void JointStatePublisher::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("spin_some"), &JointStatePublisher::spin_some);
  ClassDB::bind_method(D_METHOD("publish_joint_state", "joint_names", "positions", "velocities"),
                        &JointStatePublisher::publish_joint_state);
}
