#ifndef UDP_H_INCLUDED
#define UDP_H_INCLUDED

#include <memory>
#include <sys/types.h>
#include "log.h"

namespace VoicePlay {

class UDP
{
public:
	UDP();
	~UDP();
	static UDP* Create(std::shared_ptr<Log> log, unsigned short const port);
	// Check to see if there is incoming data on the socket.
	// If block is true, then the call will block waiting for data.  Otherwise it will simply poll the socket.
	bool Select(bool const block);
	// Receive returns false if there was an error.  Otherwise it returns true and buffer contains a null terminated string.
	// It will block on the socket if there is no data, so make sure you call Select() first.
	bool Receive(char* const buffer, unsigned int const buffersize);
private:
	bool Initialize(std::shared_ptr<Log> log, unsigned short const port);
	std::shared_ptr<Log> m_log;
	int m_socket;
	fd_set m_fdset;
};

}

#endif
