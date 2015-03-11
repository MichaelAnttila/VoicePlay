#ifndef MP3_H_INCLUDED
#define MP3_H_INCLUDED

#include <memory>
#include "log.h"
#include "audio.h"

namespace VoicePlay {

class MP3Library
{
public:
	MP3Library();
	~MP3Library();
	static MP3Library* Create(std::shared_ptr<Log> log);
private:
	bool Initialize(std::shared_ptr<Log> log);
	bool m_initialized;
};

class MP3
{
public:
	MP3();
	~MP3();
	static MP3* Create(std::shared_ptr<Log> log, std::shared_ptr<Audio> audio, char const* const filename);
	bool Play();
private:
	bool Initialize(std::shared_ptr<Log> log, std::shared_ptr<Audio> audio, char const* const filename);
	std::shared_ptr<Log> m_log;
	std::shared_ptr<Audio> m_audio;
};

}

#endif
