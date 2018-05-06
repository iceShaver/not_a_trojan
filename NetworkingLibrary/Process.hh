#pragma once
#include <Windows.h>
#include <string>
using std::string;



class Process {
public:
	Process(const string& applicationName, const string& cmd, int priority = NORMAL_PRIORITY_CLASS);
	~Process();
	//Process(const Process&) = delete;
	Process(Process&&);
	void run();
	DWORD waitAndGet();
	void terminate();

private:
	PROCESS_INFORMATION processInfo_;
	STARTUPINFO startupinfo_{};

	void closeHandles();

};