#pragma once

#include "Common/Common.h"

namespace KKAM::Graphics {
	class ShaderBlob {
	public:
		static ComPtr<ID3DBlob> LoadShaderBlob(const std::string& path, const std::string& entryPoint, const std::string& shaderModel) {
			ComPtr<ID3DBlob> shaderBlob;
			auto csoPath = path + ".cso";
			HRESULT hr;
			if (std::filesystem::exists(csoPath)) {
				hr = D3DReadFileToBlob(StringToWString(csoPath).c_str(), shaderBlob.GetAddressOf());
				if (FAILED(hr)) {
					throw std::runtime_error("Failed to read shader from file");
				}
				OutputDebugStringA("Shader loaded successfully\n");
			}
			else {
				hr = D3DCompileFromFile(
					StringToWString(path + ".hlsl").c_str(),
					nullptr,
					nullptr,
					entryPoint.c_str(),
					shaderModel.c_str(),
					0,
					0,
					shaderBlob.GetAddressOf(),
					nullptr
				);
				if (FAILED(hr)) {
					throw std::runtime_error("Failed to compile shader");
				}
				OutputDebugStringA("Shader compiled successfully\n");
			}
			return shaderBlob;
		}
	};
}