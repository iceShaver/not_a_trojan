#pragma once
#ifdef NETWORKINGLIBRARY_EXPORTS  
#define NETWORKINGLIBRARY_API __declspec(dllexport)
#else  
#define NETWORKINGLIBRARY_API __declspec(dllimport)
#endif  
#include "DataType.hh"
#pragma pack(push, 1)
struct Metadata
{
	DataType dataType;
	size_t length;
};
#pragma pack(pop)