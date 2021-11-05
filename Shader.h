#pragma once
#include "iBindable.h"
#include "enexception.h"
#include "enString.h"
#include <d3dcompiler.h>
#include <filesystem>

namespace En3rN::DX
{	
	class Shader : public Bindable
	{
	public:
		static std::wstring shaderFolder;
		ID3DBlob* GetBlob() { return shaderBlob.Get(); }
		LPVOID	GetBufferPointer() { return shaderBlob->GetBufferPointer(); }
		SIZE_T	GetBufferSize() { return shaderBlob->GetBufferSize(); }

	protected:
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3D11ShaderReflection> reflection;
		
		Shader(std::string filename) : m_passName(filename)
		{
			std::wstring file = shaderFolder + L"\\Shaders\\" + stringconverter::wstr(filename);
			errchk::hres(D3DReadFileToBlob(stringconverter::wstr(filename).c_str(), &shaderBlob), EnExParam);			
		}
		Shader() = default;
		Shader(const Shader& other) = default;
		Shader(Shader&& other) = default;
		virtual ~Shader() {};

		virtual void Reflect() = 0;
		std::string m_passName;
	};

	class PixelShader : public Shader
	{
	public:
		struct Include : public ID3DInclude
		{
			STDMETHOD(Open)(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
			STDMETHOD(Close)(THIS_ LPCVOID pData);
		};
		using handle = std::shared_ptr<PixelShader>;
		PixelShader(std::string filename) : Shader::Shader(filename)
		{
			errchk::hres(pDevice->CreatePixelShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, &pPixelShader),
				EnExParam);
		}
		//PASS::GetPassName , Material::GetEntryPoint()
		PixelShader(std::string passName, std::string entryPoint)
		{
			std::string filename("PS" + passName + ".hlsl");
			ComPtr<ID3DBlob> errBlob;
			errchk::hres(D3DCompileFromFile(
				stringconverter::wstr(filename).c_str(), 
				nullptr,	//macro 
				nullptr,	//include interface
				entryPoint.c_str(),
				"ps_5_0", 
				D3DCOMPILE_ENABLE_STRICTNESS,
				NULL,
				&shaderBlob,
				&errBlob),
				EnExParam);

			errchk::hres(pDevice->CreatePixelShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, &pPixelShader),
				EnExParam);

		}
		PixelShader() = default;
		PixelShader(const PixelShader & other) = default;
		PixelShader(PixelShader && other) noexcept = default;
		PixelShader& operator = (const PixelShader & other) = default;
		PixelShader& operator = (PixelShader && other) noexcept = default;
		~PixelShader() = default;
		static std::string GetKey(std::wstring filename)
		{
			return typeid(PixelShader).name() + stringconverter::str(filename);			
		}
		void Bind() override {pContext->PSSetShader(pPixelShader.Get(), 0, 0);}
		void Reflect() override;
	private:
		ComPtr<ID3D11PixelShader> pPixelShader;
	};
	
	class VertexShader : public Shader
	{
	public:
		using handle = std::shared_ptr<VertexShader>;
		VertexShader(std::string filename) : Shader::Shader(filename)
		{			
			errchk::hres(pDevice->CreateVertexShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, 
				&pVertexShader),
				EnExParam);
		}
		//PASS::GetPassName , Material::GetEntryPoint()
		VertexShader(std::string passName, std::string entryPoint)
		{
			std::string filename("VS" + passName + ".hlsl");
			ComPtr<ID3DBlob> errBlob;
			errchk::hres(D3DCompileFromFile(
				stringconverter::wstr(filename).c_str(),
				nullptr,	//macro 
				nullptr,	//include interface
				entryPoint.c_str(),
				"vs_5_0",
				D3DCOMPILE_ENABLE_STRICTNESS,
				NULL,
				&shaderBlob,
				&errBlob),
				EnExParam);

			errchk::hres(pDevice->CreateVertexShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, &pVertexShader),
				EnExParam);
		}
		VertexShader() = default;
		VertexShader(const VertexShader & other) = default;
		VertexShader(VertexShader && other) noexcept = default;
		VertexShader& operator = (const VertexShader & other) = default;
		VertexShader& operator = (VertexShader && other) noexcept = default;
		~VertexShader() = default;

		static std::string GetKey(std::string filename)
		{
			return { typeid(VertexShader).name() + filename };			
		}
		void Bind() override { pContext->VSSetShader(pVertexShader.Get(), 0, 0); }
		void Reflect() override;
		std::vector<std::string> GetSignatures();

	private:
		ComPtr<ID3D11VertexShader> pVertexShader;
	};
}
