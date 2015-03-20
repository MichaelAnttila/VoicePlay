#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <stdarg.h>

namespace VoicePlay {

class Log
{
public:
	Log();
	~Log();
	static Log* Create();
	void Info(char const* const format, ...);
	void Error(char const* const format, ...);
private:
	void Write(char const* const format, va_list args);
};

}

#endif
