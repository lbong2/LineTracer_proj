cmd_/home/pi/project/motor_driver/modules.order := {   echo /home/pi/project/motor_driver/motor_driver.ko; :; } | awk '!x[$$0]++' - > /home/pi/project/motor_driver/modules.order
