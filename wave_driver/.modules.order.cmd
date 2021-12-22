cmd_/home/pi/project/wave_driver/modules.order := {   echo /home/pi/project/wave_driver/wave_driver.ko; :; } | awk '!x[$$0]++' - > /home/pi/project/wave_driver/modules.order
