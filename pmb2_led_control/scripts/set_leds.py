#!/usr/bin/env python3
"""
Simple script to test LED control service
Usage:
  rosrun pmb2_led_control set_leds.py on   # Turn LEDs on
  rosrun pmb2_led_control set_leds.py off  # Turn LEDs off
"""

import rospy
import sys
from std_srvs.srv import SetBool

def main():
    rospy.init_node('led_control_client')
    
    # Wait for service
    service_name = '/pmb2/set_leds'
    rospy.wait_for_service(service_name)
    
    # Create service proxy
    try:
        set_leds = rospy.ServiceProxy(service_name, SetBool)
        
        if len(sys.argv) < 2:
            print("Usage: set_leds.py [on|off]")
            sys.exit(1)
        
        command = sys.argv[1].lower()
        
        if command == 'on':
            response = set_leds(True)
            print(f"Result: {response.message}")
        elif command == 'off':
            response = set_leds(False)
            print(f"Result: {response.message}")
        else:
            print("Invalid command. Use 'on' or 'off'")
            sys.exit(1)
            
    except rospy.ServiceException as e:
        print(f"Service call failed: {e}")
        sys.exit(1)

if __name__ == '__main__':
    main()
