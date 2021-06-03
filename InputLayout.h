#pragma once
#include "Drawable.h"
#include "enexception.h"
#include <vector>




namespace En3rN::DX
{
	
	class InputLayout : public Bindable
	{
	public:
		InputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>& ied, ID3DBlob* blob)
		{
			errchk::hres(pDevice->CreateInputLayout(
				ied.data(), std::size(ied),
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				&pInputLayout),
				EnExParam);
		}
		InputLayout(InputLayout&& other) noexcept = default;
		void Bind() override
		{
			pContext->IASetInputLayout(pInputLayout.Get());
		}
		static std::vector<D3D11_INPUT_ELEMENT_DESC> Position;
		static std::vector<D3D11_INPUT_ELEMENT_DESC> PositionColor;
		static std::vector<D3D11_INPUT_ELEMENT_DESC> PositionTexCoord;
		static std::vector<D3D11_INPUT_ELEMENT_DESC> PositionTexCoord3d;
	private:
		ComPtr<ID3D11InputLayout> pInputLayout;
	};

	
}