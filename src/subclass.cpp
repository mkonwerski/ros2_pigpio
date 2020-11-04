#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class Subclass
{
private:
public:
    Subclass(/* args */){};
    ~Subclass(){};
    std::string hello(std::string &s) const
    {
        return "Hello " + s;
    }
};

class MinimalSubscriber : public rclcpp::Node
{
public:
    MinimalSubscriber()
        : Node("minimal_subscriber")
    {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }

private:
    Subclass subclass_;
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
        std::string s = subclass_.hello(msg->data);
        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", s.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalSubscriber>());
    rclcpp::shutdown();
    return 0;
}