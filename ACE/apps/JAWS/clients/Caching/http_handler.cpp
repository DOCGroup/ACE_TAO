
//=============================================================================
/**
 *  @file    http_handler.cpp
 *
 *  @author James Hu
 */
//=============================================================================

#include "ace/Filecache.h"
#include "http_handler.h"

#include <charconv>
#include <cstring>
#include <limits>
#include <memory>

HTTP_Handler::HTTP_Handler (std::string const &path)
{
  this->request_ = "GET " + path + " HTTP/1.0\r\nAccept: HTTP/1.0\r\n\r\n";

  // Find the filename.
  size_t const last_slash = path.rfind ('/');
  if (last_slash == std::string::npos)
    {
      this->filename_ = path;
    }
  else if (last_slash == path.size () - 1)
    {
      this->filename_ = "index.html";
    }
  else
    {
      this->filename_ = path.substr (last_slash + 1);
    }
}

int
HTTP_Handler::open (void *)
{
  // If you want threads, use the activate stuff.
#if 0
  if (this->activate () != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("HTTP_Handler::open, whups!\n")),
                        -1);
    }

  return 0;
#else
  return this->svc ();
#endif /* 0 */
}

int
HTTP_Handler::svc ()
{
  char buf[BUFSIZ] {};
  int count {};

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("[%t] sending request --\n%C"),
              this->request_.c_str ()));

  if (this->peer ().send_n (this->request_.c_str (), this->request_.size ())
      != static_cast<ssize_t> (this->request_.size ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Failed to send HTTP request\n")), -1);
    }

  // Read in characters until encounter \r\n\r\n
  bool done {};
  char const *contentlength {};

  do
    {
      while (((count += this->peer ().recv_n (buf + count, 1)) > 0)
             && (static_cast<size_t> (count) < sizeof (buf) - 1))
        {
          buf[count] = '\0';

          if (count >= 2)
            {
              done = std::strcmp (buf + count - 2, "\n\n") == 0;
            }

          if (done)
            {
              break;
            }

          if (count < 4)
            {
              continue;
            }

          done = std::strcmp (buf + count - 4, "\r\n\r\n") == 0;

          if (done)
            {
              break;
            }
        }

      if (!done)
        {
          char *last = std::strrchr (buf, '\n');
          if (last == nullptr)
            {
              ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Malformed HTTP response header\n")), -1);
            }
          last[0] = '\0';

          contentlength = std::strstr (buf, "\nContent-length:");
          if (contentlength == nullptr)
            {
              contentlength = std::strstr (buf, "\nContent-Length:");
            }

          if (contentlength != nullptr)
            {
              done = true;
            }
          else
            {
              last[0] = '\n';
              count = std::strlen (last);
              std::memmove (buf, last, count + 1);
            }
        }
      else
        {
          contentlength = std::strstr (buf, "\nContent-length:");

          if (contentlength == nullptr)
            {
              contentlength =
                std::strstr (buf, "\nContent-Length:");
            }
        }

    }
  while (!done);

  size_t response_size {};
  bool has_content_length {};
  if (contentlength != nullptr)
    {
      char const *value = std::strchr (contentlength, ':');
      if (value != nullptr)
        {
          ++value;
          while (*value == ' ' || *value == '\t')
            {
              ++value;
            }

          char const *value_end = std::strpbrk (value, "\r\n");
          if (value_end == nullptr)
            {
              value_end = value + std::strlen (value);
            }

          std::from_chars_result const parsed =
            std::from_chars (value, value_end, response_size);
          if (parsed.ec == std::errc {} && parsed.ptr != value)
            {
              char const *trailing = parsed.ptr;
              while (trailing != value_end &&
                     (*trailing == ' ' || *trailing == '\t'))
                {
                  ++trailing;
                }
              has_content_length = trailing == value_end;
            }
        }

      if (!has_content_length)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Invalid HTTP Content-Length header\n")),
                            -1);
        }

      if (response_size > static_cast<size_t> ((std::numeric_limits<ssize_t>::max) ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("HTTP response body is too large\n")),
                            -1);
        }

      this->response_size_ = response_size;
      ACE_Filecache_Handle afh (ACE_TEXT_CHAR_TO_TCHAR (this->filename_.c_str ()),
                                this->response_size_);

      if (this->peer ().recv_n (afh.address (), this->response_size_)
          != static_cast<ssize_t> (this->response_size_))
        {
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Failed to receive HTTP response body\n")), -1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("  ``%C'' is now cached.\n"),
                  this->filename_.c_str ()));
    }
  else
    {
      // Maybe we should do something more clever here, such as extend
      // ACE_Filecache_Handle to allow the creation of cache objects
      // whose size is unknown?

      // Another possibility is to write the contents out to a file,
      // and then cache it.

      // Perhaps make ACE_Filecache_Handle more savvy, and allow a
      // constructor which accepts a PEER as a parameter.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("HTTP_Handler, no content-length header!\n")));
    }

  return 0;
}

std::string const &
HTTP_Handler::filename () const
{
  return this->filename_;
}

int
HTTP_Connector::connect (std::string const &url)
{
  u_short port {};
  std::string host;
  std::string path;

  if (this->parseurl (url, host, port, path) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("HTTP_Connector, error parsing url: %C\n"),
                  url.c_str ()));
      return -1;
    }

  HTTP_Handler hh (path);
  HTTP_Handler *hhptr = std::addressof (hh);

  // First check the cache.
  if (ACE_Filecache::instance ()->find (ACE_TEXT_CHAR_TO_TCHAR (hh.filename ().c_str ())) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  ``%C'' is already cached.\n"),
                  hh.filename ().c_str ()));
      return 0;
    }

  return this->connector_.connect (hhptr, ACE_INET_Addr (port, host.c_str ()));
}

#define DEFAULT_SERVER_PORT 80

// extract the main components of a URL
int
HTTP_Connector::parseurl (std::string const &url,
                          std::string &host,
                          u_short &port,
                          std::string &path)
{
  constexpr char scheme[] = "http://";
  constexpr size_t scheme_length = sizeof (scheme) - 1;
  if (url.compare (0, scheme_length, scheme) != 0)
    {
      return -1;
    }

  size_t const authority_start = scheme_length;
  size_t const path_start = url.find_first_of ("/?", authority_start);
  std::string const authority = url.substr (authority_start,
                                            path_start - authority_start);
  size_t const port_separator = authority.find (':');
  host = authority.substr (0, port_separator);
  if (host.empty ())
    {
      return -1;
    }

  port = DEFAULT_SERVER_PORT;
  if (port_separator != std::string::npos)
    {
      std::string const port_text = authority.substr (port_separator + 1);
      unsigned int parsed_port {};
      auto const result = std::from_chars (port_text.data (),
                                           port_text.data () + port_text.size (),
                                           parsed_port);
      if (port_text.empty ()
          || result.ec != std::errc {}
          || result.ptr != port_text.data () + port_text.size ()
          || parsed_port == 0
          || parsed_port > (std::numeric_limits<u_short>::max) ())
        {
          return -1;
        }
      port = static_cast<u_short> (parsed_port);
    }

  if (path_start == std::string::npos)
    {
      path = "/";
    }
  else
    {
      path = url.substr (path_start);
      if (url[path_start] == '?')
        {
          path.insert (path.begin (), '/');
        }
    }

  return 0;
}

