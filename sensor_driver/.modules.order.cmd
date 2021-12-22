cmd_/home/pi/project/sensor_driver/modules.order := {   echo /home/pi/project/sensor_driver/sensor_driver.ko; :; } | awk '!x[$$0]++' - > /home/pi/project/sensor_driver/modules.order
