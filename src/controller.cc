#include "controller.h"
#include "file.h"
#include "project.h"
#include "module.h"

using namespace std;

map<Eclair::Module*, map<string, Eclair::Controller*>*> Eclair::Controller::_s_controllers;

Eclair::Controller::~Controller()
{
	if (this->_lib_handler)
		dlclose(this->_lib_handler);
}

Eclair::Controller 	*Eclair::Controller::get(Module *mod, const string& path) noexcept
{
	string 		name, dir, to_load;
	void		*lib_handler = nullptr;
	Controller 	*(*create)(void);
	Controller 	*c = nullptr;

	auto map_controllers = Eclair::Controller::_s_controllers.find(mod);
	if (map_controllers == Eclair::Controller::_s_controllers.end())
		Eclair::Controller::_s_controllers[mod] = new map<string, Eclair::Controller*>();
	auto it = Eclair::Controller::_s_controllers[mod]->find(path);
	if (it != Eclair::Controller::_s_controllers[mod]->end())
		return (it->second);
	name = Eclair::File::basename(path, dir);
	to_load = mod->path() + "/controllers/" + dir + "/lib" + name + ".so";
	lib_handler = dlopen(to_load.c_str(), RTLD_LAZY);
	if (!lib_handler)
	{
		Eclair::Controller::_func_error(path, dlerror());
		return (nullptr);
	}
	create = (Controller *(*)(void))dlsym(lib_handler, "create");
	if (!create)
	{
		dlclose(lib_handler);
		Eclair::Controller::_func_error(path, dlerror());
		return (nullptr);
	}
	c = create();
	if (c == nullptr)
	{
		dlclose(lib_handler);
		Eclair::Controller::_func_error(path, "Method create return nullptr");
		return (nullptr);
	}
	c->_module = mod;
	c->_lib_handler = lib_handler;
	c->_name = path;
	Eclair::Controller::_s_controllers[mod]->operator[](path) = c;
	return (c);
}

Eclair::Controller 	*Eclair::Controller::find(Eclair::Module *mod, const string& controller, std::string& function) noexcept
{
	string path;
    path = Eclair::Controller::decompose(controller, function);
    return (Eclair::Controller::get(mod, path));
}

string				Eclair::Controller::decompose(const string& fullpath, string& function) noexcept
{
	size_t index = fullpath.find_last_of(':');
	function = "";
	if (index == string::npos)
		return ("");
	function = fullpath.substr(index + 1, fullpath.length() - index - 1);
	return (fullpath.substr(0, index));
}

Eclair::Response	*Eclair::Controller::render(const std::string &file) const noexcept
{
	Eclair::Response 	*rep = new Eclair::Response();

	//cout << "module : " << (size_t)this->_module << endl;
	string filename = this->_module->path() + "/ressources/views/" + file;
	rep->setFilename(filename);
	return (rep);
}

Eclair::Response 	*Eclair::Controller::redirect(const std::string& route, Eclair::Controller::RedirectType type) const noexcept
{
	Eclair::Response 	*rep = new Eclair::Response();

	rep->setRedirect(true);
	rep->setRoute(route);
	rep->setStatus((int)type);
	return (rep);
}

void				Eclair::Controller::destroy()
{
	for (auto mapcontrols : Eclair::Controller::_s_controllers)
	{
		for (auto it : *mapcontrols.second)
		{
			void (*del)(Eclair::Controller*);
			del = (void(*)(Eclair::Controller*))dlsym(it.second->_lib_handler, "destroy");
			//del(it.second);
		}
		delete mapcontrols.second;
	}
}