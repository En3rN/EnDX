#pragma once
#include "iHandle.h"
#include "enWin.h"
#include <vector>
#include <deque>

namespace En3rN
{
	class Event : shPtr(Event) , public std::enable_shared_from_this<Event>
	{
	public:
		using Que = std::deque<Event::handle>;
		enum class Category { Application, Window, Keyboard, Mouse };
		enum class Type {KeyDown, KeyUp, Move, WheelUp, WheelDown, RawCapture, FullScreen};
		enum class Status {NotHandled, Dispatched, Handled};
		Event(Category	category, Type type,  WPARAM wparam, LPARAM lparam);
		static Event::handle Create(Event::Category	category, Event::Type type, WPARAM wparam, LPARAM lparam);

		Category	category;
		Type		type;
		Status		status;
		WPARAM		wparam;
		LPARAM		lparam;
	};
	class EventListener
	{
	public:
		using Container = std::vector<EventListener*>;
		virtual bool OnEvent(Event& e) = 0;
		virtual void Register();
		virtual void Unregister();
	};
	class EventHandler : unPtr(EventHandler)
	{
	public:
		void AddEvent(Event::handle event);
		void Register(EventListener* listener);
		void Unregister(EventListener* listener);
		void ProcessEvents();
	private:
		EventListener::Container listeners;
		Event::Que eventQue;
	};
	
}

