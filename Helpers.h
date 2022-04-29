#pragma once
namespace En3rN::Helpers {
	//make unique names for imgui functions, useful when you have multiple entrys with same name
	inline auto tag = []( auto id, auto name ) {
		std::string s = name + "##" + std::to_string( id );
		return s;
	};
}
