#include "audio.h"

namespace VoicePlay
{

Audio::Audio()
:	m_initialized(false)
,	m_log()
,	m_device(0)
{
}

Audio::~Audio()
{
	if (m_device) {
		ao_close(m_device);
		m_device = 0;
	}
	if (m_initialized) {
		ao_shutdown();
		m_initialized = false;
	}
}

Audio* Audio::Create(std::shared_ptr<Log> log)
{
	Audio* result = new Audio();
	if (!result || !result->Initialize(log)) {
		log->Info("Audio::Create: Could not create or initialize Audio object.");
		if (result) {
			delete result;
			result = 0;
		}
	}
	return result;
}

bool Audio::Open(int const bytespersample, int const rate, int const channels)
{
	if (m_device) {
		Close();
	}
	ao_sample_format format;
	format.bits = bytespersample * 8;
	format.rate = rate;
	format.channels = channels;
	format.byte_format = AO_FMT_NATIVE;
	format.matrix = 0;
	int driver = ao_default_driver_id();
	m_device = ao_open_live(driver, &format, 0);
	if (!m_device) {
		m_log->Info("Audio::Open could not open the output stream.");
		return false;
	}
	return true;
}

bool Audio::Play(char* const buffer, int const size)
{
	if (!ao_play(m_device, buffer, size)) {
		m_log->Info("Audio::Play encountered an error.");
		return false;
	}
	return true;
}

void Audio::Close()
{
	if (m_device) {
		ao_close(m_device);
		m_device = 0;
	}
}

bool Audio::Initialize(std::shared_ptr<Log> log)
{
	m_log = log;
	ao_initialize();
	m_initialized = true;
	return true;
}

}
