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
	using tstring =
		std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;

	template <typename CharT>
	using tstringstream =
		std::basic_stringstream<CharT, std::char_traits<CharT>, std::allocator<CharT>>;


	template<typename CharT>
	std::vector<tstring<CharT>> split(tstring<CharT> str, CharT const delimiter) {
		auto sstream = tstringstream<CharT>{ str };
		auto result = std::vector<tstring<CharT>>();
		auto element = tstring<CharT>{};
		while (std::getline(sstream, element, delimiter)) {
			if (!element.empty()) {
				result.push_back(element);
			}
		}
		return result;
	}
}
