#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "udp.h"

namespace
{
static inline int min(int a, int b)
{
	if (a < b) {
		return a;
	} else {
		return b;
	}
}
}

namespace VoicePlay
{

UDP::UDP()
:	m_log()
,	m_socket(0)
{
}

UDP::~UDP()
{
	if (m_socket) {
		close(m_socket);
		m_socket = 0;
	}
}

UDP* UDP::Create(std::shared_ptr<Log> log, unsigned short const port)
{
	UDP* result = new UDP();
	if (!result || !result->Initialize(log, port)) {
		log->Info("UDP::Create: Could not create or initialize UDP object.");
		delete result;
		return 0;
	}
	return result;
}

bool UDP::Receive(char* const buffer, unsigned int const buffersize)
{
	int bytesreceived = recv(m_socket, buffer, buffersize, 0);
	if (bytesreceived == -1) {
		m_log->Info("UDP::Receive: Call to recv() failed.\n");
		return false;
	}
	buffer[min(bytesreceived, buffersize-1)] = 0;
	return true;
}

bool UDP::Initialize(std::shared_ptr<Log> log, unsigned short const port)
{
	m_log = log;
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == -1) {
		m_log->Info("UDP::Initialize: Call to socket() failed.");
		return false;
	}
	struct sockaddr_in si_me = {};
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(m_socket, reinterpret_cast<sockaddr*>(&si_me), sizeof(si_me)) == -1) {
		m_log->Info("UDP::Initialize: Call to bind() failed.");
		return false;
	}
	return true;
}

}
