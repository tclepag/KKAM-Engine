#include "Core/AppWindow.h"

namespace KKAM::Core {
	AppWindow::~AppWindow() {
		Shutdown();
	}
	bool AppWindow::Initialize(const AppWindowSettings& WinSettings) {
		WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpszClassName = WinSettings.Title.c_str();
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpfnWndProc = &AppWindow::WinProc;

		if (!RegisterClassEx(&wc)) {
			return false;
		}

		WinHWND_ = CreateWindowEx(0, wc.lpszClassName, WinSettings.Title.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, WinSettings.Width, WinSettings.Height, NULL, NULL, wc.hInstance, this);

		SetClassLongPtr(WinHWND_, GWLP_USERDATA, (LONG_PTR)this);

		return true;
	}
	WinEvProcResult AppWindow::Process() {
		// Event processing logic goes here
		while (PeekMessage(&WinMSG_, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&WinMSG_);
			DispatchMessage(&WinMSG_);
		}
		return WinEvProcResult::OK;
	}
	void AppWindow::Shutdown() {
		// Window shutdown logic goes here
	}
	void AppWindow::RegisterProc(UINT Msg, string ProcName) {
		// Register process logic goes here
	}
	void AppWindow::HookProc(string ProcName, string HookName, std::function<LRESULT(UINT, WPARAM, LPARAM)> Proc) {
		// Hook process logic goes here
	}

	LRESULT CALLBACK AppWindow::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		AppWindow* pThis = reinterpret_cast<AppWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (!pThis) {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		if (pThis) {
			for (auto& [proc, hookData] : pThis->Procs_) {
				if (hookData.Msg == msg) {
					for (auto& procFunc : hookData.Procs) {

						procFunc(msg, wParam, lParam);
					}
				}
			}
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}