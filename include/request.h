#ifndef ECLAIR_REQUEST_H
# define ECLAIR_REQUEST_H

# include "eclair.h"
# include "mongoose.h"

# define REQUEST_TYPE_GET	0
# define REQUEST_TYPE_POST	1

namespace Eclair
{
	class Request
	{
	public:
		explicit Request(const std::string& uri);
		virtual ~Request() = default;

		inline std::string uri() const noexcept {
			return (this->_uri);
		}

	private:
		char					_type = REQUEST_TYPE_GET;
		std::string				_uri;
	};
}

#endif