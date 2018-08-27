#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x1bfd809d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x110cf8a0, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xd4d7708e, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x79c5a9f0, __VMLINUX_SYMBOL_STR(ioremap) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x9416e1d8, __VMLINUX_SYMBOL_STR(__request_region) },
	{ 0x80b1cd59, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x29ce7956, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0xfb485186, __VMLINUX_SYMBOL_STR(platform_get_resource) },
	{ 0xaa156417, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x1cd5f9ba, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xc35f62c8, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x85f74b00, __VMLINUX_SYMBOL_STR(iomem_resource) },
	{ 0x531c90ec, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xdb435c1f, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x2ab3cc9d, __VMLINUX_SYMBOL_STR(__release_region) },
	{ 0xfc39e32f, __VMLINUX_SYMBOL_STR(ioport_unmap) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cbyu,ecen427-audio_codec");
MODULE_ALIAS("of:N*T*Cbyu,ecen427-audio_codecC*");
