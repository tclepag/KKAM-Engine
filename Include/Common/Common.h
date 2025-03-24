#pragma once

#define WIN32_LEAN_AND_MEAN

// Windows
#include <windows.h>

// DirectX
// DX11
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3d11_1.h>
#include <d3d11_3.h>
#include <d3dcompiler.h>

// Third Party
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

// ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// Wrl
#include <wrl/client.h>

// Main headers
#include <filesystem>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <thread>
#include <locale>
#include <codecvt>

struct Vertex {
	float x, y, z;
	float r, g, b;
};
using VertexArray = std::vector<Vertex>;
using Indices = std::vector<unsigned int>;

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

// Convert string to wstring
inline std::wstring StringToWString(const std::string& str) {
    if (str.empty()) {
        return std::wstring();
    }

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
    if (size_needed <= 0) {
        return std::wstring();
    }

    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), &wstr[0], size_needed);

    return wstr;
}

inline std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) {
        return std::string();
    }

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
        nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) {
        return std::string();
    }

    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
        &str[0], size_needed, nullptr, nullptr);

    return str;
}