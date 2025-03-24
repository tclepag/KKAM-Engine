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
		wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_KKAMENGINE));
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

		RegisterProc(WM_SIZING, "WindowResize");
		RegisterProc(WM_SIZE, "WindowResize");
		RegisterProc(WM_GETMINMAXINFO, "WindowLimits", true);

		HookProc("WindowResize", "BaseWindowResize", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			Width_ = LOWORD(lParam);
			Height_ = HIWORD(lParam);
			});

		HookProc("WindowLimits", "BaseWindowLimits", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 800;
			lpMMI->ptMinTrackSize.y = 600;
			});

		ShowWindow(WinHWND_, SW_SHOW);
		UpdateWindow(WinHWND_);

		Initialized_ = true;

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
	void AppWindow::RegisterProc(UINT Msg, string ProcName, bool Override) {
		Procs_[ProcName] = WinProcHook{ Msg, {}, Override };
	}
	void AppWindow::HookProc(string ProcName, string HookName, std::function<void(UINT, WPARAM, LPARAM)> Proc) {
		for (auto& [proc, hookData] : Procs_) {
			if (proc == ProcName) {
				hookData.Procs.push_back(Proc);
			}
		}
	}

	LRESULT CALLBACK AppWindow::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		ImGuiIO& io = ImGui::GetIO();

		switch (msg) {
		case WM_LBUTTONDOWN:
			io.MouseDown[0] = true;
			return 0;
		case WM_LBUTTONUP:
			io.MouseDown[0] = false;
			return 0;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = true;
			return 0;
		case WM_RBUTTONUP:
			io.MouseDown[1] = false;
			return 0;
		case WM_MOUSEWHEEL:
			io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
			return 0;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			return 0;
		case WM_KEYDOWN:
			if (wParam < 256)
				io.AddKeyEvent(ImGuiKey(wParam), true);
			return 0;
		case WM_KEYUP:
			if (wParam < 256)
				io.AddKeyEvent(ImGuiKey(wParam), false);
			return 0;
		case WM_CHAR:
			if (wParam > 0 && wParam < 0x10000)
				io.AddInputCharacter((unsigned short)wParam);
			return 0;
		}


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
					if (hookData.Override) {
						return 0;
					}
				}
			}
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}