#include <ros/ros.h>
#include <gazebo_msgs/SetLightProperties.h>
#include <std_srvs/SetBool.h>
#include <geometry_msgs/Vector3.h>

class LEDController {
private:
    ros::NodeHandle nh_;
    ros::ServiceServer led_service_;
    ros::ServiceClient gazebo_light_client_;
    
    // LED names - must match the LED names in the URDF
    std::vector<std::string> led_names_ = {
        "led_front_left_light",
        "led_front_right_light",
        "led_rear_left_light",
        "led_rear_right_light"
    };
    
    // Store LED states
    std::map<std::string, bool> led_states_;

public:
    LEDController() : nh_("~") {
        // Initialize all LEDs as off
        for (const auto& name : led_names_) {
            led_states_[name] = false;
        }
        
        // Create service server for LED control
        led_service_ = nh_.advertiseService("set_leds", &LEDController::handleLEDRequest, this);
        
        // Create client for Gazebo light properties
        gazebo_light_client_ = nh_.serviceClient<gazebo_msgs::SetLightProperties>(
            "/gazebo/set_light_properties");
        
        ROS_INFO("PMB2 LED Controller initialized successfully");
        ROS_INFO("Available service: /pmb2_led_control_node/set_leds");
    }
    
    bool handleLEDRequest(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res) {
        try {
            if (req.data) {
                // Turn on all LEDs
                ROS_INFO("Turning ON all LEDs");
                for (const auto& led_name : led_names_) {
                    turnOnLED(led_name);
                    led_states_[led_name] = true;
                }
                res.success = true;
                res.message = "All LEDs turned ON";
            } else {
                // Turn off all LEDs
                ROS_INFO("Turning OFF all LEDs");
                for (const auto& led_name : led_names_) {
                    turnOffLED(led_name);
                    led_states_[led_name] = false;
                }
                res.success = true;
                res.message = "All LEDs turned OFF";
            }
            return true;
        } catch (const std::exception& e) {
            ROS_ERROR("Error handling LED request: %s", e.what());
            res.success = false;
            res.message = std::string("Error: ") + e.what();
            return false;
        }
    }
    
    void turnOnLED(const std::string& led_name) {
        gazebo_msgs::SetLightProperties srv;
        srv.request.light_name = led_name;
        
        // Red light properties
        srv.request.diffuse.r = 0.8;
        srv.request.diffuse.g = 0.0;
        srv.request.diffuse.b = 0.0;
        srv.request.diffuse.a = 1.0;
        
        srv.request.specular.r = 0.8;
        srv.request.specular.g = 0.0;
        srv.request.specular.b = 0.0;
        srv.request.specular.a = 1.0;
        
        srv.request.attenuation_constant = 0.5;
        srv.request.attenuation_linear = 0.1;
        srv.request.attenuation_quadratic = 0.01;
        srv.request.range = 5.0;
        
        if (gazebo_light_client_.call(srv)) {
            ROS_DEBUG("LED %s turned ON", led_name.c_str());
        } else {
            ROS_WARN("Failed to turn on LED: %s", led_name.c_str());
        }
    }
    
    void turnOffLED(const std::string& led_name) {
        gazebo_msgs::SetLightProperties srv;
        srv.request.light_name = led_name;
        
        // Black light properties (off)
        srv.request.diffuse.r = 0.0;
        srv.request.diffuse.g = 0.0;
        srv.request.diffuse.b = 0.0;
        srv.request.diffuse.a = 0.0;
        
        srv.request.specular.r = 0.0;
        srv.request.specular.g = 0.0;
        srv.request.specular.b = 0.0;
        srv.request.specular.a = 0.0;
        
        srv.request.attenuation_constant = 0.5;
        srv.request.attenuation_linear = 0.1;
        srv.request.attenuation_quadratic = 0.01;
        srv.request.range = 5.0;
        
        if (gazebo_light_client_.call(srv)) {
            ROS_DEBUG("LED %s turned OFF", led_name.c_str());
        } else {
            ROS_WARN("Failed to turn off LED: %s", led_name.c_str());
        }
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "pmb2_led_control_node");
    
    LEDController controller;
    
    ros::spin();
    
    return 0;
}
