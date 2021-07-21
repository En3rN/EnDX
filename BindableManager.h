#pragma once
#include "iBindable.h"
#include <unordered_map>
#include <map>
#include <mutex>
namespace En3rN::DX
{

	class BindableManager 
	{
	public:
		template <class T, typename... ConstructorArgs>  //std::enable_if<std::is_base_of<Bindable, T>::value>
		static T::handle Query(const ConstructorArgs& ... args)
		{
			return Get().pQuery<T>(args ...);
		}
	private:
		template <class T, typename... ConstructorArgs>
		Bindable::handle pQuery(ConstructorArgs ... args)
		{
			std::scoped_lock(mtx);

			std::string key = T::GetKey(std::forward<ConstructorArgs>(args)...);

			const auto req = bindables.find(key);
			if (req == bindables.end())
			{
				auto bindable = std::make_shared<T>(std::forward<ConstructorArgs>(args) ...);
				auto item = std::make_pair(key, bindable);
				bindables.insert(item);
				return bindable;
			}
			else
			{
				return req->second;
			}

		}
		static BindableManager& Get()
		{
			static BindableManager manager;
			return manager;
		};
		std::map<std::string, Bindable::handle> bindables;
		std::mutex mtx;
		
	};
}

