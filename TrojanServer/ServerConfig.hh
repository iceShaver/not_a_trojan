#ifndef CONFIG_HH
#define CONFIG_HH
#include <string>
#include <Windows.h>

namespace ServerConfig {
	constexpr PCWSTR WND_CLASS_NAME = L"WindowClass";
	constexpr PCWSTR WND_TITLE = L"MyTrojan management application";
	constexpr const int WND_WIDTH = 1280;
	constexpr const int WND_HEIGHT = 720;
	constexpr PCSTR LISTEN_PORT = "55555";
	constexpr uint64_t AUTH_PACKET = 2564788125965854;
	constexpr int BUFFER_SIZE = 4096;
}
#endif // CONFIG_HH
