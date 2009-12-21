// $Id$

#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "URL_Parser.h"
#include "ace/ACE.h"

#include "DAnCE/Logger/Log_Macros.h"

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
        this->debug_ = true;
        break;
      case 'u':
        success = parseURL (get_opt.opt_arg ());
        break;
        // Usage fallthrough.
      default:
        success = false;
      }

  if (this->hostname_ == 0 || this->filename_ == 0)
    {
      success = false;
    }

  return success;
}

URL_Parser::URL_Parser (void)
  : hostname_ (ACE::strnew (ACE_TEXT("127.0.0.1"))),
    port_ (ACE_DEFAULT_HTTP_SERVER_PORT),
    filename_ (0),
    debug_ (false)
{
}

bool URL_Parser::parseURL (ACE_TCHAR* url)
{
  ACE_TCHAR* ptr = 0;
  bool success = true;
  ptr = ACE_OS::strstr (url, ACE_TEXT("http://"));
  if (ptr)
    url += ACE_OS::strlen (ACE_TEXT("http://"));

  if (url[0] == '/')
    {
      this->filename_ = ACE_OS::strdup (url);
    }
  else
    {
      ptr = ACE_OS::strstr (url, ":");
      if (ptr)
        this->port_ = ACE_OS::atoi (ptr + 1);
      else
        ptr = ACE_OS::strstr (url, "/");

      if(!ptr)
        success = false;
      else
        {
          size_t host_len = ptr - url;
          ACE::strdelete (this->hostname_);
          ACE_NEW_RETURN (this->hostname_, char [host_len + 1], false);
          ACE_OS::strncpy (this->hostname_, url, host_len);
          this->hostname_ [host_len] = '\0';
          ptr = ACE_OS::strstr (ptr, "/");
          if (ptr)
            {
              this->filename_ = ACE_OS::strdup(ptr);
            }
          else
            {
              success = false;
            }
        }
    }
    return success;
}


void URL_Parser::Error (void)
{
  DANCE_DEBUG (6, (LM_DEBUG, "./http_client -u http://hostname:port/filename [-d]\n"));
}

URL_Parser::~URL_Parser()
{
  delete [] this->hostname_;
  ACE_OS::free (this->filename_);
}
