#pragma once
#include <Windows.h>
#include <functional>
using std::function;
class Thread
{
public:
	Thread(LPTHREAD_START_ROUTINE startFunction, LPVOID args, int priority = THREAD_PRIORITY_NORMAL);
	Thread(const Thread& other) = delete;
	Thread(Thread&& other) noexcept;
	~Thread();
	DWORD waitAndGet();
	void run();
	void terminate();
private:
	HANDLE hThread_;
	void closeHandle();
};