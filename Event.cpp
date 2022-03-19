#include "Event.h"
#include "EnDX.h"
namespace En3rN
{
	Event::handle Event::Create(Event::Category category, Event::Type type, WPARAM wparam, LPARAM lparam)
	{
		return std::make_shared<Event>(category, type, wparam, lparam);
	}

	void En3rN::Event::Listener::Register()
	{
		auto& eh = DX::Window::GetEventHandler();
		eh.Register(this);
	}

	void En3rN::Event::Listener::Unregister()
	{
		auto& eh = DX::Window::GetEventHandler();
		eh.Unregister(this);
	}

	void Event::Handler::AddEvent(Event event)
	{
		eventQue.push(event);
	}
	void Event::Handler::Register(Event::Listener* listener)
	{
		listeners.push_back(listener);
	}
	void Event::Handler::Unregister(Event::Listener* listener)
	{
		for(auto it=listeners.begin();it<listeners.end();++it)
			if (*it == listener) {
				listeners.erase(it);
				break;
			}
		//std::erase_if(listeners, [&]() {it == listeners; })
	}

	void Event::Handler::ProcessEvents()
	{
		while (!eventQue.empty())
		{
			for (auto& listener : listeners)
				if (listener->OnEvent(eventQue.front())) 
					break;
			eventQue.pop();
		}
	}

	En3rN::Event::Event(Category category,Event::Type type, WPARAM wparam, LPARAM lparam) :
		category(category), type(type), status(Event::Status::NotHandled), wparam(wparam), lparam(lparam)
	{
		
	}

}
