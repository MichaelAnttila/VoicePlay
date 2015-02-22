#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define PORT 9930

int min(int a, int b)
{
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

int main(void)
{
	printf("VoicePlay\n");

	int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == -1) {
		printf("Call to socket() failed.\n");
		return 1;
	}

	struct sockaddr_in si_me = {};
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, reinterpret_cast<sockaddr*>(&si_me), sizeof(si_me)) == -1) {
		printf("Call to bind() failed.\n");
		return 1;
	}

	while (true) {
		struct sockaddr_in si_other = {};
		socklen_t slen = sizeof(si_other);
		char buf[1024];
		int bytesreceived = recvfrom(s, buf, sizeof(buf), 0, reinterpret_cast<sockaddr*>(&si_other), &slen);
		if (bytesreceived == -1) {
			printf("recvfrom()\n");
			return 1;
		}
		buf[min(bytesreceived, sizeof(buf)-1)] = 0;
		printf("Received packet from %s:%d\nData: %s\nSize: %d\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf, bytesreceived);
	}

	close(s);
	return 0;
}
