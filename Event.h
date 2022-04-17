#pragma once
#include "iHandle.h"
#include "enWin.h"
#include <queue>
#include <vector>
#include <deque>

namespace En3rN
{
	class Event : shPtr(Event) , public std::enable_shared_from_this<Event>
	{
	public:
		class Listener;
		class Handler;
		using Que = std::queue<Event>;
		enum class Category { Application, Window, Keyboard, Mouse };
		enum class Flag {KeyDown, KeyUp, Move, WheelUp, WheelDown, RawCapture, FullScreen};
		enum class Status {NotHandled, Dispatched, Handled};
		Event(Category	category, Flag type,  WPARAM wparam, LPARAM lparam);
		static Event::handle Create(Event::Category	category, Event::Flag type, WPARAM wparam, LPARAM lparam);

		Category	category;
		Flag		type;
		Status		status;
		WPARAM		wparam;
		LPARAM		lparam;
	};
	class Event::Listener
	{
	public:
		struct Base 
		{
			using Container = std::vector<Event::Listener*>;
		};
		virtual bool OnEvent(Event& e) = 0;
		virtual void Register();
		virtual void Unregister();
	private:
	};
	class Event::Handler : unPtr(Event::Handler)
	{
	public:
		void AddEvent(Event event);
		void Register(Event::Listener* listener);
		void Unregister(Event::Listener* listener);
		void ProcessEvents();
	private:
		Event::Listener::Base::Container listeners;
		Event::Que eventQue;
	};
	
}

