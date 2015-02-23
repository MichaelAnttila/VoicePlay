#include "mp3.h"

namespace VoicePlay
{

MP3::MP3()
:	m_log()
{
}

MP3::~MP3()
{
}

MP3* MP3::Create(std::shared_ptr<Log> log, char const* const filename)
{
	MP3* result = new MP3();
	if (!result || !result->Initialize(log, filename)) {
		log->Info("MP3::Create: Could not create or initialize MP3 object.");
		delete result;
		return 0;
	}
	return result;
}

bool MP3::Play()
{
	return true;
}

bool MP3::Initialize(std::shared_ptr<Log> log, char const* const filename)
{
	m_log = log;
	return true;
}

}
