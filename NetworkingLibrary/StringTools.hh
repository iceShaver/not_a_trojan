#pragma once
#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif 
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

namespace StringTools {
	template <typename CharT>
	using StringT =
		std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;

	template <typename CharT>
	using StringStreamT =
		std::basic_stringstream<CharT, std::char_traits<CharT>, std::allocator<CharT>>;


	template<typename CharT>
	std::vector<StringT<CharT>> split(StringT<CharT> str, CharT const delimiter) {
		auto sstream = StringStreamT<CharT>{ str };
		auto result = std::vector<StringT<CharT>>();
		auto element = StringT<CharT>{};
		while (std::getline(sstream, element, delimiter)) {
			if (!element.empty()) {
				result.push_back(element);
			}
		}
		return result;
	}
}
