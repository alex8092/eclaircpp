#ifndef ECLAIR_FILE_H
# define ECLAIR_FILE_H

# include "eclair.h"

namespace Eclair
{
	class File
	{
	public:
		explicit File() noexcept = default;
		virtual ~File() = default;

		static std::string	noext(const std::string& filename, std::string& ext);
		static std::string	basename(const std::string& filepath, std::string& dir);
	};
}

#endif