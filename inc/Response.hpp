#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#ifndef TIMEOUT
# define TIMEOUT 10
#endif

#ifndef MAXCONNECT
# define MAXCONNECT 10
#endif

#include <map>
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include "Cgi.hpp"
#include "Utils.hpp"
#include "Request.hpp"

class Request;

class	Response
{
	public:
		Response();
		Response(const Response &r);
		//Response(Request &req);
		~Response();
		//SET VARS
		void		setBody(const std::string &msg);
		void		setCgiPath(const std::string &cgi);
		void		setCode(const int &code);
		void		setSocket(int socket);
		//WRITE RESPONSE
		std::string	putStatusLine(int code);
		void		putGeneralHeaders(void);
		bool		putPostHeaders(const std::string &file);
		int			fileToBody(const std::string &path);
		//SEND RESPONSE
		std::string	&getResponse(int code);
		void		sendError(int code);

	private:
		static std::map<int, std::pair<std::string, std::string> >	_status;
		static std::map<int, std::pair<std::string, std::string> >	_initStatus();
		std::string		_body;
		std::string		_query;
		std::string		_reqbody;
		std::string		_response;
		//std::string		_cgi_path;
		std::string		_path; //tmp
		std::string		_servname; //tmp
		std::string		_method; //tmp
		std::string		_host; //tmp
		int				_socket; //tmp
		int				_port; //tmp
		bool			_cgi; //tmp
		bool			_keep_alive; //tmp
		std::vector<std::string>	_cgiargs;
		unsigned int	_code;
		Request			*_req;

		Response	&operator=(const Response &r);
		void		_parseCgiResponse(void);
		void		_handleGet(void);
		void		_handlePost(void);
		void		_handleDelete(void);
		std::string	_parseUrl(const std::string &url);
		std::vector<std::string> _setCgi(std::string &path);
		bool		_createFile(void);
		bool		_isNotAccepted(std::string str);
		void		_makeAutoIndex(void);
};

#endif
