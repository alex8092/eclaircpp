#include "project.h"
#include "directory.h"
#include "routes.h"

using namespace std;

map<string, Eclair::Project*>		Eclair::Project::_projects;
map<string, Eclair::Project*>		Eclair::Project::_projects_by_host;

Eclair::Project 	*Eclair::Project::get(const string& name) noexcept
{
	auto		it = Eclair::Project::_projects.find(name);
	if (it != Eclair::Project::_projects.end())
		return (it->second);
	else
	{
		Project 	*proj = new Project();
		Eclair::Project::_projects[name] = proj;
		proj->_projname = name;
		return (proj); 
	}
}

bool				Eclair::Project::loadProject(YAML::Node config) noexcept {
	this->_config_proj = config;
	YAML::Node host = config["host"];
	if (!host)
	{
		cerr << "cannot find host config for projects" << endl;
		return (false);
	}
	Eclair::Project::_projects_by_host[host.as<string>()] = this;
	return (this->load(this->_projname, true));
}


void				Eclair::Project::destroy() noexcept
{
	for (auto it = Eclair::Project::_projects.begin(); it != Eclair::Project::_projects.end(); ++it)
	{
		delete it->second;
	}
	Eclair::Project::_projects.clear();
	Eclair::Project::_projects_by_host.clear();
}