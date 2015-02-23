#include <stdio.h>
#include <memory>
#include <ao/ao.h>
#include <mpg123.h>
#include "log.h"
#include "udp.h"

#define PORT 9930

int main(void)
{
	std::shared_ptr<VoicePlay::Log> log = std::shared_ptr<VoicePlay::Log>(VoicePlay::Log::Create());
	log->Info("VoicePlay");

    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, "/mnt/usb/Purple Motion - Musicdisk - 2nd Reality OST.mp3");
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * 8;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
        ao_play(dev, (char*)buffer, done);

    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

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
