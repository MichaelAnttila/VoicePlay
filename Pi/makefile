all: vps.o udp.o log.o
	g++ -Wall vps.o udp.o log.o -o voiceplayserver

vps.o: vps.cc
	g++ -Wall -c -std=c++0x vps.cc

udp.o: udp.cc
	g++ -Wall -c -std=c++0x udp.cc

log.o: log.cc
	g++ -Wall -c -std=c++0x log.cc

clean:
	rm *.o voiceplayserver
