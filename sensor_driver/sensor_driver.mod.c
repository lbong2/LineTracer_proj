#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x44afce, "module_layout" },
	{ 0xae651774, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xf0462214, "class_destroy" },
	{ 0x22460fe9, "device_destroy" },
	{ 0xfe990052, "gpio_free" },
	{ 0x54535e55, "gpiod_direction_input" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xc2bcb805, "cdev_add" },
	{ 0xaaad239e, "cdev_init" },
	{ 0x7754488a, "device_create" },
	{ 0x84e8af7f, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x938ca289, "gpiod_get_raw_value" },
	{ 0xead0d68d, "gpio_to_desc" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "BA164B34B104F954F65DA61");
