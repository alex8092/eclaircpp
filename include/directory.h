#ifndef ECLAIR_DIRECTORY_H
# define ECLAIR_DIRECTORY_H

# include "eclair.h"
# include <sys/types.h>
# include <dirent.h>

namespace Eclair
{
	struct DirectoryFile
	{
		std::string name;
		bool		is_directory;
		bool		is_regular_file;
	};

	class Directory
	{
	private:
		std::string		_dirname;
		DIR				*_dir = nullptr;
		std::string		_error_str;
		DirectoryFile	_file;
		explicit	Directory();
	public:
		explicit 			Directory(const std::string& dir);
		virtual 			~Directory() noexcept;
		bool				open() noexcept;
		DirectoryFile		*next() noexcept;
		inline std::string 	error() const noexcept {
			return (this->_error_str);
		}
		inline std::string	path() const noexcept {
			return (this->_dirname);
		}
		static inline std::string current() noexcept {
			char str[1025];
			return (getcwd(str, 1024));
		}
	};
}

#endif