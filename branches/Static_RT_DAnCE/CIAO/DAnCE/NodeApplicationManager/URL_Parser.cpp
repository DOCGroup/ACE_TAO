// $Id$

#include "URL_Parser.h"

#include "ace/ACE.h"
#include "ace/OS_NS_string.h"

bool
URL_Parser::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("rwu:h:p:f:d"));

  bool success = true;
  int c;

  while ((c = get_opt ()) != -1)
    {
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
            break;
        }
    }

  if (this->hostname_ == 0 || this->filename_ == 0)
    {
      success = false;
    }

  return success;
}

URL_Parser::URL_Parser (void)
  : hostname_ (ACE::strnew ("127.0.0.1")),
    port_ (ACE_DEFAULT_HTTP_SERVER_PORT),
    filename_ (0),
    debug_ (0)
{
}

bool URL_Parser::parseURL (char* url)
{
  char* ptr = ACE_OS::strstr (url, "http://");
  bool success = true;

  if (0 != ptr)
    {
      url += ACE_OS::strlen ("http://");
    }

  if (url[0] == '/')
    {
      this->filename_ = ACE_OS::strdup (url);
    }
  else
    {
      ptr = ACE_OS::strstr (url, ":");

      if (0 != ptr)
        {
          this->port_ = ACE_OS::atoi (ptr + 1);
        }
      else
        {
          ptr = ACE_OS::strstr (url, "/");
        }

      if (0 == ptr)
        {
          success = false;
        }
      else
        {
          size_t host_len = ptr - url;
          ACE::strdelete (this->hostname_);
          ACE_NEW_RETURN (this->hostname_, char [host_len + 1], false);
          ACE_OS::strncpy (this->hostname_, url, host_len);
          this->hostname_ [host_len] = '\0';
          ptr = ACE_OS::strstr (ptr, "/");

          if (0 != ptr)
            {
              this->filename_ = ACE_OS::strdup (ptr);
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
  ACE_DEBUG ((LM_DEBUG,
              "./http_client -u http://hostname:port/filename [-d]\n"));
}


URL_Parser::~URL_Parser (void)
{
  delete [] this->hostname_;
  ACE_OS::free (this->filename_);
}
