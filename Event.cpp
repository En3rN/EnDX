#include "Event.h"
#include "EnDX.h"
namespace En3rN
{
	Event::handle Event::Create(Event::Category category, Event::Type type, WPARAM wparam, LPARAM lparam)
	{
		return std::make_shared<Event>(category, type, wparam, lparam);
	}

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

	void EventHandler::AddEvent(Event::handle event)
	{
		eventQue.push_back(event);
	}
	void EventHandler::Register(EventListener* listener)
	{
		listeners.push_back(listener);
	}
	void EventHandler::Unregister(EventListener* listener)
	{
		for(auto it=listeners.begin();it<listeners.end();++it)
			if (*it == listener) {
				listeners.erase(it);
			}
		
	}

	void EventHandler::ProcessEvents()
	{
		while (!eventQue.empty())
		{
			for (auto listener : listeners)
			{
				if (listener->OnEvent(*eventQue.front())) break;
			}
			eventQue.pop_front();
		}
	}

	En3rN::Event::Event(Category category,Event::Type type, WPARAM wparam, LPARAM lparam) :
		category(category), type(type), status(Event::Status::NotHandled), wparam(wparam), lparam(lparam)
	{
		
	}

}
