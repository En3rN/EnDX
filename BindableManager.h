#pragma once
#include "iBindable.h"
#include "enBuffer.h"
#include "enString.h"
#include "logger.h"
#include <vector>
#include <utility>
#include <sstream>
#include <stdint.h>
#include <unordered_map>
#include <map>
#include <mutex>
#include <filesystem>
namespace En3rN::DX
{
	template<typename>
	struct is_std_vector : std::false_type {};

	template<typename T, typename A>
	struct is_std_vector<std::vector<T, A>> : std::true_type {};

	template <typename T, typename= typename std::enable_if_t<std::is_enum_v<T>>>
	std::ostream& operator << (std::ostream& os, const T& e)
	{
		os << std::to_string(std::underlying_type_t<T>(e));
		return os;
	}

	class BindableManager
	{
	public:
		template <class T, typename... ConstructorArgs, std::enable_if_t<std::is_base_of_v<Bindable,T>, bool> = true >
		static std::shared_ptr<T> Query(ConstructorArgs ... args)
		{
			return Get().pQuery<T>((args)...);
		}
	private:
		template <class T, typename... ConstructorArgs>
		std::shared_ptr<T> pQuery(ConstructorArgs ... args)
		{
			std::scoped_lock(mtx);
			//std::string key= T::GetKey((args)...);
			std::string bn = typeid(T).name();
			std::string key = bn.substr(bn.find_last_of("::", bn.size())+1, bn.size() - bn.find_last_of("::", bn.size()+1));
			key += GenerateKey(args...);
			const auto req = bindables.find(key);
			if (req == bindables.end())
			{
				auto bindable = std::make_shared<T>((args)...);
				bindables[key] = bindable;
				return bindable;
			}
			else
			{
				return std::static_pointer_cast<T>(req->second);
			}

		}
		auto ToStr(const std::string& t) { return t; }
		auto ToStr(const std::wstring& t) { return stringconverter::str(t); }
		auto ToStr(const char* t) { return std::string{ t }; }
		auto ToStr(const wchar_t* t) { return stringconverter::str(std::wstring{ t }); }
		auto TsStr(const std::vector<std::string>& t)
		{
			std::string ret;
			for (auto& s : t)
				ret += ToStr(s);
			return ret;
		}
		template <typename T>
		auto ToStr(const T& t)
		{
			if constexpr (std::is_enum_v<T>) {
				return std::to_string(std::underlying_type_t<T>(t));
			};
			if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
				return std::to_string(t);
			};
			if constexpr (std::is_same_v<std::filesystem::path, T>) {
				return std::string(t.string());
			}
			if constexpr (std::is_convertible_v<T, std::string>) {
				return std::string{ t };
			}
			if constexpr (is_std_vector<T>::value) {
				std::string ret;
				for (auto& s : t)
				{
					if constexpr (std::is_convertible_v<decltype(s), std::string>) {
						ret += ToStr(s);
						continue;
					}
					
					if constexpr (std::is_same_v<decltype(s), std::string>) {
						ret += ToStr(s);
						continue;
					}
					ret += std::to_string(typeid(t).hash_code());
					return ret;
				}
				return ret;
			}
			//Logger::Err(typeid(T).name());
			return  std::string{ "!" };
		}		
		template <typename T, typename ...Ts>
		void _GenerateKey(std::string & s, T& t, Ts& ...ts)
		{
			s += "#" + ToStr(t);
			if constexpr (sizeof...(ts) > 0)
				_GenerateKey(s, ts...);
		}
		template <typename ...Ts>
		std::string GenerateKey(Ts& ...ts)
		{
			std::string key{};
			if constexpr (sizeof...(ts) > 0)
				_GenerateKey(key, ts...);
			return key;
		}
		static BindableManager& Get()
		{
			static BindableManager manager;
			return manager;
		};
		std::map<std::string, Bindable::Base::handle> bindables;
		std::mutex mtx;
		
	};
}

