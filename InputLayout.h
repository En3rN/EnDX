#pragma once

#include "enexception.h"
#include "BindableManager.h"
#include <vector>

namespace En3rN::DX
{
	
	class InputLayout : public Bindable
	{
	public:
		using handle = std::shared_ptr<InputLayout>;
		InputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> ied, ID3DBlob* shaderBlob, const std::string& modelName)
		{
			errchk::hres(pDevice->CreateInputLayout(
				ied.data(), std::size(ied),
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				&pInputLayout),
				EnExParam);
		}
		InputLayout(const std::vector<std::string>& signatures, ID3DBlob* shaderBlob, const std::string& modelName)
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> ied{};
			for (auto& signature : signatures)
				ied.emplace_back(element_desc[signature]);
			errchk::hres(pDevice->CreateInputLayout(
				ied.data(), std::size(ied),
				shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(),
				&pInputLayout),
				EnExParam);
		}
		InputLayout(InputLayout&& other) noexcept = default;
		static std::string GetKey(std::vector<D3D11_INPUT_ELEMENT_DESC> ied, ID3DBlob* shaderBlob, const std::string& modelName )
		{
			std::string elementString;
			for (const auto& e : ied)
				elementString += e.SemanticName;
			return typeid(InputLayout).name()+elementString;
		}
		static InputLayout::handle Query(std::vector<D3D11_INPUT_ELEMENT_DESC> ied, ID3DBlob* shaderBlob, const std::string& modelName )
		{
			return BindableManager::Query<InputLayout>(ied, shaderBlob, modelName);
		}
		void Bind() override
		{
			pContext->IASetInputLayout(pInputLayout.Get());
		}
	private:
		static std::map<std::string, D3D11_INPUT_ELEMENT_DESC> element_desc;
		ComPtr<ID3D11InputLayout> pInputLayout;
	};

	
}
