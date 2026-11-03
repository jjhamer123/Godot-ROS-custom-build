#include "godot_ros/demos/bool_topic.hpp"

void BoolTopic::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("spin_some"), &BoolTopic::spin_some);
  ClassDB::bind_method(D_METHOD("set_topic", "topic"), &BoolTopic::set_topic);
  ClassDB::bind_method(D_METHOD("set_publish_topic", "topic"), &BoolTopic::set_publish_topic);
  ClassDB::bind_method(D_METHOD("set_subscribe_topic", "topic"), &BoolTopic::set_subscribe_topic);
  ClassDB::bind_method(D_METHOD("publish", "value"), &BoolTopic::publish);
  ClassDB::bind_method(D_METHOD("get_value"), &BoolTopic::get_value);
  ClassDB::bind_method(D_METHOD("has_new_data"), &BoolTopic::has_new_data);
}