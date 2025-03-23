#pragma once

#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>

namespace KKAM::Core {
	using std::string;
	using std::wstring;
	struct AppWindowSettings {
		int Width;
		int Height;
		wstring Title;
	};

	struct WinProcHook {
		UINT Msg;
		std::vector<std::function<LRESULT(UINT, WPARAM, LPARAM)>> Procs;
	};

	enum class WinEvProcResult {
		OK,
		ERR,
		SHUTDOWN
	};

	class AppWindow {
	public:
		~AppWindow();
		/// <summary>
		/// Sets up the window
		/// </summary>
		/// <param name="WinSettings"></param>
		bool Initialize(const AppWindowSettings& WinSettings);
		/// <summary>
		/// Processes the window events, and returns the status of the process
		/// </summary>
		/// <returns></returns>
		WinEvProcResult Process();
		/// <summary>
		/// Frees the window resources
		/// </summary>
		void Shutdown();
		/// <summary>
		/// Registers a process message under the provided name, and calls all functions hooked onto that name
		/// </summary>
		/// <param name="Msg"></param>
		/// <param name="ProcName"></param>
		void RegisterProc(UINT Msg, string ProcName);
		/// <summary>
		/// Creates a function with a unique name, and hooks it to the provided process name
		/// </summary>
		/// <param name="ProcName"></param>
		/// <param name="HookName"></param>
		/// <param name="Proc"></param>
		void HookProc(string ProcName, string HookName, std::function<LRESULT(UINT, WPARAM, LPARAM)> Proc);

		/// <summary>
		/// Get monitor-aware information
		/// </summary>
		/// <param name="OutRect"></param>
		void GetMonitorArea(RECT& OutRect);
		/// <summary>
		/// Places the window in the center of the screen
		/// </summary>
		void CenterWindow();
		/// <summary>
		/// Handles the DPI change
		/// </summary>
		/// <param name="NewDPI"></param>
		void HandleDPI(int NewDPI);

		// Getters

		HWND GetHWND() const { return WinHWND_; }
		HMONITOR GetMonitor() const { return WinMonitor_; }
		int GetWidth() const { return Width_; }
		int GetHeight() const { return Height_; }
		int GetDPI() const { return DPI_; }
		bool IsRunning() const { return IsRunning_; }
		bool IsShuttingDown() const { return IsShuttingDown_; }
		const string& GetTitle() const { return Title; }
	private:
		// Window Objects
		HWND WinHWND_ = nullptr;
		MSG WinMSG_{};
		HMONITOR WinMonitor_ = nullptr;

		// Window variables
		std::unordered_map<string, WinProcHook> Procs_;

		bool IsRunning_ = false;
		bool IsShuttingDown_ = false;

		int Width_ = 800;
		int Height_ = 600;
		string Title = "NULL";

		int DPI_ = 0;

		// Helpers

		/// <summary>
		/// Updates the current monitor info
		/// </summary>
		void UpdateMonitorInfo();

		static LRESULT CALLBACK WinProc(HWND HWnd, UINT UMsg, WPARAM WParam, LPARAM LParam);
	};
} // namespace KKAM::Core