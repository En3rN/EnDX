#include "Event.h"
#include "EnDX.h"
namespace En3rN
{

	void En3rN::EventListener::Register()
	{
		auto& eh = DX::Window::GetEventHandler();
		eh.Register(this);
	}

	void En3rN::EventListener::Unregister()
	{
		auto& eh = DX::Window::GetEventHandler();
		eh.Unregister(this);
	}
	
	void EventHandler::AddEvent(Event& event)
	{
		eventQue.push_back(event);
	}
	void EventHandler::Register(EventListener* listener)
	{
		listeners.push_back(listener);
	}
	void EventHandler::Unregister(EventListener* listener)
	{
		//auto it = std::remove_if(listeners.begin(), listeners.end(), listener);
	}

	void EventHandler::ProcessEvents()
	{
		while (!eventQue.empty())
		{
			for (auto listener : listeners)
			{
				if (listener->OnEvent(eventQue.front())) break;
			}
			eventQue.pop_front();
		}
	}

	En3rN::Event::Event(Category category,Event::Type type, WPARAM wparam, LPARAM lparam) :
		category(category), type(type), status(Event::Status::NotHandled), wparam(wparam), lparam(lparam)
	{
		DX::Window::GetEventHandler().AddEvent(*this);
	}

}
