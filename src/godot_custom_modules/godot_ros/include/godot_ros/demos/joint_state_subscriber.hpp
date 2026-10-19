#ifndef GODOT__GODOT_ROS__DEMOS__JointStateSubscriber_HPP
#define GODOT__GODOT_ROS__DEMOS__JointStateSubscriber_HPP

#include <iostream>
#include <string>

#include "core/object/ref_counted.h"

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

class JointStateSubscriber : public RefCounted {
  GDCLASS(JointStateSubscriber, RefCounted);
public:
  JointStateSubscriber() {
    try {
      rclcpp::init(0, nullptr);
    } catch (...) {
      std::cout << "Already initialized, ignoring..." << std::endl;
    }

    m_node = std::make_shared<rclcpp::Node>("godot_JointStateSubscriber_node");
    set_topic("/joint_states");
  }

  ~JointStateSubscriber() {
    rclcpp::shutdown();
  }

  inline void spin_some() {
    rclcpp::spin_some(m_node);
  }

  // Change which topic to listen on (e.g. the real robot's joint states)
  void set_topic(String topic) {
    subscription_.reset();
    subscription_ = m_node->create_subscription<sensor_msgs::msg::JointState>(
      std::string(topic.utf8().get_data()), 10,
      std::bind(&JointStateSubscriber::topic_callback, this, std::placeholders::_1));
  }

  void topic_callback(const sensor_msgs::msg::JointState::SharedPtr msg)
  {
    names_.clear();
    positions_.clear();
    velocities_.clear();
    for (size_t i = 0; i < msg->name.size(); ++i) {
      names_.push_back(String(msg->name[i].c_str()));
    }
    for (size_t i = 0; i < msg->position.size(); ++i) {
      positions_.push_back(msg->position[i]);
    }
    for (size_t i = 0; i < msg->velocity.size(); ++i) {
      velocities_.push_back(msg->velocity[i]);
    }
    new_data = true;
  }

  PackedStringArray get_joint_names() { return names_; }
  PackedFloat64Array get_joint_positions() { return positions_; }
  PackedFloat64Array get_joint_velocities() { return velocities_; }
  bool has_new_data() {
    if (new_data) { new_data = false; return true; }
    return false;
  }

protected:
  static void _bind_methods();

  std::shared_ptr<rclcpp::Node> m_node;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscription_;
  PackedStringArray names_;
  PackedFloat64Array positions_;
  PackedFloat64Array velocities_;
  bool new_data = false;
};
#endif
