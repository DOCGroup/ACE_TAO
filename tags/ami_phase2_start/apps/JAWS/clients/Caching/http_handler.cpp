// $Id$

// ============================================================================
//
// = LIBRARY
//    apps/JAWS/clients/Caching
//
// = FILENAME
//    http_handler.cpp
//
// = AUTHOR
//    James Hu
//
// ============================================================================

#include "ace/Filecache.h"
#include "http_handler.h"

ACE_RCSID(Caching, http_handler, "$Id$")

HTTP_Handler::HTTP_Handler (void)
{
}

HTTP_Handler::HTTP_Handler (const char * path)
{
  // How long is the request going to be?
  this->request_[0] = '\0';
  this->request_size_ =
    ACE_OS::strlen ("GET ")
    + ACE_OS::strlen (path)
    + ACE_OS::strlen (" HTTP/1.0\r\nAccept: HTTP/1.0\r\n\r\n");

  // Make the request.
  if ((u_int) this->request_size_ < sizeof (this->request_))
    ACE_OS::sprintf (this->request_,
		     "GET %s HTTP/1.0\r\nAccept: HTTP/1.0\r\n\r\n",
		     path);

  // Find the filename.
  const char *last = ACE_OS::strrchr (path, '/');

  if (last == 0)
    last = path;
  else if (last[1] == '\0')
    last = "index.html";
  else
    last = last+1;

  ACE_OS::sprintf (this->filename_, "%s", last);
}

int
HTTP_Handler::open (void *)
{
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
HTTP_Handler::svc (void)
{
  static char buf[BUFSIZ];
  int count = 0;

  ACE_DEBUG ((LM_DEBUG, "[%t] sending request --\n%s", this->request_));

  this->peer ().send_n (this->request_, this->request_size_);

  // Read in characters until encounter \r\n\r\n
  int done = 0;
  char *contentlength;

  do
    {
      while (((count += this->peer ().recv_n (buf + count, 1)) > 0)
             && ((u_int) count < sizeof (buf)))
        {
          buf[count] = '\0';

          if (count < 2)
	    continue;
          done = ACE_OS::strcmp (buf + count - 4, "\n\n") == 0;

          if (done)
	    break;

          if (count < 4)
	    continue;

          done = ACE_OS::strcmp (buf + count - 4, "\r\n\r\n") == 0;

          if (done)
	    break;
        }

      if (!done)
        {
          char *last = ACE_OS::strrchr (buf, '\n');
          last[0] = '\0';

          if ((contentlength = ACE_OS::strstr (buf, "\nContent-length:"))
              || (contentlength = ACE_OS::strstr (buf, "\nContent-Length:")))
            done = 1;
          else
            {
              last[0] = '\n';
              count = ACE_OS::strlen (last);
              ACE_OS::memmove (buf, last, count + 1);
            }
        }
      else
        {
	  contentlength = ACE_OS::strstr (buf, "\nContent-length:");

          if (!contentlength)
            contentlength =
	      ACE_OS::strstr (buf, "\nContent-Length:");
        }

    }
  while (!done);

  // ASSERT (contentlength != 0)
  if (contentlength
      && (::sscanf (contentlength, "\nContent-%*[lL]ength: %d ",
                    &this->response_size_) == 1))
    {
      ACE_Filecache_Handle afh (this->filename_,
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

const char *
HTTP_Handler::filename (void) const
{
  return this->filename_;
}

int
HTTP_Connector::connect (const char * url)
{
  char host[BUFSIZ];
  u_short port;
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
  if (ACE_Filecache::instance ()->find (hh.filename ()) == 0)
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
HTTP_Connector::parseurl (const char *url,
			  char *host,
			  u_short *port,
			  char *path)
{
  int status = 0;

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
		  ACE_OS::strcpy (path, "/");
		}
	    }
	  else
	    *port = DEFAULT_SERVER_PORT;
	}
      else ACE_OS::strcpy (path, "/");
    }

  // 0 => success
  // -1 => error
  return status;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<HTTP_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<HTTP_Handler>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<HTTP_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<HTTP_Handler>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<HTTP_Handler>*, ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
