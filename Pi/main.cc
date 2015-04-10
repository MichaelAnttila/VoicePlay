#include <stdio.h>
#include <memory>
#include <strings.h>
#include "log.h"
#include "udp.h"
#include "audio.h"
#include "mp3.h"
#include "directory.h"

#define PORT 9930

int main(void)
{
	std::shared_ptr<VoicePlay::Log> log = std::shared_ptr<VoicePlay::Log>(VoicePlay::Log::Create());
	if (!log) {
		printf("Error: Could not open log.\n");
		return 1;
	}
	log->Info("VoicePlay");
	std::shared_ptr<VoicePlay::Audio> audio = std::shared_ptr<VoicePlay::Audio>(VoicePlay::Audio::Create(log));
	if (!audio) {
		log->Error("Error: Could not create Audio object.");
		return 2;
	}
	std::shared_ptr<VoicePlay::MP3Library> mp3library = std::shared_ptr<VoicePlay::MP3Library>(VoicePlay::MP3Library::Create(log));
	if (!mp3library) {
		log->Error("Error: Could not create MP3Library object.");
		return 3;
	}
	std::shared_ptr<VoicePlay::UDP> udp = std::shared_ptr<VoicePlay::UDP>(VoicePlay::UDP::Create(log, PORT));
	if (!udp) {
		log->Error("Error: Could not create UDP object.");
		return 4;
	}
	std::shared_ptr<VoicePlay::Directory> directory = std::shared_ptr<VoicePlay::Directory>(VoicePlay::Directory::Create(log, "/mnt/usb/Music"));
	if (!directory) {
		log->Error("Error: Could not create directory object.");
		return 5;
	}
	std::shared_ptr<VoicePlay::MP3> mp3 = 0;
	std::deque<std::string> playlist = std::deque<std::string>();
	while (true) {
		if (udp->Select(!mp3 && playlist.empty())) {
			char buf[1024];
			if (!udp->Receive(buf, sizeof(buf))) {
				log->Error("Error: Encountered an error while trying to read data from the network.");
				return 6;
			}
			log->Info("Received packet: %s", buf);
			if (strcasecmp(buf, "skip") == 0 && !playlist.empty()) {
				mp3 = 0;
			} else {
				playlist = directory->Match(buf);
				mp3 = 0;
			}
		}
		if (mp3) {
			if (!mp3->Play()) {
				mp3 = 0;
			}
		}
		if (!mp3) {
			while (!playlist.empty()) {
				log->Info("Playing %s", playlist[0].c_str());
				mp3 = std::shared_ptr<VoicePlay::MP3>(VoicePlay::MP3::Create(log, audio, playlist[0]));
				if (!mp3) {
					log->Error("Error: Could not open file.");
				}
				playlist.pop_front();
				break;
			}
		}
	}
	return 0;
}
