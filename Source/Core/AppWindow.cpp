#include "Core/AppWindow.h"

namespace KKAM::Core {
	AppWindow::~AppWindow() {
		Shutdown();
	}
	bool AppWindow::Initialize(const AppWindowSettings& WinSettings) {
		WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpszClassName = L"KKAMWINDOWCLASS";
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpfnWndProc = &AppWindow::WinProc;

		if (!RegisterClassEx(&wc)) {
			return false;
		}

		WinHWND_ = CreateWindowEx(0, wc.lpszClassName, WinSettings.Title.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, WinSettings.Width, WinSettings.Height, NULL, NULL, wc.hInstance, this);

		if (!WinHWND_) {
			return false;
		}

		Width_ = WinSettings.Width;
		Height_ = WinSettings.Height;
		Title_.assign(WinSettings.Title.begin(), WinSettings.Title.end());

		ShowWindow(WinHWND_, SW_SHOW);
		UpdateWindow(WinHWND_);

		return true;
	}
	WinEvProcResult AppWindow::Process() {
		while (PeekMessage(&WinMSG_, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&WinMSG_);
			DispatchMessage(&WinMSG_);

			std::cout << "Message: " << WinMSG_.message << std::endl;

			if (WinMSG_.message == WM_QUIT) {
				return WinEvProcResult::SHUTDOWN;
			}
		}
		return WinEvProcResult::OK;
	}
	void AppWindow::Shutdown() {
		if (WinHWND_) {
			DestroyWindow(WinHWND_);
			UnregisterClass(L"KKAMWINDOWCLASS", GetModuleHandle(NULL));
			WinHWND_ = NULL;
		}
	}
	void AppWindow::RegisterProc(UINT Msg, string ProcName) {
		Procs_[ProcName] = WinProcHook{ Msg, {} };
	}
	void AppWindow::HookProc(string ProcName, string HookName, std::function<void(UINT, WPARAM, LPARAM)> Proc) {
		Procs_[ProcName].Procs.push_back(Proc);
	}

	LRESULT CALLBACK AppWindow::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		AppWindow* pThis = NULL;

		if (msg == WM_NCCREATE) {
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<AppWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}
		else {
			// For all other messages, get the pointer we stored
			pThis = reinterpret_cast<AppWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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