#include "mp3.h"

namespace VoicePlay
{

MP3Library::MP3Library()
:	m_initialized(false)
{
}

MP3Library::~MP3Library()
{
	if (m_initialized) {
		mpg123_exit();
		m_initialized = false;
	}
}

MP3Library* MP3Library::Create(std::shared_ptr<Log> log)
{
	MP3Library* result = new MP3Library();
	if (!result || !result->Initialize(log)) {
		log->Info("MP3Library::Create: Could not create or initialize MP3Library object.");
		if (result) {
			delete result;
			result = 0;
		}
	}
	return result;
}

bool MP3Library::Initialize(std::shared_ptr<Log> log)
{
	if (mpg123_init() != MPG123_OK) {
		log->Info("MP3Library::Create: Could not initialize MPG123 library.");
		return false;
	}
	m_initialized = true;
	return true;
}

MP3::MP3()
:	m_log()
,	m_audio()
,	m_mp3(0)
,	m_buffer(0)
,	m_buffersize(0)
{
}

MP3::~MP3()
{
	if (m_buffer) {
		free(m_buffer);
		m_buffer = 0;
	}
	if (m_mp3) {
		mpg123_close(m_mp3);
		mpg123_delete(m_mp3);
		m_mp3 = 0;
	}
	if (m_audio) {
		m_audio->Close();
	}
}

MP3* MP3::Create(std::shared_ptr<Log> log, std::shared_ptr<Audio> audio, char const* const filename)
{
	MP3* result = new MP3();
	if (!result || !result->Initialize(log, audio, filename)) {
		log->Info("MP3::Create: Could not create or initialize MP3 object.");
		if (result) {
			delete result;
			result = 0;
		}
	}
	return result;
}

bool MP3::Play()
{
        size_t size = 0;
        if (mpg123_read(m_mp3, static_cast<unsigned char*>(m_buffer), m_buffersize, &size) != MPG123_OK) {
		return false;
	}
	if (!m_audio->Play(static_cast<char*>(m_buffer), size)) {
		m_log->Info("MP3::Initialize: Aborting playback due to an error.");
		return false;
	}
	return true;
}

bool MP3::Initialize(std::shared_ptr<Log> log, std::shared_ptr<Audio> audio, char const* const filename)
{
	m_log = log;
	m_audio = audio;
	int error = 0;
	m_mp3 = mpg123_new(0, &error);
	if (!m_mp3) {
		m_log->Info("MP3::Initialize: Could not initialize MP3 library - mpg123_new returned (%s).", mpg123_plain_strerror(error));
		return false;
	}
	m_buffersize = mpg123_outblock(m_mp3);
	m_buffer = malloc(m_buffersize);
	if (!m_buffer) {
		m_log->Info("MP3::Initialize: Out of memory.");
		return false;
	}
	error = mpg123_open(m_mp3, filename);
	if (error != MPG123_OK) {
		m_log->Info("MP3::Initialize: Could not open file (%s) - mpg123_open returned (%s).", filename, mpg123_plain_strerror(error));
		return false;
	}
	int channels = 0;
	int encoding = 0;
	long rate = 0;
	error = mpg123_getformat(m_mp3, &rate, &channels, &encoding);
	if (error != MPG123_OK) {
		m_log->Info("MP3::Initialize: Could not get format for file (%s) - mpg123_getformat returned (%s).", filename, mpg123_plain_strerror(error));
		return false;
	}
	if (!m_audio->Open(mpg123_encsize(encoding), rate, channels)) {
		m_log->Info("MP3::Initialize: Could not open audio output stream for encoding %d, rate %d, channels %d.", encoding, rate, channels);
		return false;
	}
	return true;
}

}
