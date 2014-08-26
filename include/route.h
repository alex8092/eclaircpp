#ifndef ECLAIR_ROUTE_H
# define ECLAIR_ROUTE_H

# include "eclair.h"

namespace Eclair
{
	class Route
	{
	private:
		std::string 	_controller;
		std::string		_file;
		std::string		_pattern;
	public:
		explicit Route() = default;
		virtual ~Route() = default;

		inline std::string controller() const noexcept {
			return (this->_controller);
		}
		inline std::string file() const noexcept {
			return (this->_file);
		}
		inline std::string pattern() const noexcept {
			return (this->_pattern);
		}

		inline void setController(const std::string& controller) noexcept {
			this->_controller = controller;
		}
		inline void setFile(const std::string& file) noexcept {
			this->_file = file;
		}
		inline void setPattern(const std::string& pattern) noexcept {
			this->_pattern = pattern;
		}
	};
}

#endif