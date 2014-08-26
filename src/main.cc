#include "eclair.h"
#include "request.h"
#include "directory.h"
#include "project.h"
#include "routes.h"
#include "mongoose.h"
#include "fcgio.h"
#include "controller.h"
#include "file.h"

using namespace std;

static int ev_handler(struct mg_connection *conn, enum mg_event ev)
{
	switch (ev)
	{
		case MG_REQUEST:
		{
			Eclair::Project *proj = nullptr;
			try
			{
				 proj = Eclair::Project::getByHost(mg_get_header(conn, "host"));
			}
			catch (...)
			{
				return MG_FALSE;
			}
			if (!proj)
				return MG_FALSE;
			Eclair::Request     *req = new Eclair::Request(conn->uri);
			Eclair::Response 	*rep = proj->doRequest(req);
			if (rep)
			{
				mg_send_status(conn, rep->status());
				mg_send_file(conn, rep->filename().c_str());
				delete req;
				delete rep;
				return (MG_MORE);
			}
			delete req;
			return MG_FALSE;
		}
		case MG_AUTH:
		{
			return MG_TRUE;
		}
		default:
		{
			return MG_FALSE;
		}
	}
}

bool continuer = true;

static void stop_server(int)
{
	continuer = false;
}

int	main(int argc, char **argv)
{
	Eclair::Config  &conf = Eclair::Config::global();
	if (conf.exist("global"))
	{
		YAML::Node projects = conf["global"]["projects"];
		if (!projects["folder"])
		{
			cerr << "You need the projects.folder in config/global.yml" << endl;
			return (-1);
		}
		if (projects)
		{
			auto it = projects.begin();
			for (; it != projects.end(); ++it)
			{
				string first = it->first.as<string>();
				if (first.compare("folder"))
				{
					Eclair::Project *proj = Eclair::Project::get(first);
					if (!proj->loadProject(it->second))
						return (-1);
				}
			}
		}
		else
		{
			cerr << "You need projects in config/global.yml" << endl;
			return (-2);
		}
	}
	else
	{
		cerr << "You need config/global.yml" << endl;
		return (-3);
	}
	signal(SIGINT, stop_server);
	struct mg_server *server = mg_create_server(NULL, ev_handler);
	mg_set_option(server, "listening_port", "8080");
	while (continuer) mg_poll_server(server, 1000);
	mg_destroy_server(&server);
	cout << "Server is stopping" << endl;
	Eclair::Project::destroy();
	Eclair::Controller::destroy();
	return (0);
}
