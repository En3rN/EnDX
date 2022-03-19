#pragma once

//case En3rN::DX::Texture::Type::RotateCubeMap:
//{
//	tdesc.Width;
//	tdesc.Height; // /= 3
//	tdesc.ArraySize = 6;
//	tdesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
//	UINT sysMemPitch = subres.SysMemPitch = 12;
//	srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//	Rect rect;
//	rect.w = tdesc.Width / 4;
//	rect.h = tdesc.Height / 3;
//
//	ScratchImage cube;
//	cube.InitializeCube(simg.GetMetadata().format, rect.w, rect.h, 1, 1);
//
//	std::vector<uint16_t> srcImgIndexes{ 6,4,1,9,7,5 };
//	D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_X;
//	D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_X;
//	D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_Y;
//	D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_Y;
//	D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_Z;
//	D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_Z;
//
//	D3D11_SUBRESOURCE_DATA subres[6]{};
//	for (auto i = 0; i < 6; i++)
//	{
//		auto& srcImgIndex = srcImgIndexes[i];
//		rect.x = (srcImgIndex % 4) * rect.w;
//		rect.y = (srcImgIndex / 4) * rect.w;
//		subres[i].SysMemPitch = sysMemPitch;
//		DirectX::CopyRectangle(*simg.GetImages(), rect, *cube.GetImage(0, i, 0), TEX_FILTER_DEFAULT, 0, 0);
//	}
//
//	ScratchImage rotated[6];
//	int i = 0;
//	FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_HORIZONTAL, rotated[i]); i++;
//	FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_HORIZONTAL, rotated[i]); i++;
//	FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_VERTICAL, rotated[i]); i++;
//	FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_VERTICAL, rotated[i]); i++;
//	FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_HORIZONTAL, rotated[i]); i++;
//	FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_HORIZONTAL, rotated[i]); i++;
//	rect.x = 0; rect.y = 0;
//	simg.InitializeCube(cube.GetMetadata().format, rect.w, rect.h, 1, 1);
//	for (auto i = 0; i < 6; i++)
//		CopyRectangle(*rotated[i].GetImages(), rect, *simg.GetImage(0, i, 0), TEX_FILTER_DEFAULT, 0, 0);
//
//	break;
//}

