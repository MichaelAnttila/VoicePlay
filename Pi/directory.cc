#include <algorithm>
#include <stdio.h>
#include <dirent.h>
#include <strings.h>
#include "directory.h"

namespace VoicePlay
{

Directory::Directory()
:	m_log()
,	m_path()
,	m_filenames()
{
}

Directory::~Directory()
{
}

Directory* Directory::Create(std::shared_ptr<Log> log, std::string const path)
{
	Directory* result = new Directory();
	if (!result || !result->Initialize(log, path)) {
		log->Info("Directory::Create: Could not create or initialize Directory object.");
		if (result) {
			delete result;
			result = 0;
		}
	}
	return result;
}

bool CaseInsensitiveFind(std::string const& a, std::string const& b)
{
	auto it = std::search(a.begin(), a.end(), b.begin(), b.end(), [](char c1, char c2) {
		return std::toupper(c1) == std::toupper(c2);
	});
	return (it != a.end());
}

static std::vector<std::string> Split(std::string const input)
{
	std::vector<std::string> result;
	char const* i = input.c_str();
	char temp[256];
	int index = 0;
	while (true) {
		if (*i == 0 || *i == ' ' || *i == '-' || *i == '.') {
			if (index > 0) {
				temp[index] = 0;
				result.push_back(temp);
				index = 0;
			}
			if (*i == 0) {
				break;
			}
		} else {
			temp[index] = *i;
			index++;
		}	
		i++;
	}
	return result;
}

std::deque<Directory::Result> Directory::Match(std::string const keywords)
{
	std::vector<std::string> keys = Split(keywords); 
	struct InterimResult
	{
		FileName filename;
		int score;
	};
	std::vector<InterimResult> interimresults;
	for (auto it : m_filenames) {
		InterimResult result = {it, 0};
		interimresults.push_back(result);
	}
	for (auto kt : keys) {
		std::vector<InterimResult> matchingresults;
		for (auto it : interimresults) {
			for (auto tt = it.filename.tokens.begin(); tt != it.filename.tokens.end(); tt++) {
				if (strcasecmp(tt->c_str(), kt.c_str()) == 0) {
					it.filename.tokens.erase(tt);
					it.score += 3;
					matchingresults.push_back(it);	
				} else if (CaseInsensitiveFind(*tt, kt)) {
					it.filename.tokens.erase(tt);
					it.score += 1;
					matchingresults.push_back(it);	
				}
			}
		}
		interimresults = matchingresults;
	}
	std::deque<Directory::Result> results;
	for (auto it : interimresults) {
		m_log->Info("Matched: %s, score %d", it.filename.name.c_str(), it.score);
		std::string fullpath = m_path;
		fullpath.append("/");
		fullpath.append(it.filename.name);
		Directory::Result result = {fullpath, it.score};
		results.push_back(result);
	}
	return results;
}

bool Directory::Initialize(std::shared_ptr<Log> log, std::string const path)
{
	m_log = log;
	m_path = path;
	DIR* dir = opendir(m_path.c_str());
	if (!dir) {
		m_log->Info("Directory::Initialize: Could not open directory (%s).", m_path.c_str());
		return false;
	}
	while (true) {
		dirent* dirent = readdir(dir);
		if (!dirent) {
			break;
		}
		if (dirent->d_name[0] == '.') {
			continue;
		}
		FileName filename = {};
		filename.name = dirent->d_name;
		filename.tokens = Split(filename.name);
		m_filenames.push_back(filename);
	}
	if (closedir(dir)) {
		m_log->Info("Directory::Initialize: Could not close directory (%s).", m_path.c_str());
	}
	return true;
}

}
