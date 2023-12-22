#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
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
__used __section(__versions) = {
	{ 0x3e549f1d, "module_layout" },
	{ 0x5138e6ba, "param_ops_int" },
	{ 0xd682f4f0, "unregister_rpmsg_driver" },
	{ 0xdaa7679, "__register_rpmsg_driver" },
	{ 0x74f8c57c, "devm_kmalloc" },
	{ 0xfd42377d, "_dev_err" },
	{ 0x87221f28, "rpmsg_send" },
	{ 0x4059792f, "print_hex_dump" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xbf69e32a, "_dev_info" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("rpmsg:rpmsg-client-sample");
