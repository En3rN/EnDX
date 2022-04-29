#pragma once
#include <vector>
#include <memory>

namespace En3rN::DX
{
	/*template <typename Element>
	struct iContainer
	{
		using Container = std::vector<Element>;
		template<typename Element>
		void Add(std::shared_ptr<Element> element)
		{
			if constexpr(!std::is_same_v<element, Element>) {
				for(auto& stored : m_elements) {
					if(std::dynamic_pointer_cast<Element>(stored)) {
						stored = element;
						return;
					}
				}
			}
			
			return;
		}
		template <typename T>
		bool Has()
		{
			for(auto& element : m_elements)
				if(std::dynamic_pointer_cast<T>(element))
					return true;
			return false;
		}
		template <typename T>
		auto Get()
		{
			std::shared_ptr<T> result;
			for(auto& element : m_elements) {
				result = std::dynamic_pointer_cast<T>(element);
				if(result)
					return result;
			}
			return result;
		}
		template <typename ... elements>
		void Add(Element element, elements ... rest)
		{
			Addelement(element);
			if constexpr(sizeof...(rest) > 0)
				Addelements((rest)...);
		}
		const Container& GetContainer() const { return m_elements; }
		Container& GetContainer() { return m_elements; }
	protected:
		Container m_elements;
	};*/
}

