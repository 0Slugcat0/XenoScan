#pragma once

#ifndef XENOSCANENGINE_LIB
#error This header is for internal library use. Include "ScannerTarget.h" instead.
#endif


// This file is include guarded because we want to ignore it on non-Windows systems.
// CMake will stop the compiler form seeing it, but it wont stop inclusion.
#ifdef __linux__
#include "ScannerTarget.h"

// We define NativeScannerTarget as ScannerTargetLinux so that the
// factory will use this class for native processes
#ifndef NativeScannerTarget
#define NativeScannerTarget ScannerTargetLinux
#else
#error Only one NativeScannerTarget can exist!
#endif

class ScannerTargetLinux : public ScannerTarget
{
public:
	static ScannerTarget::FACTORY_TYPE::KEY_TYPE Key;

	ScannerTargetLinux();
	~ScannerTargetLinux();

	virtual bool attach(const ProcessIdentifier &pid);
	virtual bool isAttached() const;

	virtual bool queryMemory(const MemoryAddress &adr, MemoryInformation& meminfo, MemoryAddress &nextAdr) const;
	virtual bool getMainModuleBounds(MemoryAddress &start, MemoryAddress &end) const;

	virtual uint64_t getFileTime64() const;
	virtual uint32_t getTickTime32() const;

protected:
	virtual bool rawRead(const MemoryAddress &adr, const size_t objectSize, void* result) const;
	virtual bool rawWrite(const MemoryAddress &adr, const size_t objectSize, const void* const data) const;

private:
	ProcessHandle processHandle;
	MemoryAddress mainModuleStart, mainModuleEnd;
	size_t pageSize;

	MemoryAddress getMainModuleBaseAddress() const;
};

#endif
