#pragma once
#include "iBindable.h"
#include "enexception.h"
#include "enString.h"
#include "Config.h"
#include <d3dcompiler.h>
#include <filesystem>

namespace En3rN::DX
{	
	class Shader : public Bindable
	{
	public:
		ID3DBlob* GetBlob() { return shaderBlob.Get(); }
		LPVOID	GetBufferPointer() { return shaderBlob->GetBufferPointer(); }
		SIZE_T	GetBufferSize() { return shaderBlob->GetBufferSize(); }

	protected:
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3D11ShaderReflection> reflection;
		
		Shader(std::filesystem::path filename) : m_passName(filename.string())
		{			
			std::filesystem::path file = Config::GetFolders().shaderFolder / filename;
			errchk::hres(D3DReadFileToBlob(filename.wstring().c_str(), &shaderBlob));
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
		PixelShader(std::filesystem::path filename) : Shader::Shader(filename)
		{
			errchk::hres(pDevice->CreatePixelShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, &pPixelShader)
				);
		}
		//PASS::GetPassName , Material::GetEntryPoint()
		PixelShader(std::string passName, std::string entryPoint)
		{
			using namespace std::string_literals;
			std::filesystem::path hlslfile("PS"s + passName + ".hlsl"s);
			std::filesystem::path csofile (Config::GetFolders().shaderFolder / ("PS" + passName + entryPoint + ".cso"s));
			UINT flags{};
#ifdef _DEBUG
			flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // DEBUG
			ComPtr<ID3DBlob> errBlob;
			HRESULT hres = D3DCompileFromFile(
				hlslfile.wstring().c_str(),
				nullptr,	//macro 
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	//include interface
				entryPoint.c_str(),
				"ps_5_0",
				flags,
				NULL,
				&shaderBlob,
				&errBlob);
			if(hres != S_OK)
				throw EnExcept((char*)errBlob->GetBufferPointer());
			errchk::hres(pDevice->CreatePixelShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, &pPixelShader));
		}
		PixelShader() = default;
		PixelShader(const PixelShader & other) = default;
		PixelShader(PixelShader && other) noexcept = default;
		PixelShader& operator = (const PixelShader & other) = default;
		PixelShader& operator = (PixelShader && other) noexcept = default;
		~PixelShader() = default;		
		void Bind() override 
		{
			pContext->PSSetShader(pPixelShader.Get(), 0, 0);
		}
		void Reflect() override;
	private:
		ComPtr<ID3D11PixelShader> pPixelShader;
	};

	class NullPixelShader : public PixelShader
	{
		void Bind() override { pContext->PSSetShader(nullptr, nullptr, 0); }
		void Reflect() override{};

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
				&pVertexShader));
		}
		//PASS::GetPassName , Material::GetEntryPoint()
		VertexShader(std::string passName, std::string entryPoint)
		{
			UINT flags{};
#ifdef _DEBUG
			flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // DEBUG
			using namespace std::string_literals;
			std::filesystem::path hlslfile("VS"s + passName + ".hlsl"s);
			std::string filename("VS" + passName + ".hlsl");
			ComPtr<ID3DBlob> errBlob;
			errchk::hres(D3DCompileFromFile(
				hlslfile.wstring().c_str(),
				nullptr,	//macro 
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	//include interface
				entryPoint.c_str(),
				"vs_5_0",
				flags,
				NULL,
				&shaderBlob,
				&errBlob));

			errchk::hres(pDevice->CreateVertexShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, &pVertexShader));
		}
		VertexShader() = default;
		VertexShader(const VertexShader & other) = default;
		VertexShader(VertexShader && other) noexcept = default;
		VertexShader& operator = (const VertexShader & other) = default;
		VertexShader& operator = (VertexShader && other) noexcept = default;
		~VertexShader() = default;		
		void Bind() override { pContext->VSSetShader(pVertexShader.Get(), 0, 0); }
		void Reflect() override;
		std::vector<std::string> GetSignatures();

	private:
		ComPtr<ID3D11VertexShader> pVertexShader;
	};
	class ComputeShader : public Shader
	{
	public:
		
		using handle = std::shared_ptr<ComputeShader>;
		ComputeShader(std::filesystem::path filename) : Shader::Shader(filename)
		{
			errchk::hres(pDevice->CreateComputeShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, &pShader)
			);
		}
		//PASS::GetPassName , Material::GetEntryPoint()
		ComputeShader(std::string passName, std::string entryPoint)
		{
			using namespace std::string_literals;
			std::filesystem::path hlslfile("PS"s + passName + ".hlsl"s);
			std::filesystem::path csofile (Config::GetFolders().shaderFolder / ("PS" + passName + entryPoint + ".cso"s));
			UINT flags{};
#ifdef _DEBUG
			flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // DEBUG
			ComPtr<ID3DBlob> errBlob;
			HRESULT hres = D3DCompileFromFile(
				hlslfile.wstring().c_str(),
				nullptr,	//macro 
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	//include interface
				entryPoint.c_str(),
				"cs_5_0",
				flags,
				NULL,
				&shaderBlob,
				&errBlob);
			if(hres != S_OK)
				throw EnExcept((char*)errBlob->GetBufferPointer());
			errchk::hres(pDevice->CreateComputeShader(
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				nullptr, &pShader));
		}
		ComputeShader() = default;
		ComputeShader(const ComputeShader& other) = default;
		ComputeShader(ComputeShader&& other) noexcept = default;
		ComputeShader& operator = (const ComputeShader& other) = default;
		ComputeShader& operator = (ComputeShader&& other) noexcept = default;
		~ComputeShader() = default;
		void Bind() override
		{
			pContext->CSSetShader(pShader.Get(), nullptr, 0);
		}
		void Reflect() override {}
	private:
		ComPtr<ID3D11ComputeShader> pShader;

	};
}
