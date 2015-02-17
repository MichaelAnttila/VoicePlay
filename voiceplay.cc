#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define BUFLEN 512
#define NPACK 10
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
	struct sockaddr_in si_me, si_other;
	int s, i;
	socklen_t slen=sizeof(si_other);
	char buf[BUFLEN];

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) {
		printf("socket\n");
		return 1;
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, reinterpret_cast<sockaddr*>(&si_me), sizeof(si_me))==-1) {
		printf("bind\n");
		return 1;
	}

	for (i=0; i<NPACK; i++) {
		if (recvfrom(s, buf, BUFLEN, 0, reinterpret_cast<sockaddr*>(&si_other), &slen)==-1) {
			printf("recvfrom()\n");
			return 1;
		}
		buf[min(slen, BUFLEN-1)] = 0;
		printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
	}

	close(s);
	return 0;
}
