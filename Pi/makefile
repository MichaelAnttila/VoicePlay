all: log.o udp.o audio.o mp3.o directory.o main.o
	g++ -Wall log.o udp.o audio.o mp3.o directory.o main.o -lmpg123 -lao -o voiceplayserver

log.o: log.cc
	g++ -Wall -c -std=c++0x log.cc

udp.o: udp.cc
	g++ -Wall -c -std=c++0x udp.cc

audio.o: audio.cc
	g++ -Wall -c -std=c++0x audio.cc

mp3.o: mp3.cc
	g++ -Wall -c -std=c++0x mp3.cc

directory.o: directory.cc
	g++ -Wall -c -std=c++0x directory.cc

main.o: main.cc
	g++ -Wall -c -std=c++0x main.cc

clean:
	rm *.o voiceplayserver
