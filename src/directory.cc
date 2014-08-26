#include "directory.h"
#include <errno.h>
#include <cstring>

using namespace std;

Eclair::Directory::Directory() {}

Eclair::Directory::Directory(const std::string& dir)
{
	this->_dirname = dir;
}

Eclair::Directory::~Directory() noexcept
{
	if (this->_dir != nullptr)
		closedir(this->_dir);
}

bool					Eclair::Directory::open() noexcept
{
	this->_dir = opendir(this->_dirname.c_str());
	if (!this->_dir)
	{
		this->_error_str = strerror(errno);
		return (false);
	}
	return (true);
}

Eclair::DirectoryFile	*Eclair::Directory::next() noexcept
{
	struct dirent					*item;

	if (this->_dir == nullptr)
		return (nullptr);
	while ((item = readdir(this->_dir)) && ((item->d_name[0] == '.' && !item->d_name[1]) || (item->d_name[0] == '.' && item->d_name[1] == '.' && !item->d_name[2])));
	if (item)
	{
		this->_file.name = item->d_name;
		this->_file.is_directory = (item->d_type == DT_DIR);
		this->_file.is_regular_file = (item->d_type == DT_REG);
		return (&this->_file);
	}
	return (nullptr);
}