#include "Process.hh"
#include <vector>
#include "ExitCodes.hh"

Process::Process(const string& applicationName, const string & cmd, int priority) : startupinfo_{ sizeof(startupinfo_) } {
	auto lpCommandLine = std::vector<char>{ cmd.begin(), cmd.end() };
	lpCommandLine.push_back('\0');
	CreateProcess(
		applicationName.c_str(),							// lpApplicationName
		lpCommandLine.data(),								// lpCommandLine
		nullptr,											// lpProcessAttributes
		nullptr,											// lpThreadAttributes
		true,												// bInheritHandles
		CREATE_SUSPENDED,									// dwCreationFlags
		nullptr,											// lpEnvironment
		nullptr,											// lpCurrentDirectory
		&startupinfo_,										// lpStartupInfo
		&processInfo_										// lpProcessInformation
	);
	SetPriorityClass(processInfo_.hProcess, priority);
}

Process::~Process() {
	closeHandles();
}

Process::Process(Process&& other) {
	startupinfo_ = other.startupinfo_;
	processInfo_ = other.processInfo_;
	other.processInfo_.hProcess = INVALID_HANDLE_VALUE;
	other.processInfo_.hThread = INVALID_HANDLE_VALUE;
}

void Process::run() {
	ResumeThread(processInfo_.hThread);
}

DWORD Process::waitAndGet() {
	if (processInfo_.hProcess == INVALID_HANDLE_VALUE)
		return -1;
	WaitForSingleObject(processInfo_.hProcess, INFINITE);
	DWORD retVal;
	GetExitCodeProcess(processInfo_.hProcess, &retVal);
	closeHandles();
	return retVal;
}

void Process::terminate() {
	TerminateProcess(
		processInfo_.hProcess,						// process HANDLE
		static_cast<UINT>(ExitCodes::TERMINATED)	// exitCode
	);
	closeHandles();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Process::closeHandles() {
	CloseHandle(processInfo_.hProcess);
	CloseHandle(processInfo_.hThread);
	processInfo_.hProcess = processInfo_.hThread = INVALID_HANDLE_VALUE;
}