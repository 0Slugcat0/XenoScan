#pragma once
#include <string.h>

#include "ScanVariantComparator.h"

class ScanVariant;

class ScanVariantUnderlyingTypeTraits
{
public:
	virtual const ScanVariantComparator getComparator() const = 0;
	virtual const ScanVariantComparator getBigEndianComparator() const = 0;
	virtual const void copyFromBuffer(const uint8_t* buffer, const size_t &size, const bool &isLittleEndian, void* output) const = 0;
	virtual const size_t getSize() const = 0;
	virtual const size_t getAlignment() const = 0;
	virtual const std::wstring getName() const = 0;
	virtual const std::wstring getFormatString() const = 0;
	virtual const uint32_t getBaseType() const = 0;
	virtual const uint32_t getTargetType() const = 0;
	virtual const bool isStringType() const = 0;
	virtual const bool isNumericType() const = 0;
	virtual const bool isSignedNumericType() const = 0;
	virtual const bool isUnsignedNumericType() const = 0;
	virtual const bool isFloatingPointNumericType() const = 0;
	virtual const bool isDynamicType() const = 0;
	virtual const bool isStructureType() const = 0;
	virtual const std::wstring toString(void* data) const = 0;
	virtual void fromString(const std::wstring& input, ScanVariant& output) const = 0;
};

template<typename T> struct alignment_trick { char c; T member; };
#define ALIGNOF(type) offsetof (alignment_trick<type>, member)

template <typename TYPE, bool UNSIGNED, bool FLOATING, uint32_t BASE_TYPE, uint32_t TARGET_TYPE>
class ScanVariantUnderlyingNumericTypeTraits : public ScanVariantUnderlyingTypeTraits
{
public:
	ScanVariantUnderlyingNumericTypeTraits(const std::wstring& typeName, const std::wstring& typeFormat)
		: typeName(typeName),
		typeFormat(typeFormat)
	{}
	virtual ~ScanVariantUnderlyingNumericTypeTraits() {}

	inline virtual const ScanVariantComparator getComparator() const
	{
		return &numericComparator<TYPE>;
	}
	inline virtual const ScanVariantComparator getBigEndianComparator() const
	{
		return &bigEndianNumericComparator<TYPE>;
	}
	inline virtual const void copyFromBuffer(const uint8_t* buffer, const size_t &size, const bool &isLittleEndian, void* output) const
	{
		// TODO maybe ASSERT(size == sizeof(TYPE));
		*(TYPE*)output = isLittleEndian ? *(TYPE*)buffer : swapEndianness(*(TYPE*)buffer);
	}
	inline virtual const size_t getSize() const { return sizeof(TYPE); }
	inline virtual const size_t getAlignment() const { return ALIGNOF(TYPE); }
	inline virtual const std::wstring getName() const { return this->typeName; }
	inline virtual const std::wstring getFormatString() const { return this->typeFormat; }
	inline virtual const uint32_t getBaseType() const { return BASE_TYPE; }
	inline virtual const uint32_t getTargetType() const { return TARGET_TYPE; };
	inline virtual const bool isStringType() const { return false; }
	inline virtual const bool isNumericType() const { return true; }
	inline virtual const bool isSignedNumericType() const { return !UNSIGNED; }
	inline virtual const bool isUnsignedNumericType() const { return UNSIGNED; }
	inline virtual const bool isFloatingPointNumericType() const { return FLOATING; }
	inline virtual const bool isDynamicType() const { return (BASE_TYPE != TARGET_TYPE); };
	inline virtual const bool isStructureType() const { return false; }
	inline virtual const std::wstring toString(void* data) const
	{
		TYPE value;
		wchar_t buffer[100];

		memcpy(&value, data, sizeof(value));
		swprintf<100>(buffer, this->typeFormat.c_str(), value);
		return buffer;
	}
	virtual void fromString(const std::wstring& input, ScanVariant& output) const;

private:
	std::wstring typeName, typeFormat;
};

template <uint32_t BASE_TYPE, uint32_t TARGET_TYPE>
class ScanVariantUnderlyingAsciiStringTypeTraits : public ScanVariantUnderlyingTypeTraits
{
public:
	ScanVariantUnderlyingAsciiStringTypeTraits() {}
	virtual ~ScanVariantUnderlyingAsciiStringTypeTraits() {}

	inline virtual const ScanVariantComparator getComparator() const { return nullptr; }
	inline virtual const ScanVariantComparator getBigEndianComparator() const { return nullptr; }
	inline virtual const void copyFromBuffer(const uint8_t* buffer, const size_t &size, const bool &isLittleEndian, void* output) const
	{
		*(std::string*)output = std::string((std::string::value_type*)buffer, (std::string::value_type*)&buffer[size]);
	}
	inline virtual const size_t getSize() const { return 0; }
	inline virtual const size_t getAlignment() const { return 1; }
	inline virtual const std::wstring getName() const { return L"ascii string"; }
	inline virtual const std::wstring getFormatString() const { return L"%s"; }
	inline virtual const uint32_t getBaseType() const { return BASE_TYPE; }
	inline virtual const uint32_t getTargetType() const { return TARGET_TYPE; };
	inline virtual const bool isStringType() const { return true; }
	inline virtual const bool isNumericType() const { return false; }
	inline virtual const bool isSignedNumericType() const { return false; }
	inline virtual const bool isUnsignedNumericType() const { return false; }
	inline virtual const bool isFloatingPointNumericType() const { return false; }
	inline virtual const bool isDynamicType() const { return false; };
	inline virtual const bool isStructureType() const { return false; }
	inline virtual const std::wstring toString(void* data) const
	{
		std::string value((std::string::value_type*)data);
		return std::wstring(value.begin(), value.end());
	}
	virtual void fromString(const std::wstring& input, ScanVariant& output) const;
};

template <uint32_t BASE_TYPE, uint32_t TARGET_TYPE>
class ScanVariantUnderlyingWideStringTypeTraits : public ScanVariantUnderlyingTypeTraits
{
public:
	ScanVariantUnderlyingWideStringTypeTraits() {}
	virtual ~ScanVariantUnderlyingWideStringTypeTraits() {}

	inline virtual const ScanVariantComparator getComparator() const { return nullptr; }
	inline virtual const ScanVariantComparator getBigEndianComparator() const { return nullptr; }
	inline virtual const void copyFromBuffer(const uint8_t* buffer, const size_t &size, const bool &isLittleEndian, void* output) const
	{
		*(std::wstring*)output = std::wstring((std::wstring::value_type*)buffer, (std::wstring::value_type*)&buffer[size]);
	}
	inline virtual const size_t getSize() const { return 0; }
	inline virtual const size_t getAlignment() const { return 1; }
	inline virtual const std::wstring getName() const { return L"wide string"; }
	inline virtual const std::wstring getFormatString() const { return L"%s"; }
	inline virtual const uint32_t getBaseType() const { return BASE_TYPE; }
	inline virtual const uint32_t getTargetType() const { return TARGET_TYPE; };
	inline virtual const bool isStringType() const { return true; }
	inline virtual const bool isNumericType() const { return false; }
	inline virtual const bool isSignedNumericType() const { return false; }
	inline virtual const bool isUnsignedNumericType() const { return false; }
	inline virtual const bool isFloatingPointNumericType() const { return false; }
	inline virtual const bool isDynamicType() const { return false; };
	inline virtual const bool isStructureType() const { return false; }
	inline virtual const std::wstring toString(void* data) const
	{
		return std::wstring((std::wstring::value_type*)data);
	}
	virtual void fromString(const std::wstring& input, ScanVariant& output) const;
};

template <uint32_t BASE_TYPE, uint32_t TARGET_TYPE>
class ScanVariantUnderlyingStructureTypeTraits : public ScanVariantUnderlyingTypeTraits
{
public:
	ScanVariantUnderlyingStructureTypeTraits() {}
	virtual ~ScanVariantUnderlyingStructureTypeTraits() {}

	inline virtual const ScanVariantComparator getComparator() const { return nullptr; }
	inline virtual const ScanVariantComparator getBigEndianComparator() const { return nullptr; }
	inline virtual const void copyFromBuffer(const uint8_t* buffer, const size_t &size, const bool &isLittleEndian, void* output) const
	{
		// TODO maybe assert false?
	}
	inline virtual const size_t getSize() const { return 0; }
	inline virtual const size_t getAlignment() const { return 1; }
	inline virtual const std::wstring getName() const { return L"struct"; }
	inline virtual const std::wstring getFormatString() const { return L""; }
	inline virtual const uint32_t getBaseType() const { return BASE_TYPE; }
	inline virtual const uint32_t getTargetType() const { return TARGET_TYPE; };
	inline virtual const bool isStringType() const { return false; }
	inline virtual const bool isNumericType() const { return false; }
	inline virtual const bool isSignedNumericType() const { return false; }
	inline virtual const bool isUnsignedNumericType() const { return false; }
	inline virtual const bool isFloatingPointNumericType() const { return false; }
	inline virtual const bool isDynamicType() const { return false; };
	inline virtual const bool isStructureType() const { return true; }
	inline virtual const std::wstring toString(void* data) const
	{
		return L"(user-defined structure)";
	}
	virtual void fromString(const std::wstring& input, ScanVariant& output) const;
};

template <uint32_t BASE_TYPE, uint32_t TARGET_TYPE>
class ScanVariantUnderlyingNullTypeTraits : public ScanVariantUnderlyingTypeTraits
{
public:
	ScanVariantUnderlyingNullTypeTraits() {}
	virtual ~ScanVariantUnderlyingNullTypeTraits() {}

	inline virtual const ScanVariantComparator getComparator() const { return nullptr; }
	inline virtual const ScanVariantComparator getBigEndianComparator() const { return nullptr; }
	inline virtual const void copyFromBuffer(const uint8_t* buffer, const size_t &size, const bool &isLittleEndian, void* output) const
	{
		// TODO maybe assert false?
	}
	inline virtual const size_t getSize() const { return 0; }
	inline virtual const size_t getAlignment() const { return 4; }
	inline virtual const std::wstring getName() const { return L"null"; }
	inline virtual const std::wstring getFormatString() const { return L""; }
	inline virtual const uint32_t getBaseType() const { return BASE_TYPE; }
	inline virtual const uint32_t getTargetType() const { return TARGET_TYPE; };
	inline virtual const bool isStringType() const { return false; }
	inline virtual const bool isNumericType() const { return false; }
	inline virtual const bool isSignedNumericType() const { return false; }
	inline virtual const bool isUnsignedNumericType() const { return false; }
	inline virtual const bool isFloatingPointNumericType() const { return false; }
	inline virtual const bool isDynamicType() const { return false; };
	inline virtual const bool isStructureType() const { return false; }
	inline virtual const std::wstring toString(void* data) const
	{
		return L"(null)";
	}
	virtual void fromString(const std::wstring& input, ScanVariant& output) const;
};
