// $Id$

// HTTP_Client.cpp, Stoyan

#include "HTTP_Client.h"
#include "HTTP_Handler.h"
#include "ace/OS_NS_string.h"

HTTP_Client::HTTP_Client (void)
{
}

HTTP_Client::~HTTP_Client (void)
{
  this->close ();
}

// initialize address and filename. No network i/o in open
int
HTTP_Client::open (const ACE_TCHAR *filename,
                   const ACE_TCHAR *hostname,
                   u_short port)
{
  filename_ = ACE_OS::strdup (filename);
  inet_addr_.set (port, hostname);
  return 0;
}

// read from connection length bytes from offset, into Message block
int
HTTP_Client::read (ACE_Message_Block *mb)
{
  // Create a HTTP_Client Reader
  HTTP_Reader HTTP_reader (mb, filename_);
  HTTP_Handler *brp = &HTTP_reader;

  // Connect to the server
  if (connector_.connect (brp, inet_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Client::read():Connector error"), -1);

  return HTTP_reader.byte_count ();
}

// close down the HTTP_Client
int
HTTP_Client::close (void)
{
  if (filename_)
    {
      ACE_OS::free ((void *) filename_);
      filename_ = 0;
    }
  return 0;
}

