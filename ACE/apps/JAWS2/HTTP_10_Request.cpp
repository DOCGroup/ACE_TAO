#include "HTTP_10_Request.h"
#include "ace/OS_NS_pwd.h"
#include "ace/OS_NS_stdlib.h"
#include <cstring>

const char *
JAWS_HTTP_10_Request::method () const
{
  return this->request_line ()->method_str ();
}

const char *
JAWS_HTTP_10_Request::uri () const
{
  return this->request_line ()->url ();
}

const char *
JAWS_HTTP_10_Request::version () const
{
  return this->request_line ()->version ();
}

int
JAWS_HTTP_10_Request::type () const
{
  return this->request_line ()->method ();
}

const char *
JAWS_HTTP_10_Request::path () const
{
  if (this->path_.empty ())
    this->set_path (this->uri ());
  return this->path_.empty () ? nullptr : this->path_.c_str ();
}

void
JAWS_HTTP_10_Request::set_status (int s)
{
  HTTP_Request::set_status (s);
}

void
JAWS_HTTP_10_Request::path (const char *uri_string)
{
  this->set_path (uri_string);
}

void
JAWS_HTTP_10_Request::set_path (char const *uri_string) const
{
  this->path_.clear ();
  if (uri_string == nullptr || uri_string[0] != '/')
    return;

  if (uri_string[1] == '~')
    {
      char const * const username = uri_string + 2;
      char const * const slash = std::strchr (username, '/');
      std::string const user (username,
                              slash == nullptr
                                ? std::strlen (username)
                                : static_cast<std::string::size_type> (slash - username));
      std::string home;

      if (user.empty ())
        {
          char const * const home_environment = ACE_OS::getenv ("HOME");
          if (home_environment == nullptr)
            return;
          home = home_environment;
        }
      else
        {
#if !defined (ACE_WIN32) && !defined (VXWORKS)
          char pw_buf[BUFSIZ] {};
          struct passwd pw_struct {};
          struct passwd *pw_result {};
          if (ACE_OS::getpwnam_r (user.c_str (), &pw_struct, pw_buf,
                                  sizeof pw_buf, &pw_result) != 0
              || pw_result == nullptr
              || pw_result->pw_dir == nullptr)
            return;
          home = pw_result->pw_dir;
#else
          return;
#endif /* NOT ACE_WIN32 AND NOT VXWORKS */
        }

      this->path_ = home + "/.www-docs";
      if (slash != nullptr)
        this->path_ += slash;
    }
  else
    this->path_ = std::string (".") + uri_string;
}
