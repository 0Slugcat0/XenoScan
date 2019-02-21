#include "ScannerTargetLinux.h"

#include "Assert.h"
#include "StdListBlueprint.h"
#include "StdMapBlueprint.h"
#include "NativeClassInstanceBlueprint.h"

ScannerTargetLinux::ScannerTargetLinux() {
}

ScannerTargetLinux::~ScannerTargetLinux() {
}

bool ScannerTargetLinux::attach(const ProcessIdentifier& pid) {
}

bool ScannerTargetLinux::isAttached() const {
}

bool ScannerTargetLinux::queryMemory(const MemoryAddress& adr,
		MemoryInformation& meminfo, MemoryAddress& nextAdr) const {
}

bool ScannerTargetLinux::getMainModuleBounds(MemoryAddress& start,
		MemoryAddress& end) const {
}

uint64_t ScannerTargetLinux::getFileTime64() const {
}

uint32_t ScannerTargetLinux::getTickTime32() const {
}

bool ScannerTargetLinux::rawRead(const MemoryAddress& adr,
		const size_t objectSize, void* result) const {
}

bool ScannerTargetLinux::rawWrite(const MemoryAddress& adr,
		const size_t objectSize, const void* const data) const {
}

MemoryAddress ScannerTargetLinux::getMainModuleBaseAddress() const {
}
