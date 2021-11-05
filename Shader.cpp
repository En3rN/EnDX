#include "Shader.h"
namespace fs = std::filesystem;
namespace En3rN::DX
{
	std::wstring Shader::shaderFolder = fs::current_path();
	void PixelShader::Reflect()
	{
		
	}
	void VertexShader::Reflect()
	{
		
	}
	std::vector<std::string> VertexShader::GetSignatures()
	{
		D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &reflection);
		D3D11_SIGNATURE_PARAMETER_DESC signature{};
		std::vector<std::string> signatures{};
		UINT index{};
		while (reflection->GetInputParameterDesc(index, &signature)==S_OK)
		{
			signatures.emplace_back(signature.SemanticName);
			++index;
		}
		return signatures;
	}
	STDMETHODIMP_(HRESULT __stdcall) PixelShader::Include::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP_(HRESULT __stdcall) PixelShader::Include::Close(LPCVOID pData)
	{
		return E_NOTIMPL;
	}
}