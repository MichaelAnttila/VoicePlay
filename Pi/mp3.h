#ifndef MP3_H_INCLUDED
#define MP3_H_INCLUDED

#include <memory>
#include "log.h"

namespace VoicePlay {

class MP3
{
public:
	MP3();
	~MP3();
	static MP3* Create(std::shared_ptr<Log> log, char const* const filename);
	bool Play();
private:
	bool Initialize(std::shared_ptr<Log> log, char const* const filename);
	std::shared_ptr<Log> m_log;
};

}

#endif
