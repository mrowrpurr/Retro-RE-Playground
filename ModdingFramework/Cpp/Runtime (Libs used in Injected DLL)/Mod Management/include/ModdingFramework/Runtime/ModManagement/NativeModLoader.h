#pragma once

#include <Logging.h>
#include <ModdingFramework/Runtime/Core/Mod.h>

#include <filesystem>

namespace ModdingFramework::Runtime::ModManagement::NativeModLoader {

    // Lazy for now...
    std::unordered_map<std::string, HMODULE> _dllModules;

    bool Load(const Core::Mod& mod) {
        Log("Loading native mod: {}", mod.GetName());

        auto dllPath = mod.GetPath() / mod.GetSource();
        if (!std::filesystem::exists(dllPath)) {
            Log("Native mod DLL not found: {}", dllPath.string());
            return false;
        }

        auto dll = LoadLibraryA(dllPath.string().c_str());
        if (!dll) {
            Log("Failed to load native mod DLL: {}", dllPath.string());
            return false;
        }

        _dllModules[mod.GetName()] = dll;

        auto loadMod = (void (*)())GetProcAddress(dll, "Load");
        if (!loadMod) {
            Log("Failed to find Load function in native mod DLL: {}", dllPath.string());
            return false;
        }

        try {
            loadMod();
            Log("Loaded native mod DLL: {}", dllPath.string());
            return true;
        } catch (const std::exception& e) {
            Log("Failed to load native mod DLL: {}", e.what());
        } catch (...) {
            Log("Failed to load native mod DLL: Unknown error");
        }
        return false;
    }

    bool Unload(const Core::Mod& mod) {
        Log("Unloading native mod: {}", mod.GetName());

        auto dllPath = mod.GetPath() / mod.GetSource();
        if (!std::filesystem::exists(dllPath)) {
            Log("Native mod DLL not found: {}", dllPath.string());
            return false;
        }

        auto dll = _dllModules[mod.GetName()];
        if (!dll) {
            Log("Native mod DLL not loaded: {}", dllPath.string());
            return false;
        }

        auto unloadMod = (void (*)())GetProcAddress(dll, "Unload");
        if (!unloadMod) {
            Log("Failed to find Unload function in native mod DLL: {}", dllPath.string());
            return false;
        }

        try {
            unloadMod();
            Log("Unloaded native mod DLL: {}", dllPath.string());
            FreeLibrary(dll);
            _dllModules.erase(mod.GetName());
            Log("FreeLibrary DLL: {}", dllPath.string());
            return true;
        } catch (const std::exception& e) {
            Log("Failed to unload native mod DLL: {}", e.what());
        } catch (...) {
            Log("Failed to unload native mod DLL: Unknown error");
        }
        return false;
    }
}