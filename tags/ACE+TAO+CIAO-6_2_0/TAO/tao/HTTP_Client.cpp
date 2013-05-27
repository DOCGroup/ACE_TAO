// $Id$

#include "tao/HTTP_Client.h"

#if (TAO_HAS_HTTP_PARSER == 1)

#include "tao/HTTP_Handler.h"
#include "ace/OS_NS_string.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_HTTP_Client::TAO_HTTP_Client (void)
{
}

TAO_HTTP_Client::~TAO_HTTP_Client (void)
{
  this->close ();
}

int
TAO_HTTP_Client::open (const ACE_TCHAR *filename,
                       const ACE_TCHAR *hostname,
                       u_short port)
{
  this->filename_ = ACE_OS::strdup (filename);
  this->inet_addr_.set (port, hostname);
  return 0;
}

int
TAO_HTTP_Client::read (ACE_Message_Block *mb)
{
  // Create a HTTP_Client Reader
  TAO_HTTP_Reader HTTP_reader (mb, this->filename_);
  TAO_HTTP_Handler *brp = &HTTP_reader;

  // Connect to the server
  if (connector_.connect (brp, this->inet_addr_) == -1)
    {
      TAOLIB_ERROR_RETURN ((LM_ERROR, "TAO (%P|%t) - HTTP_Client::read, Connector error\n"), -1);
    }

  return HTTP_reader.byte_count ();

}

int
TAO_HTTP_Client::close (void)
{
  if (this->filename_)
    {
      ACE_OS::free ((void *) this->filename_);
      this->filename_ = 0;
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_HTTP_PARSER == 1 */
