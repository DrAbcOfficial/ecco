#include <vector>
#include <filesystem>

#include "api/IEccoPlugin.h"
#include "scripts/script_system.h"
#include "scripts/CEccoScriptSystem.h"
#include "CEccoFuncs.h"

#include "plugin_system.h"

#include "meta_utility.h"


std::vector<IEccoPlugin*> g_aryPlugins;
static std::vector<void*> g_aryPluginHandles;

void CallPluginQuery() {
    extern CEccoFuncs g_pEccoFuncs;
    for (IEccoPlugin* plugin : g_aryPlugins) {
        plugin->Query(GetEccoScriptSystem(), &g_pEccoFuncs, &g_engfuncs, gpGlobals, gpMetaGlobals, gpGamedllFuncs, gpMetaUtilFuncs, &Plugin_info);
    }
}
void CallPluginInit() {
    extern CEccoFuncs g_pEccoFuncs;
    for (IEccoPlugin* plugin : g_aryPlugins) {
        plugin->Initialize();
    }
}

void LoadPlugins() {
        std::filesystem::path pluginDir(EccoMetaUtility::GetGameDir());
        pluginDir.append("addons/ecco/extensions");
        if (!std::filesystem::exists(pluginDir))
            return;
        for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
            if (!entry.is_regular_file())
                continue;
#ifdef _WIN32
            if (entry.path().extension() != ".dll") {
                continue;
            }
#else
            if (entry.path().extension() != ".so") {
                continue;
            }
#endif
            void* handle = nullptr;
#ifdef _WIN32
            handle = LoadLibraryW(entry.path().c_str());
#else
            handle = dlopen(entry.path().string().c_str(), RTLD_LAZY);
#endif
            if (!handle) 
                continue;
            IEccoPlugin* (*GetEccoPluginInstance)() = nullptr;
#ifdef _WIN32
            GetEccoPluginInstance = reinterpret_cast<IEccoPlugin * (*)()>(
                GetProcAddress(static_cast<HMODULE>(handle), "GetEccoPluginInstance")
                );
#else
            GetEccoPluginInstance = reinterpret_cast<IEccoPlugin * (*)()>(
                dlsym(handle, "GetEccoPluginInstance")
                );
#endif
            if (GetEccoPluginInstance) {
                IEccoPlugin* plugin = GetEccoPluginInstance();
                if (plugin) {
                    g_aryPlugins.push_back(plugin);
                    g_aryPluginHandles.push_back(handle);
                    continue;
                }
            }
#ifdef _WIN32
            FreeLibrary(static_cast<HMODULE>(handle));
#else
            dlclose(handle);
#endif
        }
}

void UnloadPlugins() {
    for (IEccoPlugin* plugin : g_aryPlugins) {
		plugin->Shutdown();
    }
    g_aryPlugins.clear();
    for (void* handle : g_aryPluginHandles) {
#ifdef _WIN32
        FreeLibrary(static_cast<HMODULE>(handle));
#else
        dlclose(handle);
#endif
    }
    g_aryPluginHandles.clear();
}