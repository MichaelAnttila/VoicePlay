#include <algorithm>
#include <stdio.h>
#include <dirent.h>
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

std::vector<std::string> Directory::Match(std::string const keywords)
{
	std::vector<std::string> result;
	for (auto it: m_filenames) {
		if (CaseInsensitiveFind(it, keywords)) {
			std::string fullpath = m_path;
			fullpath.append("/");
			fullpath.append(it);
			result.push_back(fullpath);
		}
	}
	return result;
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
		m_filenames.push_back(dirent->d_name);
	}
	if (closedir(dir)) {
		m_log->Info("Directory::Initialize: Could not close directory (%s).", m_path.c_str());
	}
	return true;
}

}
