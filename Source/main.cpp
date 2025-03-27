#include <windows.h>
#include "Core/KEngine.h"
#include <exception>

using Core::KEngine;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    try {
        auto* kEngine = new KEngine();
        kEngine->Initialize();
        kEngine->Run();
        kEngine->Shutdown();
		delete kEngine;
        return 0;
	}
    catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
}