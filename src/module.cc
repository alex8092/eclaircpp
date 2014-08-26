#include "module.h"
#include "controller.h"

using namespace std;

Eclair::Module::~Module()
{
	for (auto it : this->_submodules)
		delete it.second;
	if (this->_config)
		delete this->_config;
}

bool		Eclair::Module::load(const string& name, bool project) noexcept
{
	this->_name = name;
	this->_path = Eclair::Config::projectsFolder() + ((project) ? "/" : "/lib/") + name;
	if (!this->_version.empty())
		this->_path += "/" + this->_version; 
	Eclair::Directory 		dir(this->_path);
	Eclair::DirectoryFile	*file;

	cout << "module [" << this->path() << "] is loading" << endl;
	this->_has_config = false;
	this->_is_load = false;
	if (!dir.open())
	{
		cerr << "Unable to open folder : " << dir.path() << endl;
		return (false);
	}
	while ((file = dir.next()))
	{
		if (file->is_directory && !file->name.compare("config"))
		{
			this->_config = new Eclair::Config(dir.path() + "/config");
			if (!this->_config || !this->_config->reload())
				return (false);
			this->_has_config = true;
		}
	}
	if (!this->_has_config)
	{
		cerr << "You need the config directory" << endl;
		return (false);
	}
	if (this->_config->exist("routes", true))
	{
		if (!this->_routes.reload(this->_config->folder("routes")))
		{
			cerr << "[Error] module [" << this->path() << "] : cannot load routes" << endl;
			return (false);
		}
	}
	else
		cerr << "[Warning] module [" << this->path() << "] : no routes found" << endl;
	if (!load_submodules())
	{
		cerr << "Fail to load submodules" << endl;
	}
	this->_is_load = true;
	return (true);
}

bool		Eclair::Module::load_submodules() noexcept
{
	if (this->_config->exist("modules"))
	{
		YAML::Node modules = (*this->_config)["modules"]["modules"];
		if (modules)
		{
			cout << "module [" << this->path() << "] : " << endl;
			for (auto it = modules.begin(); it != modules.end(); ++it)
			{
				Eclair::Module 		*mod = new Eclair::Module();
				string modulename = it->first.as<string>();
				cout << "\tsubmodule : [" << modulename << "]";
				YAML::Node	modconfs = it->second;
				YAML::Node	version = modconfs["version"];
				if (version)
					mod->setVersion("v" + version.as<string>());
				if (!this->version().empty())
					cout << " - v" << this->version();
				cout << endl;
				if (mod->load(modulename))
				{
					auto it = this->_submodules.find(modulename);
					if (it != this->_submodules.end())
					{
						cerr << "Two same modules" << endl;
						return (false);
					}
					this->_submodules[modulename] = mod;
				}
				else
					return (false);
			}
		}
	}
	return (true);
}

Eclair::Module 			*Eclair::Module::get_module_asset(string& uri)
{
	Eclair::Module 	*mod = nullptr;

	string assets_begin = "/assets-" + this->_name + "/";
	if (uri.find(assets_begin) == 0)
	{
		uri = uri.substr(assets_begin.length() - 1);
		mod = this;
	}
	else
	{
		if ()
	}
	return (mod);
}

Eclair::Response		*Eclair::Module::doStatic(Eclair::Request *req) noexcept
{
	Eclair::Response	*rep = nullptr;

	
}

Eclair::Response		*Eclair::Module::doRequest(Eclair::Request *req) noexcept
{
	Eclair::Response 	*rep = nullptr;
	if ((rep = this->doStatic(req)))
		return (rep);
	Eclair::Route 		*route = nullptr;
	route = this->_routes.find(req->uri());
	//cout << "module [" << this->path() << "] : " << req->uri() << endl;
	if (route)
	{
		//cout << "\tfind route [" << req->uri() << "]" << endl;
		string param;
		if (!(param = route->controller()).empty())
		{
			string function;
			Eclair::Controller *c = Eclair::Controller::find(this, param, function);
			if (c)
				return (c->doRequest(req, function));
		}
		else if (!(param = route->file()).empty())
		{
			rep = new Eclair::Response();
			rep->setFilename(this->path() + "/ressources/views/" + param);
			//cout << "filename : " << rep->filename() << endl;
		}
	}
	else
	{
		for (auto it = this->_submodules.begin(); it != this->_submodules.end(); ++it)
		{
			if ((rep = it->second->doRequest(req)))
				return (rep);
		}
	}
	return (rep);
}