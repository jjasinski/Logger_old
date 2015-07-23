#pragma once
//------------------------------------------

// INCLUDES
#include <string>
#include <cstdarg>
#include <cstdio>

namespace logger
{
	namespace internal_helpers
	{
		inline size_t needsBufferSize(const char* aFormat, va_list aArgs)
		{
			va_list copyArgs;
			va_copy(copyArgs, aArgs);
			int bufferSize = vsnprintf(nullptr, 0, aFormat, copyArgs);
			return static_cast<size_t>(bufferSize);
		}

		inline std::string makeStringByPrintf(const char* aFormat, ...)
		{
			va_list args;
			va_start(args, aFormat);
			size_t bufferSize = needsBufferSize(aFormat, args);
			std::string message(bufferSize, '\0');
			vsprintf(&message[0], aFormat, args);
			va_end(args);
			return std::move(message);
		}


		// one parameter specialization
		inline std::string makeFormattedString(const char* aString)
		{
			return std::string(aString);
		}


		// check if default function template parameters are allowed (since c++11?)
		template<
			typename A1 = std::nullptr_t,
			typename A2 = std::nullptr_t,
			typename A3 = std::nullptr_t,
			typename A4 = std::nullptr_t,
			typename A5 = std::nullptr_t,
			typename A6 = std::nullptr_t,
			typename A7 = std::nullptr_t,
			typename A8 = std::nullptr_t,
			typename A9 = std::nullptr_t,
			typename A10 = std::nullptr_t,
			typename A11 = std::nullptr_t,
			typename A12 = std::nullptr_t,
			typename A13 = std::nullptr_t,
			typename A14 = std::nullptr_t,
			typename A15 = std::nullptr_t,
			typename A16 = std::nullptr_t,
			typename A17 = std::nullptr_t,
			typename A18 = std::nullptr_t,
			typename A19 = std::nullptr_t,
			typename A20 = std::nullptr_t
		>
		std::string makeFormattedString(
		const char* aFormat,
		const A1& a1 = A1(),
		const A2& a2 = A2(),
		const A3& a3 = A3(),
		const A4& a4 = A4(),
		const A5& a5 = A5(),
		const A6& a6 = A6(),
		const A7& a7 = A7(),
		const A8& a8 = A8(),
		const A9& a9 = A9(),
		const A10& a10 = A10(),
		const A11& a11 = A11(),
		const A12& a12 = A12(),
		const A13& a13 = A13(),
		const A14& a14 = A14(),
		const A15& a15 = A15(),
		const A16& a16 = A16(),
		const A17& a17 = A17(),
		const A18& a18 = A18(),
		const A19& a19 = A19(),
		const A20& a20 = A20()
		)
		{
			return makeStringByPrintf(aFormat,
				a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
				a11, a12, a13, a14, a15, a16, a17, a18, a19, a20
				);
		}

	}// end internal_helpers

	template<typename... Args>
	std::string makeString(const char* aFormat, const Args&...args)
	{
		return internal_helpers::makeFormattedString(aFormat, args...);
	}

	template<typename... Args>
	std::string makeString(const std::string& aFormat, const Args&...args)
	{
		return internal_helpers::makeFormattedString(aFormat.c_str(), args...);
	}

	// empty string specialization
	inline std::string makeString()
	{
		return std::string();
	}


}// end logger