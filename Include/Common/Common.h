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
#include <DirectXMath.h>

// Wrl
#include <wrl/client.h>

// Main headers
#include <vector>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <thread>

struct Vertex {
	float x, y, z;
	float r, g, b;
};
using VertexArray = std::vector<Vertex>;
using Indices = std::vector<unsigned int>;

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;