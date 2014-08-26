#ifndef ECLAIR_PROJECT_H
# define ECLAIR_PROJECT_H

# include "eclair.h"
# include "config.h"
# include "module.h"

namespace Eclair
{
	class Project : public Module
	{
	private:
		static std::map<std::string, Project*>								_projects;
		static std::map<std::string, Project*>								_projects_by_host;
		YAML::Node															_config_proj;
		std::string 														_projname;

		explicit 									Project() = default;
		explicit 									Project(Project&) = delete;
		explicit 									Project(Project&&) = delete;
		Project 									operator=(Project&) = delete;
		Project 									operator=(Project&&) = delete;

	public:
		virtual			 			~Project() = default;

		static Project 				*get(const std::string& name) noexcept;
		static inline Project 		*getByHost(const std::string& host) noexcept {
			return (Project::_projects_by_host[host]);
		}
		virtual bool				loadProject(YAML::Node config) noexcept;
		static void					destroy() noexcept;
	};
}

#endif