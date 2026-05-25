#include "register_types.h"

#include "core/config/project_settings.h"
#include "core/error/error_macros.h"
#include "core/extension/gdextension.h"
#include "core/extension/gdextension_loader.h"
#include "core/extension/gdextension_manager.h"
#include "core/object/ref_counted.h"
#include "core/string/print_string.h"

// gdextension.h 已经 transitively include 了 gdextension_interface.gen.h
// (4.6 的 interface 头是 .gen.h, 由 codegen 生成, 不要直接 include gdextension_interface.h)

extern "C" {
GDExtensionBool gd_main_extension_init(
		GDExtensionInterfaceGetProcAddress p_get_proc_address,
		GDExtensionClassLibraryPtr p_library,
		GDExtensionInitialization *r_initialization);
}

namespace {

class WxMonolithLoader : public GDExtensionLoader {
	GDSOFTCLASS(WxMonolithLoader, GDExtensionLoader);

public:
	virtual Error open_library(const String &p_path) override {
		return OK;
	}

	virtual void close_library() override {}

	virtual bool is_library_open() const override {
		return true;
	}

	virtual bool has_library_changed() const override {
		return false;
	}

	virtual bool library_exists() const override {
		return true;
	}

	virtual Error initialize(
			GDExtensionInterfaceGetProcAddress p_get_proc_address,
			const Ref<GDExtension> &p_extension,
			GDExtensionInitialization *r_initialization) override {
		print_line("[wx_monolith] calling gd_main_extension_init (static link)");

		GDExtensionBool ok = gd_main_extension_init(
				p_get_proc_address,
				reinterpret_cast<GDExtensionClassLibraryPtr>(p_extension.ptr()),
				r_initialization);

		if (!ok) {
			ERR_PRINT("[wx_monolith] gd_main_extension_init returned 0 (failure)");
			return ERR_CANT_OPEN;
		}
		return OK;
	}
};

} // namespace

void initialize_wx_monolith_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
		return;
	}

	Ref<WxMonolithLoader> loader;
	loader.instantiate();

	const String synthetic_path = "wx_monolith://gd-main-extension";

	GDExtensionManager::LoadStatus status = GDExtensionManager::get_singleton()->load_extension_with_loader(
			synthetic_path, loader);

	switch (status) {
		case GDExtensionManager::LOAD_STATUS_OK:
			print_line("[wx_monolith] gd-main-extension loaded (static)");
			break;
		case GDExtensionManager::LOAD_STATUS_ALREADY_LOADED:
			print_line("[wx_monolith] gd-main-extension already loaded");
			break;
		default:
			ERR_PRINT(vformat("[wx_monolith] load failed: status=%d", status));
			break;
	}
}

void uninitialize_wx_monolith_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
		return;
	}
}