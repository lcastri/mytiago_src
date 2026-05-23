# LED Implementation for Robot Iron (TiAGo on PMB2)

## Overview

This implementation adds **4 programmable LEDs** around the PMB2 base with full Gazebo visualization support. The LEDs can be controlled via a simple ROS service and are visible in simulation as red light sources.

## What Was Added

### 1. URDF Changes

**File:** `/pmb2_robot/pmb2_description/urdf/objects/led.urdf.xacro`
- New LED macro definition with Gazebo light elements
- All 4 LEDs defined in `all_leds` macro

**File:** `/pmb2_robot/pmb2_description/urdf/base/base.urdf.xacro`
- Added LED include
- Integrated LEDs into base macro

### 2. ROS Package

**New Package:** `pmb2_led_control`
- C++ node for LED control
- Python testing script
- Launch files

### 3. LED Positions

```
FRONT VIEW:
  LED_FL        LED_FR
  (0.35, 0.15)  (0.35, -0.15)
      |            |
    --+----base----+--
      |            |
  LED_RL        LED_RR
  (-0.35, 0.15) (-0.35, -0.15)
```

## Quick Start

### 1. Build the Package
```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

### 2. Test in Gazebo

Terminal 1 - Start the robot:
```bash
roslaunch tiago_gazebo tiago_gazebo.launch
```

Terminal 2 - Start LED control:
```bash
roslaunch pmb2_led_control pmb2_led_control.launch
```

Terminal 3 - Control LEDs:
```bash
# Turn ON
rosrun pmb2_led_control set_leds.py on

# Turn OFF
rosrun pmb2_led_control set_leds.py off
```

## Service Interface

**Service Name:** `/pmb2/set_leds`  
**Type:** `std_srvs/SetBool`

### Call Examples

**ROS Command Line:**
```bash
rosservice call /pmb2/set_leds "data: true"
rosservice call /pmb2/set_leds "data: false"
```

**Python Client:**
```python
import rospy
from std_srvs.srv import SetBool

rospy.init_node('led_test')
set_leds = rospy.ServiceProxy('/pmb2/set_leds', SetBool)

# Turn on
response = set_leds(True)
print(response.message)

# Turn off
response = set_leds(False)
print(response.message)
```

**C++ Client:**
```cpp
#include <ros/ros.h>
#include <std_srvs/SetBool.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "led_test");
    ros::NodeHandle nh;
    ros::ServiceClient client = nh.serviceClient<std_srvs::SetBool>("/pmb2/set_leds");
    
    std_srvs::SetBool srv;
    srv.request.data = true;  // Turn on
    
    if (client.call(srv)) {
        ROS_INFO("Success: %s", srv.response.message.c_str());
    } else {
        ROS_ERROR("Failed to call service");
    }
    
    return 0;
}
```

## Gazebo Visualization

The LEDs appear in Gazebo as:
- **Red spheres** at the LED positions
- **Red light sources** when turned on
- **No light emission** when turned off

### How It Works

1. The URDF defines LED links with visual meshes (red spheres)
2. Gazebo light elements are attached to each LED link
3. The C++ node communicates with Gazebo via the `/gazebo/set_light_properties` service
4. Light color/intensity changes reflect the on/off state

## File Structure

```
pmb2_led_control/
├── CMakeLists.txt
├── package.xml
├── README.md
├── src/
│   └── led_control_node.cpp          # Main LED control node
├── launch/
│   ├── pmb2_led_control.launch       # Standard launch file
│   └── tiago_gazebo_with_leds.launch # Full TiAGo+LEDs launch
├── scripts/
│   └── set_leds.py                   # Python test script
└── config/
    └── (future RViz configs)

pmb2_description/
├── urdf/base/
│   ├── base.urdf.xacro (MODIFIED)
│   └── ...
└── urdf/objects/
    ├── led.urdf.xacro                # NEW - LED definitions
    └── ...
```

## Customization Guide

### Change LED Positions

Edit `/pmb2_robot/pmb2_description/urdf/objects/led.urdf.xacro`:

```xml
<xacro:macro name="all_leds" params="parent">
  <xacro:led name="led_front_left"  parent="${parent}" x="0.35"  y="0.15"  z="0.10" />
  <!-- x: forward/backward, y: left/right, z: height -->
</xacro:macro>
```

### Change LED Color

Edit `led.urdf.xacro` in the `led` macro:

```xml
<!-- For visual material -->
<material name="RedPlastic" />

<!-- For Gazebo light -->
<diffuse>0.8 0.0 0.0 1.0</diffuse>  <!-- R G B A values 0-1 -->
```

Color examples:
- Red: `0.8 0.0 0.0 1.0`
- Green: `0.0 0.8 0.0 1.0`
- Blue: `0.0 0.0 0.8 1.0`
- Yellow: `0.8 0.8 0.0 1.0`
- White: `1.0 1.0 1.0 1.0`

### Change Light Range/Intensity

Edit `src/led_control_node.cpp` in `turnOnLED()`:

```cpp
srv.request.range = 5.0;                    // Light radius
srv.request.attenuation_linear = 0.1;       // Falloff rate
srv.request.attenuation_constant = 0.5;     // Base brightness
srv.request.attenuation_quadratic = 0.01;   // Distance power
```

### Add More LEDs

Add to `all_leds` macro in `led.urdf.xacro`:

```xml
<xacro:led name="led_new" parent="${parent}" x="0.0" y="0.25" z="0.15" />
```

Then update C++ node to include new LED in `led_names_` vector:

```cpp
std::vector<std::string> led_names_ = {
    "led_front_left_light",
    "led_front_right_light",
    "led_rear_left_light",
    "led_rear_right_light",
    "led_new_light"  // New LED
};
```

## Testing Checklist

- [ ] Package builds without errors
- [ ] Robot launches in Gazebo
- [ ] Red spheres visible at LED positions
- [ ] LED service responds to `true` calls
- [ ] LED service responds to `false` calls
- [ ] Lights turn on (red glow visible)
- [ ] Lights turn off (no glow)
- [ ] Python script works
- [ ] Service is callable via `rosservice call`

## Troubleshooting

**LEDs not visible in Gazebo:**
- Ensure Gazebo is running before starting LED node
- Check that the LED links are properly created (check in RViz)

**Service calls failing:**
- Verify `/gazebo/set_light_properties` service is available: `rosservice list`
- Check node logs: `rostopic pub` to the service directly

**LEDs not responding to service calls:**
- Check if LED node is running: `rosnode list`
- Verify service name: `rosservice list | grep led`
- Check node output: add `output="screen"` in launch file

## Future Enhancements

- [ ] Individual LED control service
- [ ] LED color/brightness control
- [ ] Animation/blinking patterns
- [ ] LED status feedback topic
- [ ] Web interface (rosbridge + web UI)
- [ ] ROS2 migration
- [ ] LED position calibration tool
- [ ] Hardware integration (real LED control)

## Compatibility

- **ROS Version:** ROS 1 (Noetic recommended)
- **Robot Platforms:** PMB2, TiAGo with PMB2 base
- **Simulation:** Gazebo 11+
- **Tested With:** Ubuntu 20.04 LTS

## License

Apache License 2.0
