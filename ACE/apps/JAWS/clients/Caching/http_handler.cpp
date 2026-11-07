
//=============================================================================
/**
 *  @file    http_handler.cpp
 *
 *  @author James Hu
 */
//=============================================================================


#include "ace/Filecache.h"
#include "http_handler.h"
#include <cstdio>
#include <cstring>

HTTP_Handler::HTTP_Handler (char const *path)
{
  // How long is the request going to be?
  this->request_[0] = '\0';
  this->request_size_ =
    std::strlen ("GET ")
    + std::strlen (path)
    + std::strlen (" HTTP/1.0\r\nAccept: HTTP/1.0\r\n\r\n");

  // Make the request.
  if (this->request_size_ < sizeof (this->request_))
    std::sprintf (this->request_,
                  "GET %s HTTP/1.0\r\nAccept: HTTP/1.0\r\n\r\n",
                  path);
  else
    this->request_size_ = 0;

  // Find the filename.
  char const *last = std::strrchr (path, '/');

  if (last == nullptr)
    last = path;
  else if (last[1] == '\0')
    last = "index.html";
  else
    last = last + 1;

  std::sprintf (this->filename_, "%s", last);
}

int
HTTP_Handler::open (void *)
{
  if (this->request_size_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "HTTP request exceeds buffer size\n"), -1);

  // If you want threads, use the activate stuff.
#if 0
  if (this->activate () != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "HTTP_Handler::open, whups!\n"), -1);
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

  ACE_DEBUG ((LM_DEBUG, "[%t] sending request --\n%s", this->request_));

  this->peer ().send_n (this->request_, this->request_size_);

  // Read in characters until encounter \r\n\r\n
  bool done {};
  char *contentlength {};

  do
    {
      while (((count += this->peer ().recv_n (buf + count, 1)) > 0)
             && (static_cast<size_t> (count) < sizeof (buf) - 1))
        {
          buf[count] = '\0';

          if (count >= 2)
            done = std::strcmp (buf + count - 2, "\n\n") == 0;

          if (done)
            break;

          if (count < 4)
            continue;

          done = std::strcmp (buf + count - 4, "\r\n\r\n") == 0;

          if (done)
            break;
        }

      if (!done)
        {
          char *last = std::strrchr (buf, '\n');
          if (last == nullptr)
            ACE_ERROR_RETURN ((LM_ERROR, "Malformed HTTP response header\n"), -1);
          last[0] = '\0';

          if ((contentlength = std::strstr (buf, "\nContent-length:"))
              || (contentlength = std::strstr (buf, "\nContent-Length:")))
            done = true;
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

          if (!contentlength)
            contentlength =
              std::strstr (buf, "\nContent-Length:");
        }

    }
  while (!done);

  // ASSERT (contentlength != 0)
  int size {};
  if (contentlength
      && (::sscanf (contentlength, "\nContent-%*[lL]ength: %d ",
                    &size) == 1))
    {
      this->response_size_ = size;
      ACE_Filecache_Handle afh (ACE_TEXT_CHAR_TO_TCHAR (this->filename_),
                                this->response_size_);

      this->peer ().recv_n (afh.address (), this->response_size_);

      ACE_DEBUG ((LM_DEBUG,
                  "  ``%s'' is now cached.\n",
                  this->filename_));
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
                  "HTTP_Handler, no content-length header!\n"));
    }

  return 0;
}

char const *
HTTP_Handler::filename () const
{
  return this->filename_;
}

int
HTTP_Connector::connect (char const *url)
{
  char host[BUFSIZ];
  u_short port {};
  char path[BUFSIZ];

  if (this->parseurl (url, host, &port, path) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "HTTP_Connector, error parsing url: %s\n",
                  url));
      return -1;
    }

  HTTP_Handler hh (path);
  HTTP_Handler *hhptr = &hh;

  // First check the cache.
  if (ACE_Filecache::instance ()->find (ACE_TEXT_CHAR_TO_TCHAR (hh.filename ())) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "  ``%s'' is already cached.\n",
                  hh.filename ()));
      return 0;
    }

  return this->connector_.connect (hhptr, ACE_INET_Addr (port, host));
}

#define DEFAULT_SERVER_PORT 80

// extract the main components of a URL
int
HTTP_Connector::parseurl (char const *url,
                          char *host,
                          u_short *port,
                          char *path)
{
  int status {};

  // hackish, but useful
  if (3 != ::sscanf (url, "http://%[^:/]:%hu%s", host, port, path))
    {
      if (2 != ::sscanf (url, "http://%[^:/]:%hu", host, port))
        {
          if (2 != ::sscanf (url, "http://%[^:/]%s", host, path))
            {
              if (1 != ::sscanf (url, "http://%[^:/]", host))
                status = -1;
              else
                {
                  *port = DEFAULT_SERVER_PORT;
                  std::strcpy (path, "/");
                }
            }
          else
            *port = DEFAULT_SERVER_PORT;
        }
      else std::strcpy (path, "/");
    }

  // 0 => success
  // -1 => error
  return status;
}

