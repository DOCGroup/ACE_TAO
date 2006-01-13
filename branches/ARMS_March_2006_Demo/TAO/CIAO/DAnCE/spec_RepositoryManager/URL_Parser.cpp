// $Id$

#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "URL_Parser.h"

#include "ace/OS_NS_string.h"

URL_Parser* URL_Parser::instance_ = 0;

URL_Parser*
URL_Parser::instance (void)
{

  if (URL_Parser::instance_ == 0) 
    URL_Parser::instance_ = new URL_Parser ();

  return URL_Parser::instance_;
}

bool
URL_Parser::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("rwu:h:p:f:d"));

  bool success = true;
  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'd':
        this->debug_ = 1;
        break;
	  case 'u':
		  success = parseURL (get_opt.opt_arg ());
		  break;
        // Usage fallthrough.
      default:
        success = false;
      }
  if (this->hostname_ == 0 || this->filename_ == 0)
	success = false;

  return success;
}

URL_Parser::URL_Parser (void) 
  : hostname_ ("127.0.0.1"),
    port_ (ACE_DEFAULT_HTTP_SERVER_PORT),
    filename_ (0),
    debug_ (0)
{
}



bool URL_Parser::parseURL (char* url)
{
	char* ptr; 
	bool success = true;

	if (ptr = ACE_OS::strstr (url, "http://"))
		url += ACE_OS::strlen ("http://");

	if (url[0] == '/')
		this->filename_ = url;
	else
	{
		if (ptr = ACE_OS::strstr (url, ":"))
			this->port_ = ACE_OS::atoi (ptr + 1);
		else
			ptr = ACE_OS::strstr (url, "/");

		if(!ptr)
			success = false;
		else
		{
			size_t host_len = ptr - url;
			this->hostname_ = new char [host_len];
			ACE_OS::strncpy (this->hostname_, url, host_len);
			this->hostname_ [host_len] = '\0';

			if (ptr = ACE_OS::strstr (ptr, "/"))
			{
				this->filename_ = ptr;
			}
			else
				success = false;
		}
	}
	return success;
}


void URL_Parser::exitOnError (void)
	{
		ACE_DEBUG ((LM_DEBUG, "./http_client -u http://hostname:port/filename [-d]\n"));
		ACE_OS::exit (1);
	}


	URL_Parser::~URL_Parser()
	{
		if(this->hostname_)
			delete [] this->hostname_;
	}
