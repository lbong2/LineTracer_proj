cmd_/home/pi/project/buzzer_driver/modules.order := {   echo /home/pi/project/buzzer_driver/buzzer_driver.ko; :; } | awk '!x[$$0]++' - > /home/pi/project/buzzer_driver/modules.order
