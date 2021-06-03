#pragma once
#include "iBindable.h"
#include "enexception.h"
#include <d3dcompiler.h>
#include <filesystem>

namespace En3rN::DX
{	
	class Shader : public Bindable
	{
	public:
		static std::wstring shaderFolder;
		ID3DBlob* GetBlob() { return blob.Get(); }
	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		
		Shader(std::wstring filename)
		{
			

			std::wstring file = shaderFolder +L"\\Shaders\\"+  filename;
			errchk::hres(D3DReadFileToBlob(file.c_str(), &blob), EnExParam);
			
		}
		Shader(Shader&& other) = default;
		virtual void Reflect() = 0;
		virtual ~Shader() {};
	};
	class PixelShader : public Shader
	{
	public:
		PixelShader(std::wstring filename) : Shader::Shader(filename)
		{
			errchk::hres(pDevice->CreatePixelShader(
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				nullptr, &pPixelShader),
				EnExParam);
		}
		void Bind() override {pContext->PSSetShader(pPixelShader.Get(), 0, 0);}
		void Reflect() override;
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
	
	class VertexShader : public Shader
	{
	public:
		VertexShader(std::wstring filename) : Shader::Shader(filename)
		{
			errchk::hres(pDevice->CreateVertexShader(
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				nullptr, &pVertexShader),
				EnExParam);
		}
		void Bind() override { pContext->VSSetShader(pVertexShader.Get(), 0, 0); }
		void Reflect() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}
