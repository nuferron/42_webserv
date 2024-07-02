#include "Response.hpp"

////// Static Assets

std::map<std::string, std::string> Response::initStatus()
{
	std::map<std::string, std::string> error;

	error["100"] = "Continue";
	error["200"] = "OK";
	error["201"] = "Created";
	error["204"] = "No Content";
	error["302"] = "Found";
	error["400"] = "Bad Request";
	error["403"] = "Forbidden";
	error["404"] = "Not Found";
	error["405"] = "Method Not Allowed";
	error["406"] = "Not Acceptable";
	error["408"] = "Request Timeout";
	error["411"] = "Length Required";
	error["500"] = "Internal Server Error";
	error["501"] = "Not Implemented";
	error["505"] = "HTTP Version Not Supported";
	return (error);
}

std::map<std::string, std::string> Response::_status = Response::initStatus();
///////

Response::Response(): _cgi(""), _response(""), _body(""), _code(0), _servname("webserv"), _timeout(10000), _maxconnect(10), _connection(false), _path("./html/test.htmls"), _method("")
{}

Response::Response(const Response &r)
{
	this->_cgi = r._cgi;
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
}

Response::~Response() {}

Response	&Response::operator=(const Response &r)
{
	this->_cgi = r._cgi;
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
	return (*this);
}

void	Response::setBody(const std::string &msg)
{
	this->_body += msg;
}

void	Response::setCgi(const std::string &cgi)
{
	this->_cgi = cgi;
}

void	Response::setCode(const int &code)
{
	this->_code = code;
}

std::string	Response::_itoa(std::string::size_type n)
{
	std::string	str;

	if (n == 0)
		return ("0");
	while (n)
	{
		str.insert(0, 1, n % 10 + '0');
		n = n / 10;
	}
	return (str);
}

std::string	&Response::getResponse(const std::string &code)
{
	if (this->_cgi.empty() && this->_cgi.find("HTTP/") != std::string::npos)
		return (this->_cgi);
	this->putGeneralHeaders();
	if (!this->_path.empty())
	{
		if (this->fileToBody(this->_path))
			return (this->_response);
	}
	if (!this->_body.empty())
		this->_response += "Content-Length: " + this->_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;
	this->_response.insert(0, this->putStatusLine(code));
	return (this->_response);
}

std::string	Response::putStatusLine(const std::string &code)
{
	return ("HTTP/1.1 " + code + " " + this->_status.at(code) + "\r\n");
}

void	Response::putGeneralHeaders(void)
{
	std::time_t	date = std::time(NULL);
	this->_response += "Date: ";
	this->_response += std::asctime(std::localtime(&date));
	this->_response += "Server: " + _servname + "\n";
	this->_response += "Keep-Alive: timeout=" + this->_itoa(this->_timeout);
	this->_response += ", max=" + this->_itoa(this->_maxconnect) + "\n";
	if (this->_connection)
		this->_response += "Connection: close\n";
	else
		this->_response += "Connection: keep-alive\n";
}
/*
void	Response::putPostHeaders()
{
	std::ifstream	mime(mime.types);
	std::string		ext = this->
	if (!mime.is_open())
	{
		this->_code = 500;
		return ;
	}

}*/

bool	Response::fileToBody(const std::string &path)
{
	if (access(path.c_str(), F_OK))
		return (sendError("404", "errors/404.html"), 1);
	else if (access(path.c_str(), R_OK))
		return (sendError("403", "errors/403.html"), 1);
	std::ifstream	rdfile(path.c_str());
	if (!rdfile.is_open())
		return (sendError("500", "errors/500.html"), 1);
	std::ostringstream	content;
	content << rdfile.rdbuf();
	this->_body = content.str();
	return (0);
}

void	Response::sendError(const std::string &code, const std::string &path)
{
	this->_response = "Content-Type: text/html\n";
	if (fileToBody(path))
	{
		this->_response += "Content-Length: 75\n";
		this->_response += "\n<html><body><h1>505</h1>"
							"<h2>Severe Internal Server Error</h2></body></html>";
		this->_response.insert(0, "HTTP/1.1 505 Severe Internal Server Error\r\n");
		return ;
	}
	this->_response.insert(0, this->putStatusLine(code));
	this->_response += "Content-Length: " + this->_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;
}
