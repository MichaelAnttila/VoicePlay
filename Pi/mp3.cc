#include <ao/ao.h>
#include <mpg123.h>
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
	g_initialized = true;
}

MP3::MP3()
:	m_log()
,	m_audio()
{
}

MP3::~MP3()
{
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
	return true;
}

bool MP3::Initialize(std::shared_ptr<Log> log, std::shared_ptr<Audio> audio, char const* const filename)
{
	m_log = log;
	m_audio = audio;
	int error = 0;
	mpg123_handle* mp3 = mpg123_new(0, &error);
	if (!mp3) {
		m_log->Info("MP3::Initialize: Could not initialize MP3 library - mpg123_new returned (%s).", mpg123_plain_strerror(error));
		return false;
	}
	size_t buffer_size = mpg123_outblock(mh);
	unsigned char* buffer = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));
	error = mpg123_open(mh, filename);
	if (error != MPG123_OK) {
		m_log->Info("MP3::Initialize: Could not open file (%s) - mpg123_open returned (%s).", filename, mpg123_plain_strerror(error));
		return false;
	}
	int channels = 0;
	int encoding = 0;
	long rate = 0;
	error = mpg123_getformat(mh, &rate, &channels, &encoding);
	if (error != MPG123_OK) {
		m_log->Info("MP3::Initialize: Could not get format for file (%s) - mpg123_getformat returned (%s).", filename, mpg123_plain_strerror(error));
		return false;
	}

	if (!m_audio->Open(mpg123_encsize(encoding), rate, channels)) {
		m_log->Info("MP3::Initialize: Could not open audio output stream for encoding %d, rate %d, channels %d.", encoding, rate, channels);
		return false;
	}

        size_t done;
        while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
                ao_play(dev, (char*)buffer, done);
        }

    free(buffer);
    m_audio->Close();
    mpg123_close(mh);
    mpg123_delete(mh);

	return true;
}

}
