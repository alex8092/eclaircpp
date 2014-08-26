#ifndef ECLAIR_RESPONSE_H
# define ECLAIR_RESPONSE_H

# include "eclair.h"
# include "request.h"

namespace Eclair
{
	class Response
	{
	private:
		int			_status = 200;
		std::string	_status_text = std::string("OK");
		std::string	_filename;
		std::string _route;
		bool		_is_redirect = false;

	public:
		explicit	Response() = default;
		virtual		~Response() = default;

		inline void setStatus(int status) noexcept {
			this->_status = status;
			this->_status_text = this->statusToText(status);
		}

		inline int 	status() const noexcept {
			return (this->_status);
		}

		inline std::string statusText() const noexcept {
			return (this->_status_text);
		}

		inline void setFilename(const std::string& filename) noexcept {
			this->_filename = filename;
		}

		inline std::string filename() const noexcept {
			return (this->_filename);
		}

		inline void setRoute(const std::string& route) noexcept {
			this->_route = route;
		}

		inline std::string route() const noexcept {
			return (this->_route);
		}

		inline void setRedirect(bool value) {
			this->_is_redirect = true;
		}

		inline bool isRedirect() const noexcept {
			return (this->_is_redirect);
		}

		static std::string statusToText(int status) noexcept;
	};
}

#endif