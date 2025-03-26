#include <windows.h>
#include "Core/Engine.h"

using Core::Engine;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    try {
        auto* engine = new Engine();
		engine->Initialize();
		engine->Run();
		engine->Shutdown();
		delete engine;
        return 0;
	}
    catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
}