#include <stdio.h>
#include "log.h"

namespace VoicePlay
{

Log::Log()
{
}

Log::~Log()
{
}

Log* Log::Create()
{
	Log* result = new Log();
	return result;
}

void Log::Info(char const* const format, ...)
{
	va_list valist;
	va_start(valist, format);
	Write(format, valist);
	va_end(valist);
}

void Log::Error(char const* const format, ...)
{
	va_list valist;
	va_start(valist, format);
	Write(format, valist);
	va_end(valist);
}

void Log::Write(char const* const format, va_list valist)
{
	vprintf(format, valist);
	printf("\n");
}

}
