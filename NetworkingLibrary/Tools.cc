#include "tools.hh"
#include <iostream>

void Tools::PrintDebugMessage(std::string msg) {
#ifdef _DEBUG
	std::cout << msg << std::endl;
#endif // _DEBUG
}
