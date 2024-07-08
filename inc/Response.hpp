#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <map>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "Cgi.hpp"
#include "Utils.hpp"

class	Response
{
	public:
		Response();
		~Response();
		//SET VARS
		void		setBody(const std::string &msg);
		void		setCgiPath(const std::string &cgi);
		void		setCode(const int &code);
		//WRITE RESPONSE
		std::string	putStatusLine(int code);
		void		putGeneralHeaders(void);
		bool		putPostHeaders(const std::string &file);
		int			fileToBody(const std::string &path);
		//SEND RESPONSE
		std::string	&getResponse(int code);
		void		sendError(int code);
		//TMP
		void		setSocket(int sock);
		void		setMethod(const std::string &meth);

	private:
		static std::map<int, std::string>	_status;
		static std::map<int, std::string>	initStatus();
		std::string		_body;
		std::string		_reqbody;
		std::string		_response;
		std::string		_cgi_path;
		std::string		_path; //tmp
		std::string		_servname; //tmp
		std::string		_method; //tmp
		int				_socket; //tmp
		int				_timeout; //tmp
		int				_maxconnect; //tmp
		bool			_connection; //tmp
		unsigned int	_code;

		Response(const Response &r);
		Response	&operator=(const Response &r);
		void		_parseCgiResponse(void);
};

#endif
