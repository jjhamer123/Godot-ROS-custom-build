#include "godot_ros/demos/joint_jog_controller.hpp"

void JointJogController::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("get_joint_names"), &JointJogController::get_joint_names);
  ClassDB::bind_method(D_METHOD("get_joint_velocities"), &JointJogController::get_joint_velocities);
  ClassDB::bind_method(D_METHOD("has_new_data"), &JointJogController::has_new_data);
  ClassDB::bind_method(D_METHOD("spin_some"), &JointJogController::spin_some);
  ClassDB::bind_method(D_METHOD("set_joint_velocity", "joint_names", "velocities"),
                        &JointJogController::set_joint_velocity);
}
