#ifndef UDP_H_INCLUDED
#define UDP_H_INCLUDED

#include <memory>
#include "log.h"

namespace VoicePlay {

class UDP
{
public:
	UDP();
	~UDP();
	static UDP* Create(std::shared_ptr<Log> log, unsigned short const port);
	// Receive returns false if there was an error.  Otherwise it returns true and buffer contains a null terminated string.
	// It will not block on the socket.  If there is no data to receive, the string will be empty.
	bool Receive(char* const buffer, unsigned int const buffersize);
private:
	bool Initialize(std::shared_ptr<Log> log, unsigned short const port);
	std::shared_ptr<Log> m_log;
	int m_socket;
};

}

#endif
