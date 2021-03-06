#ifndef DIRECTORY_H_INCLUDED
#define DIRECTORY_H_INCLUDED

#include <memory>
#include <string>
#include <vector>
#include <deque>
#include "log.h"

namespace VoicePlay {

class Directory
{
public:
	Directory();
	~Directory();
	static Directory* Create(std::shared_ptr<Log> log, std::string const path);
	struct Result
	{
		std::string filename;
		int score;
	};
	std::deque<Result> Match(std::string const keywords);
private:
	bool Initialize(std::shared_ptr<Log> log, std::string const path);
	std::shared_ptr<Log> m_log;
	std::string m_path;
	struct FileName
	{
		std::string name;
		std::vector<std::string> tokens;
	};
	std::vector<FileName> m_filenames;
};

}

#endif
