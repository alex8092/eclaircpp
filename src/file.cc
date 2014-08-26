#include "file.h"

using namespace std;

string	Eclair::File::noext(const string& filename, string& ext)
{
	size_t index = filename.find_last_of('.');
	ext = "";
	if (index == string::npos)
		return (filename);
	ext = filename.substr(index + 1, filename.length() - index - 1);
	return (filename.substr(0, index));
}

string	Eclair::File::basename(const string& filepath, string& dir)
{
	size_t index = filepath.find_last_of('/');
	dir = ".";
	if (index == string::npos)
		return (filepath);
	dir = filepath.substr(0, index);
	return (filepath.substr(index + 1, filepath.length() - index - 1));
}