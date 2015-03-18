#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <memory>
#include <ao/ao.h>
#include "log.h"

namespace VoicePlay {

class Audio
{
public:
	Audio();
	~Audio();
	static Audio* Create(std::shared_ptr<Log> log);
	bool Open(int const bytespersample, int const rate, int const channels);
	bool Play(char* const buffer, int const size);
	void Close();
private:
	bool Initialize(std::shared_ptr<Log> log);
	bool m_initialized;
	std::shared_ptr<Log> m_log;
	ao_device* m_device;
};

}

#endif
