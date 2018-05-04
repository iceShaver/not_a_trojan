#ifndef CONFIG_HH
#define CONFIG_HH
#include <string>
#include <Windows.h>

namespace ServerConfig {
	constexpr PCWSTR WND_CLASS_NAME = L"WindowClass";
	constexpr PCWSTR WND_TITLE = L"MyTrojan management application";
	constexpr const int WND_WIDTH = 1280;
	constexpr const int WND_HEIGHT = 720;
	constexpr PCSTR LISTEN_PORT = "27123";
	constexpr PCSTR PACKET_ID = "cxzasdfrgt";
}
#endif // CONFIG_HH
