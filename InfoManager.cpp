#include "InfoManager.h"
#include "Graphics.h"
#include "enexception.h"

namespace En3rN::DX
{
	InfoManager* InfoManager::infoManager = nullptr;
	InfoManager::InfoManager(ID3D11Device& device) :infoQ{}
	{
#ifndef _DEBUG | DEBUG
		return;
#endif _DEBUG | DEBUG
		errchk::hres(device.QueryInterface<ID3D11InfoQueue>(&pInfoQ), EnExParam);
		infoManager = this;
	}

	void InfoManager::Update()
	{
#ifndef _DEBUG | DEBUG
		return;
#endif // _DEBUG | DEBUG
		if (!infoManager) return;
		auto& im = *infoManager;
		UINT64 msgs;
		if (msgs = im.pInfoQ->GetNumStoredMessages())
		{
			std::string msg;
			for (auto i = 0; i < im.pInfoQ->GetNumStoredMessages(); i++)
			{
				SIZE_T message_size = 0;
				im.pInfoQ->GetMessageA(i, nullptr, &message_size); //get the size of the message

				D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(message_size); //allocate enough space
				im.pInfoQ->GetMessageA(i, message, &message_size); //get the actual message
				if (message)im.infoQ.emplace_back(message->pDescription);
				free(message);
			}
			im.pInfoQ->ClearStoredMessages();
		}
	}

	bool InfoManager::Empty()
	{
		Update();
#ifndef _DEBUG | DEBUG
		return true;
#endif // _DEBUG | DEBUG
		if (!infoManager)  return true;
		auto& im = *infoManager;
		return im.infoQ.empty();
	}

	InfoManager::InfoQ InfoManager::GetInfo()
	{
#ifndef _DEBUG | DEBUG
		return InfoManager::InfoQ{};
#endif // _DEBUG | DEBUG
		if (!infoManager)  return InfoManager::InfoQ{};
		auto& im = *infoManager;
		return im.infoQ;
	}
}
