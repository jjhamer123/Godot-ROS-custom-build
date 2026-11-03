#ifndef GODOT__GODOT_ROS__DEMOS__BoolTopic_HPP
#define GODOT__GODOT_ROS__DEMOS__BoolTopic_HPP

#include <atomic>
#include <iostream>
#include <string>

#include "core/object/ref_counted.h"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

class BoolTopic : public RefCounted
{
    GDCLASS(BoolTopic, RefCounted);

public:
    BoolTopic()
    {
        try
        {
            rclcpp::init(0, nullptr);
        }
        catch (...)
        {
            std::cout << "Already initialized, ignoring..." << std::endl;
        }

        static std::atomic<int> instance_count{0};
        int id = instance_count++;
        m_node = std::make_shared<rclcpp::Node>("godot_BoolTopic_node_" + std::to_string(id));

        set_topic("/godot_bool");
    }
    ~BoolTopic() {}
    inline void spin_some()
    {
        rclcpp::spin_some(m_node);
    }
    void set_topic(String topic)
    {
        set_publish_topic(topic);
        set_subscribe_topic(topic);
    }

    void set_publish_topic(String topic)
    {
        publisher_.reset();
        publisher_ = m_node->create_publisher<std_msgs::msg::Bool>(
            std::string(topic.utf8().get_data()), 10);
    }

    void set_subscribe_topic(String topic)
    {
        subscription_.reset();
        subscription_ = m_node->create_subscription<std_msgs::msg::Bool>(
            std::string(topic.utf8().get_data()), 10,
            std::bind(&BoolTopic::topic_callback, this, std::placeholders::_1));
    }

    void topic_callback(const std_msgs::msg::Bool::SharedPtr msg)
    {
        value_ = msg->data;
        new_data = true;
    }

    void publish(bool value)
    {
        if (!publisher_)
            return;
        std_msgs::msg::Bool msg;
        msg.data = value;
        publisher_->publish(msg);
    }

    bool get_value() { return value_; }

    bool has_new_data()
    {
        if (new_data)
        {
            new_data = false;
            return true;
        }
        return false;
    }

protected:
    static void _bind_methods();

    std::shared_ptr<rclcpp::Node> m_node;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_;
    bool value_ = false;
    bool new_data = false;
};
#endif