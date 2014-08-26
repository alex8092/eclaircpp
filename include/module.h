#ifndef ECLAIR_MODULE_H
# define ECLAIR_MODULE_H

# include "eclair.h"
# include "config.h"
# include "routes.h"
# include "route.h"
# include "request.h"
# include "response.h"

namespace Eclair
{
	class Module
	{
	private:
		explicit 						Module(Module&) = delete;
		explicit 						Module(Module&&) = delete;
		Module&							operator=(Module&) = delete;
		Module&							operator=(Module&&) = delete;
		std::string						_path;
		std::string						_name;
		std::string 					_version = std::string();
		std::map<std::string, Module*>	_submodules;
		Config 							*_config;
		Routes							_routes;		
		bool							_has_config = false;
		bool							_is_load = false;

		bool		load_submodules() noexcept;
		Module 		*get_module_asset(const string& uri);
		Response 	doStatic(Request *req) noexcept;
	protected:
		explicit 			Module() = default;

	public:
		virtual				 ~Module();

		inline void			setVersion(const std::string& version) noexcept {
			this->_version = version;
		}

		inline std::string	version() const noexcept {
			return (this->_version);
		}

		bool				load(const std::string& name, bool project = false) noexcept;

		Route 				*findRoute(const std::string& uri) noexcept;
		Response			*doRequest(Request *req) noexcept;

		inline std::string 	path() const noexcept {
			return (this->_path);
		}
	};
}

#endif