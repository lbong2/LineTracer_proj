#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("motor driver\n");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class* my_class;
static struct cdev my_device;

#define DRIVER_NAME "my_motor"
#define DRIVER_CLASS "MyModuleClass_motor"

 /**
  * @brief Write data to buffer
  */
 static ssize_t driver_write(struct file* File, const char* user_buffer, size_t count, loff_t* offs) {
	 int to_copy, not_copied, delta;
	 char value[4];

	 /* Get amount of data to copy */
	 to_copy = min(count, sizeof(value));

	 /* Copy data to user */
	 not_copied = copy_from_user(value, user_buffer, to_copy);

	 /* Setting the LED */
	 switch(value[0])
	 {
		 case '0':
			gpio_set_value(23, 0);
			break;
		 case '1':
			gpio_set_value(23, 1);
			break;
	 }
	 switch(value[1])
	 {
		 case '0':
			gpio_set_value(24, 0);
			break;
		 case '1':
			gpio_set_value(24, 1);
			break;
	 }
	 switch(value[2])
	 {
		 case '0':
			gpio_set_value(8, 0);
			break;
		 case '1':
			gpio_set_value(8, 1);
			break;
	 }
	 switch(value[3])
	 {
		 case '0':
			gpio_set_value(7, 0);
			break;
		 case '1':
			gpio_set_value(7, 1);
			break;
	 }
		 
	 /* Calculate data */
	 delta = to_copy - not_copied;
	 return delta;
 }

 /**
  * @brief This function is called, when the device file is opened
  */
 static int driver_open(struct inode* device_file, struct file* instance) {
	 printk("led_button - open was called!\n");
	 return 0;
 }

 /**
  * @brief This function is called, when the device file is opened
  */
 static int driver_close(struct inode* device_file, struct file* instance) {
	 printk("led_button - close was called!\n");
	 return 0;
 }

 static struct file_operations fops = {
	 .owner = THIS_MODULE,
	 .open = driver_open,
	 .release = driver_close,
	 //.read = driver_read,
	 .write = driver_write
 };

 /**
  * @brief This function is called, when the module is loaded into the kernel
  */
 static int __init ModuleInit(void) {
	 printk("Hello, Kernel!\n");

	 /* Allocate a device nr */
	 if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		 printk("Device Nr. could not be allocated!\n");
		 return -1;
	 }
	 printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	 /* Create device class */
	 if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		 printk("Device class can not e created!\n");
		 goto ClassError;
	 }

	 /* create device file */
	 if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		 printk("Can not create device file!\n");
		 goto FileError;
	 }

	 /* Initialize device file */
	 cdev_init(&my_device, &fops);

	 /* Regisering device to kernel */
	 if (cdev_add(&my_device, my_device_nr, 1) == -1) {
		 printk("Registering of device to kernel failed!\n");
		 goto AddError;
	 }

	 /* GPIO 23 init */
	 if (gpio_request(23, "rpi-gpio-23")) {
		 printk("Can not allocate GPIO 23\n");
		 goto AddError;
	 }

	 /* Set GPIO 23 direction */
	 if (gpio_direction_output(23, 0)) {
		 printk("Can not set GPIO 23 to output!\n");
		 goto Gpio23Error;
	 }

	 /* GPIO 24 init */
	 if (gpio_request(24, "rpi-gpio-24")) {
		 printk("Can not allocate GPIO 24\n");
		 goto AddError;
	 }

	 /* Set GPIO 24 direction */
	 if (gpio_direction_output(24, 0)) {
		 printk("Can not set GPIO 24 to output!\n");
		 goto Gpio24Error;
	 }
	 
	 /* GPIO 8 init */
	 if (gpio_request(8, "rpi-gpio-8")) {
		 printk("Can not allocate GPIO 8\n");
		 goto AddError;
	 }

	 /* Set GPIO 8 direction */
	 if (gpio_direction_output(8, 0)) {
		 printk("Can not set GPIO 8 to output!\n");
		 goto Gpio8Error;
	 }
	 
	 /* GPIO 7 init */
	 if (gpio_request(7, "rpi-gpio-7")) {
		 printk("Can not allocate GPIO 7\n");
		 goto AddError;
	 }

	 /* Set GPIO 7 direction */
	 if (gpio_direction_output(7, 0)) {
		 printk("Can not set GPIO 7 to output!\n");
		 goto Gpio7Error;
	 }

	 return 0;
 Gpio23Error:
	 gpio_free(23);
 Gpio24Error:
	 gpio_free(24);
 Gpio8Error:
	 gpio_free(8);
 Gpio7Error:
	 gpio_free(7);
 AddError:
	 device_destroy(my_class, my_device_nr);
 FileError:
	 class_destroy(my_class);
 ClassError:
	 unregister_chrdev_region(my_device_nr, 1);
	 return -1;
 }

 /**
  * @brief This function is called, when the module is removed from the kernel
  */
 static void __exit ModuleExit(void) {
	 gpio_free(23);
	 gpio_free(24);
	 gpio_free(8);
	 gpio_free(7);
	 cdev_del(&my_device);
	 device_destroy(my_class, my_device_nr);
	 class_destroy(my_class);
	 unregister_chrdev_region(my_device_nr, 1);
	 printk("Goodbye, Kernel\n");
 }

 module_init(ModuleInit);
 module_exit(ModuleExit);


