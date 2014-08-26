#ifndef ECLAIR_CONFIG_H
# define ECLAIR_CONFIG_H

# include "eclair.h"
# include "directory.h"

namespace Eclair
{
	class Config
	{
	private:
		std::string							_base_folder;
		std::map<std::string, YAML::Node>	_configs;
		std::map<std::string, Config*>		_configs_folder;
		static Config 						*_global;

		bool		load_folder(Eclair::Directory& dir) noexcept;
	public:
		explicit	Config(const std::string& folder);
		virtual		~Config();
		bool		reload() noexcept;
		bool		exist(const std::string& name, bool folder = false) const noexcept;
		YAML::Node	operator[](const std::string& item);

		static Config&		global() noexcept;
		static inline std::string	projectsFolder() {
			return (global()["global"]["projects"]["folder"].as<std::string>());
		}

		inline std::map<std::string, YAML::Node>& configs() noexcept {
			return (this->_configs);
		}

		inline std::map<std::string, Config*>& configFolders() noexcept {
			return (this->_configs_folder);
		}

		inline Config 		&folder(const std::string& folder) {
			return (*this->_configs_folder[folder]);
		}
	};
}

#endif