#ifndef GODOT__GODOT_ROS__DEMOS__JointJogController_HPP
#define GODOT__GODOT_ROS__DEMOS__JointJogController_HPP

#include <iostream>

#include "core/object/ref_counted.h"

#include "rclcpp/rclcpp.hpp"
#include "control_msgs/msg/joint_jog.hpp"

class JointJogController : public RefCounted {
  GDCLASS(JointJogController, RefCounted);
public:
  JointJogController() {
    try {
      rclcpp::init(0, nullptr);
    } catch (...) {
      std::cout << "Already initialized, ignoring..." << std::endl;
    }

    m_node = std::make_shared<rclcpp::Node>("godot_JointJogController_node");

    subscription_ = m_node->create_subscription<control_msgs::msg::JointJog>(
      "/JointJog", 10,
      std::bind(&JointJogController::topic_callback, this, std::placeholders::_1));

    publisher_ = m_node->create_publisher<control_msgs::msg::JointJog>("/JointJog", 10);
  }

  ~JointJogController() {
    rclcpp::shutdown();
  }

  inline void spin_some() {
    rclcpp::spin_some(m_node);
  }

  void topic_callback(const control_msgs::msg::JointJog::SharedPtr msg)
  {
    names_.clear();
    velocities_.clear();
    for (size_t i = 0; i < msg->joint_names.size(); ++i) {
      names_.push_back(String(msg->joint_names[i].c_str()));
    }
    for (size_t i = 0; i < msg->velocities.size(); ++i) {
      velocities_.push_back(msg->velocities[i]);
    }
    new_data = true;
  }

  PackedStringArray get_joint_names() { return names_; }
  PackedFloat64Array get_joint_velocities() { return velocities_; }
  bool has_new_data() {
    if (new_data) { new_data = false; return true; }
    return false;
  }

  void set_joint_velocity(PackedStringArray joint_names, PackedFloat64Array velocities)
  {
    control_msgs::msg::JointJog msg;
    msg.header.stamp = m_node->get_clock()->now();
    for (int i = 0; i < joint_names.size(); ++i) {
      msg.joint_names.push_back(std::string(joint_names[i].utf8().get_data()));
    }
    for (int i = 0; i < velocities.size(); ++i) {
      msg.velocities.push_back(velocities[i]);
    }
    publisher_->publish(msg);
  }

protected:
  static void _bind_methods();

  std::shared_ptr<rclcpp::Node> m_node;
  rclcpp::Subscription<control_msgs::msg::JointJog>::SharedPtr subscription_;
  rclcpp::Publisher<control_msgs::msg::JointJog>::SharedPtr publisher_;
  PackedStringArray names_;
  PackedFloat64Array velocities_;
  bool new_data = false;
};
#endif
