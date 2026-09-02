#pragma once

#if defined(_WIN32)
#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#endif

#include "../system/IApplicationLoader.h"

class ApplcationLoader: public IApplicationLoader {
private:
    void* appModule = nullptr;

public:
    void (*application_init)(SystemAPI*) = nullptr;
    void (*application_update)(SystemAPI*) = nullptr;

    bool loadApplication(const char* filepath, SystemAPI* api) {
#if defined(_WIN32)
        appModule = LoadLibraryA(filepath);
#elif defined(__linux__) || defined(__APPLE__)
        appModule = dlopen(filepath, RTLD_LAZY);
#else
        // TODO: Standalone Pico 2 OS loader implementation
        appModule = nullptr; 
#endif

        if (!appModule) return false;

#if defined(_WIN32)
        application_init = (void(*)(SystemAPI*))GetProcAddress((HMODULE)appModule, "application_init");
        application_update = (void(*)(SystemAPI*))GetProcAddress((HMODULE)appModule, "application_update");
#elif defined(__linux__) || defined(__APPLE__)
        application_init = (void(*)(SystemAPI*))dlsym(appModule, "application_init");
        application_update = (void(*)(SystemAPI*))dlsym(appModule, "application_update");
#else
        // TODO: Standalone Pico 2 OS symbol resolution
#endif

        if (application_init) application_init(api);

        return true;
    }

    void update(SystemAPI* api) {
        if (application_update) application_update(api);
    }

    void unloadApplication() {
        if (appModule) {
#if defined(_WIN32)
            FreeLibrary((HMODULE)appModule);
#elif defined(__linux__) || defined(__APPLE__)
            dlclose(appModule);
#endif
        }

        appModule  = nullptr;
        application_init   = nullptr;
        application_update = nullptr;
    }
};