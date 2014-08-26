#include "routes.h"
#include "project.h"
#include "config.h"

using namespace std;

Eclair::Routes::~Routes()
{
	for (auto it : this->_statics_routes)
		delete it.second;
}

bool	Eclair::Routes::reload(Eclair::Config& config) noexcept
{
	this->_config = &config;

	auto confs = config.configs();
	for (auto it = confs.begin(); it != confs.end(); ++it)
	{
		YAML::Node fileconf = it->second;
		cout << "file : " << it->first << endl;
		for (auto it2 = fileconf.begin(); it2 != fileconf.end(); ++it2)
		{
			YAML::Node routeconf = it2->second;
			YAML::Node pattern = routeconf["pattern"];
			if (pattern)
			{
				Eclair::Route* route = new Route();
				route->setPattern(pattern.as<string>());
				YAML::Node controller = routeconf["controller"];
				YAML::Node file;
				if (controller)
					route->setController(controller.as<string>());
				else if ((file = routeconf["file"]))
					route->setFile(file.as<string>());
				else
				{
					cerr << "You need file or controller config" << endl;
					return (false);
				}
				this->_statics_routes[route->pattern()] = route;
			}
			cout << "\troute: " << it2->first << endl;
			cout << "\t\tpattern: " << routeconf["pattern"] << endl;
		}
	}
	return (true);
}

Eclair::Route	*Eclair::Routes::find(const string& uri) noexcept
{
	auto it = this->_statics_routes.find(uri);
	if (it != this->_statics_routes.end())
		return (it->second);
	/*Eclair::Route 	*route = nullptr;

	auto it = this->_config.begin();
	for (; it != this->_config.end(); ++it)
	{
		YAML::Node	confs = it->second;
		YAML::Node 	pattern = confs["pattern"];
		string		pattern_str;
		
		if (pattern && !(pattern_str = pattern.as<string>()).compare(uri))
		{
			route = new Route();
			route->setPattern(pattern_str);
			YAML::Node controller = confs["controller"];
			YAML::Node file;
			if (controller)
				route->setController(controller.as<string>());
			else if ((file = confs["file"]))
				route->setFile(file.as<string>());
			break ;
		}
	}*/
	return (nullptr);
}

string		Eclair::Routes::uri(const string& route) const noexcept
{
	stringstream 	input(route);
	string 			word;

	cout << "in" << endl;
	//while (std::getline(input, word, '.'))
	//{
		cout << "input : " << word << endl;
	//}
	return ("/next");
}