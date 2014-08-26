#include "config.h"
#include "file.h"

using namespace std;

Eclair::Config 		*Eclair::Config::_global = nullptr;

Eclair::Config::Config(const string &base_folder) :
	_base_folder(base_folder)
{

}

Eclair::Config::~Config()
{
	for (auto it : this->_configs_folder)
		delete it.second;
}

bool	Eclair::Config::load_folder(Eclair::Directory& dir) noexcept
{
	Eclair::DirectoryFile	*file;

	while ((file = dir.next()))
	{
		if (file->is_directory)
		{
			cout << "config folder : " << file->name << " - " << dir.path() + "/" + file->name << endl;
			Eclair::Config 		*conf = new Config(dir.path() + "/" + file->name);
			if (!conf->reload())
			{
				cerr << "Unable to load configs" << endl;
				return (false);
			}
			this->_configs_folder[file->name] = conf;
		}
		else if (file->is_regular_file)
		{
			std::string filename, ext;
			filename = Eclair::File::noext(file->name, ext);
			if (ext.compare("yml"))
			{
				cerr << "Wrong file extension : " << ext << endl;
				return (false);
			}
			try
			{
				cout << this->_base_folder << " [" << filename << "]" << endl;
				this->_configs[filename] = YAML::LoadFile((dir.path() + "/" + filename + ".yml").c_str());
			} catch (...) {
				cerr << "not valid yaml : " << dir.path() + "/" + filename + ".yml" << endl;
				return (false);
			}
		}
	}
	return (true);
}

bool	Eclair::Config::reload() noexcept
{
	Eclair::Directory	dir(this->_base_folder);

	if (!dir.open())
	{
		cerr << "Cannot open config directory : " << dir.path() << endl;
		return (false);
	}
	return (this->load_folder(dir));
}

YAML::Node	Eclair::Config::operator[](const string& item)
{
	return this->_configs[item];
}

bool	Eclair::Config::exist(const string& name, bool folder) const noexcept
{
	if (folder)
	{
		auto itbis = this->_configs_folder.find(name);
		return (itbis != this->_configs_folder.end());
	}
	else
	{
		auto it = this->_configs.find(name);
		return (it != this->_configs.end());
	}
}

Eclair::Config 	&Eclair::Config::global() noexcept
{
	if (!Config::_global)
	{
		Config::_global = new Config("config");
		Config::_global->reload();
		if (Config::_global->exist("global"))
		{
			string folder = (*Config::_global)["global"]["projects"]["folder"].as<string>();
			if (folder[0] != '/')
			{
				folder = Eclair::Directory::current() + "/" + folder;
				(*Config::_global)["global"]["projects"]["folder"] = folder;
				cout << "folder : " << folder << endl;
			}
		}
	}
	return (*Config::_global);
}