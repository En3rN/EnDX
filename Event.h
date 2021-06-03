#pragma once
#include "iHandle.h"
#include "enWin.h"
#include <vector>
#include <deque>

namespace En3rN
{
	class Event : shPtr(Event) 
	{
	public:
		enum class Category { Application, Window, Keyboard, Mouse };
		enum class Type {KeyDown, KeyUp, Move, WheelUp, WheelDown, RawCapture};
		enum class Status {NotHandled, Dispatched, Handled};
		Event(Category	category, Type type,  WPARAM wparam, LPARAM lparam);

		Category	category;
		Type		type;
		Status		status;
		WPARAM		wparam;
		LPARAM		lparam;
	};
	class EventListener
	{
	public:
		virtual bool OnEvent(Event& e) = 0;
		virtual void Register();
		virtual void Unregister();
	};
	class EventHandler : unPtr(EventHandler)
	{
	public:
		void AddEvent(Event & event);
		void Register(EventListener* listener);
		void Unregister(EventListener* listener);
		void ProcessEvents();
	private:
		std::vector<EventListener*> listeners;
		std::deque<Event> eventQue;
	};
	
}

