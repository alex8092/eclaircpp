#ifndef ECLAIR_ROUTES_H
# define ECLAIR_ROUTES_H

# include "eclair.h"
# include "route.h"
# include "config.h"

namespace Eclair
{
	class Routes
	{
	private:
		Routes(Routes&) 						= delete;
		Routes(Routes&&) 						= delete;
		Routes& 								operator=(Routes&) = delete;
		Routes& 								operator=(Routes&&) = delete;
		Config 									*_config;
		std::map<std::string, Route*>			_statics_routes;

	public:
		explicit Routes() = default;
		virtual ~Routes();
		bool 		reload(Config &config) noexcept;
		Route 		*find(const std::string& uri) noexcept;
		std::string	uri(const std::string& route) const noexcept;
	};
}

#endif