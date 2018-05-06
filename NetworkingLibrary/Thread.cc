#include "Thread.hh"
#include "ExitCodes.hh"
#include <iostream>


Thread::Thread(LPTHREAD_START_ROUTINE startFunction, LPVOID args, int priority)
{
	DWORD id;
	hThread_ = CreateThread(
		nullptr,			// lpThreadAttributes
		0,					// dwStackSize
		startFunction,		// lpStartAddress
		args,				// lpParameter
		CREATE_SUSPENDED,	// dwCreationFlags
		&id					// lpThreadId
	);
	SetThreadPriority(hThread_, priority);
}


Thread::Thread(Thread&& other) noexcept {
	hThread_ = other.hThread_;
	other.hThread_ = INVALID_HANDLE_VALUE;
}

Thread::~Thread()
{
	closeHandle();
}

DWORD Thread::waitAndGet() {
	WaitForSingleObject(hThread_, INFINITE);
	DWORD exitCode;
	GetExitCodeThread(hThread_, &exitCode);
	closeHandle();
	return exitCode;
}

void Thread::run() {
	ResumeThread(hThread_);
}


void Thread::terminate() {
	TerminateThread(hThread_, static_cast<DWORD>(ExitCodes::TERMINATED));
	closeHandle();
}

void Thread::closeHandle() {
	CloseHandle(hThread_);
	hThread_ = INVALID_HANDLE_VALUE;
}