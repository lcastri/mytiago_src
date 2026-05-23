# PMB2 LED Control

This package provides LED control functionality for the PMB2 robot base with Gazebo visualization support. The LEDs are positioned around the base (front-left, front-right, rear-left, rear-right) and can be controlled via a ROS service.

## Features

- **4 LEDs** positioned around the base perimeter
- **Gazebo Integration** - LEDs are visualized as light sources in simulation
- **ROS Service Interface** - Simple `std_srvs/SetBool` service to control all LEDs
- **Easy Testing** - Includes Python script for quick testing

## Installation

1. Build the package:
```bash
cd ~/catkin_ws
catkin_make
```

2. Source the setup:
```bash
source ~/catkin_ws/devel/setup.bash
```

## Usage

### Method 1: Launch file
```bash
roslaunch pmb2_led_control pmb2_led_control.launch
```

### Method 2: Direct node launch
```bash
rosrun pmb2_led_control pmb2_led_control_node
```

### Method 3: Python script
```bash
# Turn LEDs ON
rosrun pmb2_led_control set_leds.py on

# Turn LEDs OFF
rosrun pmb2_led_control set_leds.py off
```

### Method 4: Direct service call
```bash
# Turn LEDs ON
rosservice call /pmb2/set_leds "data: true"

# Turn LEDs OFF
rosservice call /pmb2/set_leds "data: false"
```

## Service Details

**Service Name:** `/pmb2/set_leds`  
**Service Type:** `std_srvs/SetBool`

**Request:**
- `data` (bool): true to turn LEDs on, false to turn off

**Response:**
- `success` (bool): true if successful
- `message` (string): Status message

## LED Positions

The LEDs are positioned as follows:

- **led_front_left** - Front left corner at x=0.35m, y=0.15m
- **led_front_right** - Front right corner at x=0.35m, y=-0.15m
- **led_rear_left** - Rear left corner at x=-0.35m, y=0.15m
- **led_rear_right** - Rear right corner at x=-0.35m, y=-0.15m

All LEDs are at height z=0.10m on the base link.

## Gazebo Visualization

When running in Gazebo simulation:
1. Start your Gazebo world with the robot
2. Launch the LED control node
3. The LEDs will appear as red spheres around the base
4. When turned on, they emit a red light that illuminates the surrounding area
5. When turned off, no light is emitted

## Implementation Details

### URDF Components

- **LED Definition:** `/urdf/objects/led.urdf.xacro` - Contains LED macros
- **Base Integration:** `/urdf/base/base.urdf.xacro` - Includes and instantiates LEDs

### ROS Components

- **Node:** `led_control_node.cpp` - Handles service requests
- **Script:** `set_leds.py` - Python client for testing
- **Launch:** `pmb2_led_control.launch` - Easy startup

## Customization

### Change LED Positions

Edit `/urdf/objects/led.urdf.xacro` in the `all_leds` macro:

```xml
<xacro:led name="led_front_left"  parent="${parent}" x="0.35"  y="0.15"  z="0.10" />
```

### Change LED Colors

Edit `/urdf/objects/led.urdf.xacro` in the `led` macro:

```xml
<diffuse>0.8 0.0 0.0 1.0</diffuse>  <!-- R G B A -->
```

### Change LED Light Intensity

Edit `src/led_control_node.cpp` in the `turnOnLED()` function:

```cpp
srv.request.range = 5.0;  // Light range
srv.request.attenuation_linear = 0.1;  // Falloff
```

## Future Enhancements

- [ ] Individual LED control
- [ ] Color selection support
- [ ] Blinking/animation patterns
- [ ] LED state feedback topic
- [ ] Web interface control
- [ ] ROS2 support

## License

Apache License 2.0

## Author

Automatically generated for PMB2 Robot LED System
