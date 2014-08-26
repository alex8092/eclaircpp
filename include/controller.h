#ifndef ECLAIR_CONTROLLER_H
# define ECLAIR_CONTROLLER_H

# include "eclair.h"
# include "request.h"
# include "response.h"

namespace Eclair
{
	class Module;
	class Controller
	{
	private:
		static std::map<Module*, std::map<std::string, Controller*>*> _s_controllers;
		std::map<std::string, std::function<Response*(void)>>		_funcs;
		void														*_lib_handler = nullptr;
		std::string													_name;
		Request														*_req = nullptr;
		Module 														*_module = nullptr;
		static inline void											_func_error(const std::string& name, const std::string& error) noexcept {
			std::cerr << "Controller [ " << name << " ] : " << error << std::endl;
		}

	public:
		enum class	RedirectType { FOUND = 302, MOVE_PERMANENT = 301, SEE_OTHER = 303, TEMPORARY_REDIRECT = 307 };
		explicit Controller() = default;
		virtual ~Controller();

		template <class T>
		inline void			addFunction(const std::string& name, Response *(T::*func)(void)) noexcept
		{
			this->_funcs[name] = std::bind(func, static_cast<T*>(this) );
		}

		inline Response		*call(const std::string& name)
		{
			return (this->_funcs[name]());
		}

		inline void			setRequest(Request *req) noexcept {
			this->_req = req;
		}

		inline Request 		*request() const noexcept {
			return (this->_req);
		}

		inline Response 	*doRequest(Request *req, const std::string& function) {
			if ((this->_req = req))
				return (this->call(function));
			return (nullptr);
		}

		Response			*render(const std::string& file) const noexcept;
		Response 			*redirect(const std::string& route, RedirectType type = RedirectType::FOUND) const noexcept;

		static Controller 	*get(Module *mod, const std::string& path) noexcept;
		static Controller 	*find(Module* mod, const std::string& controller, std::string& function) noexcept;
		static std::string	decompose(const std::string& fullpath, std::string& function) noexcept;
		static void			destroy();
	};
}

#endif