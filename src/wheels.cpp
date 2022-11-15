#include "rclcpp/rclcpp.hpp"
#include "ros2_pigpio/HBridge.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class WheelsWriter : public rclcpp::Node
{
private:
    int pi_;

    HBridge right_;
    HBridge left_;

    void write() const
    {
        RCLCPP_INFO(this->get_logger(), "-----GO!-----");
        right_.drive(255);
        left_.drive(255);
    }

public:
    WheelsWriter() : Node("servo_subscriber")
    {
        pi_ = pigpio_start(NULL, NULL);
        
        if (pi_ >= 0)
        {
            right_.setPin(pi_, 17, 18);
            left_.setPin(pi_, 21, 22);
            write();
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "cannot connect pigpiod");
            rclcpp::shutdown();
            exit(1);
        }
    }

    ~WheelsWriter()
    {
        RCLCPP_INFO(this->get_logger(), "-----STOP!-----");
        right_.drive(0);
        left_.drive(0);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WheelsWriter>());
    rclcpp::shutdown();
    return 0;
}
