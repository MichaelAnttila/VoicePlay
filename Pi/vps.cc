#include <stdio.h>
#include <string.h>
#include <memory>
#include "log.h"
#include "udp.h"

#define PORT 9930

int main(void)
{
	std::shared_ptr<VoicePlay::Log> log = std::shared_ptr<VoicePlay::Log>(VoicePlay::Log::Create());
	log->Info("VoicePlay");

	std::shared_ptr<VoicePlay::UDP> udp = std::shared_ptr<VoicePlay::UDP>(VoicePlay::UDP::Create(log, PORT));
	if (!udp) {
		log->Error("Error: Could not create UDP object.");
		return 1;
	}

	while (true) {
		char buf[1024];
		if (!udp->Receive(buf, sizeof(buf))) {
			log->Error("Error: Encountered an error while trying to read data from the network.");
			return 2;
		}
		log->Info("Received packet: %s", buf);
	}

	return 0;
}
