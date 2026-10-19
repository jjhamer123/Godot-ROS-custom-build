#include "godot_ros/demos/joint_state_subscriber.hpp"

void JointStateSubscriber::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("spin_some"), &JointStateSubscriber::spin_some);
  ClassDB::bind_method(D_METHOD("set_topic", "topic"), &JointStateSubscriber::set_topic);
  ClassDB::bind_method(D_METHOD("get_joint_names"), &JointStateSubscriber::get_joint_names);
  ClassDB::bind_method(D_METHOD("get_joint_positions"), &JointStateSubscriber::get_joint_positions);
  ClassDB::bind_method(D_METHOD("get_joint_velocities"), &JointStateSubscriber::get_joint_velocities);
  ClassDB::bind_method(D_METHOD("has_new_data"), &JointStateSubscriber::has_new_data);
}
