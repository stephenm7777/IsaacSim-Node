#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

class SawyerRobot : public rclcpp::Node{
  public:
    SawyerRobot() : Node("sawyer_robot"){ 
      sawyer_pub = this->create_publisher<sensor_msgs::msg::JointState>("joint_command", 10); 
       
      joint_message = sensor_msgs::msg::JointState();
      joint_message.name = { 

        "right_j0", 
        "head_pan", 
        "right_j1", 
        "right_j2", 
        "right_j3", 
        "right_j4", 
        "right_j5", 
        "right_j6"
      };

      joint_message.position = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      joint_message.velocity = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      
      step = 20; 
      frequency = 1; 
      amplitude = 1; 

      timer_ = this->create_wall_timer(
        std::chrono::milliseconds(20), 
        std::bind(&SawyerRobot::jointCallback, this)); 
  
    }

    void jointCallback(){ 
      target_position = sin(2 * M_PI * frequency * time); 
      target_velocity = 2 * M_PI * frequency * cos(2 * M_PI * frequency * time);
      
      for (double &pos : joint_message.position){ 
        pos = target_position; 
      }

      for (double &pos : joint_message.velocity){ 
        pos = target_velocity; 
      }



    sawyer_pub->publish(joint_message); 

    time += step / 1000.0; 
  }

  private: 
    double step, frequency, time, amplitude, target_position, target_velocity; 
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr sawyer_pub; 
    rclcpp::TimerBase::SharedPtr timer_; 
    sensor_msgs::msg::JointState joint_message; 

};
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv); 
  rclcpp::spin(std::make_shared<SawyerRobot>());
  rclcpp::shutdown(); 

  return 0; 
}
