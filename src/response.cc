#include "response.h"
#include "project.h"
#include "mongoose.h"

using namespace std;

std::string		Eclair::Response::statusToText(int status) noexcept
{
	switch (status)
	{
		case 200:
			return ("OK");
		case 301:
			return ("Moved Permanently");
		case 302:
			return ("Found");
		case 303:
			return ("See Other");
		case 304:
			return ("Temporary Redirect");
		default:
			return ("Unknow");
	}
}