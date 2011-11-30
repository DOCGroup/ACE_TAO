// $Id$

#include "tao/HTTP_Handler.h"

#if (TAO_HAS_HTTP_PARSER == 1)

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_HTTP_Handler::TAO_HTTP_Handler (void)
{
}

TAO_HTTP_Handler::TAO_HTTP_Handler (ACE_Message_Block * mb,
                            ACE_TCHAR *filename) :
  mb_ (mb),
  filename_ (ACE_OS::strdup (filename)),
  bytecount_ (0)
{
}

TAO_HTTP_Handler::~TAO_HTTP_Handler (void)
{
  if (this->filename_)
    {
      ACE_OS::free (this->filename_);
      filename_ = 0;
    }
}

int
TAO_HTTP_Handler::open (void *)
{
  if (this->send_request () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "TAO (%P|%t) - HTTP_Handler::open, send_request failed\n"), -1);

  if (this->receive_reply () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "TAO (%P|%t) - HTTP_Handler::open, receive_reply failed\n"), -1);
  return 0;

}

int
TAO_HTTP_Handler::close (u_long)
{
  return 0;
}

int
TAO_HTTP_Handler::send_request (void)
{
  return -1;
}

int
TAO_HTTP_Handler::receive_reply (void)
{
  return -1;
}

size_t
TAO_HTTP_Handler::byte_count (void) const
{
  return bytecount_;
}

// Reader **************************************************

TAO_HTTP_Reader::TAO_HTTP_Reader (ACE_Message_Block * mb,
                                  ACE_TCHAR *filename,
                                  const char *request_prefix,
                                  const char *request_suffix) :
  TAO_HTTP_Handler (mb, filename),
  request_prefix_ (request_prefix),
  request_suffix_ (request_suffix)
{
}

int
TAO_HTTP_Reader::send_request (void)
{
  char mesg [MAX_HEADER_SIZE];

  // Check to see if the request is too big
  if (MAX_HEADER_SIZE < (ACE_OS::strlen (request_prefix_)
                         + ACE_OS::strlen (filename_)
                         + ACE_OS::strlen (request_suffix_) + 4))
    ACE_ERROR_RETURN((LM_ERROR,"TAO (%P|%t) - HTTP_Reader::send_request, request too large!"), -1);

  // Create a message to send to the server requesting retrieval of the file
  int const len = ACE_OS::sprintf (mesg, "%s %s %s", request_prefix_,
                                   ACE_TEXT_ALWAYS_CHAR (filename_),
                                   request_suffix_);

  // Send the message to server
  if (peer ().send_n (mesg, len) != len)
    ACE_ERROR_RETURN((LM_ERROR,"TAO (%P|%t) - HTTP_Reader::send_request, error sending request\n"), -1);

  return 0;
}

int
TAO_HTTP_Reader::receive_reply (void)
{
  size_t num_recvd = 0;
  char buf [MTU+1];
  char *buf_ptr = 0;
  size_t bytes_read = 0;

  // Receive the first MTU bytes and strip the header off.
  // Note that we assume that the header will fit into MTU bytes.
  if (peer ().recv_n (buf, MTU, 0, &num_recvd) >= 0)
    {
      //Make sure that response type is 200 OK
      if (ACE_OS::strstr (buf,"200 OK") == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                            "TAO (%P|%t) - HTTP_Reader::receive_reply, Response is not 200 OK\n" ), -1);

      // Search for the header termination string "\r\n\r\n", or "\n\n". If
      // found, move past it to get to the data portion.
      if ((buf_ptr = ACE_OS::strstr (buf,"\r\n\r\n")) != 0)
        buf_ptr += 4;
      else if ((buf_ptr = ACE_OS::strstr (buf, "\n\n")) != 0)     //for compatibility with JAWS
        buf_ptr += 2;
      else
        buf_ptr = buf;

      // Determine number of data bytes read. This is equal to the
      // total bytes read minus number of header bytes.
      bytes_read = num_recvd - (buf_ptr - buf);

    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) - HTTP_Reader::receive_reply, error while reading header\n"), -1);
    }

  // ***************************************************************
  // At this point, we have stripped off the header and are ready to
  // process data. buf_ptr points to the data

  ACE_Message_Block* temp = 0;
  ACE_Message_Block* curr = this->mb_;

  ACE_NEW_RETURN (temp,
                  ACE_Message_Block (bytes_read),
                  -1);
  curr->cont (temp);
  curr = curr->cont ();

  // Copy over all the data bytes into our message buffer.
  if (curr->copy (buf_ptr, bytes_read) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "TAO (%P|%t) - HTTP_Reader::receive_reply, error copying data into Message_Block\n"), -1);
    }

  // read the rest of the data into a number of ACE_Message_Blocks and
  // chain them together in a link list fashion
  num_recvd = 0;

  do
  {
    if (curr->space () == 0)
    {
      ACE_NEW_RETURN (temp,
                      ACE_Message_Block (MTU),
                      -1);
      curr->cont (temp);
      curr = curr->cont ();
    }

  if (peer ().recv_n (curr->wr_ptr (), curr->space (), 0, &num_recvd) >= 0)
    {
      // Move the write pointer
      curr->wr_ptr (num_recvd);

      // Increment bytes_read
      bytes_read += num_recvd;

    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) - HTTP_Reader::receive_reply, Error while reading header\n"), -1);
    }
  } while (num_recvd != 0);

  // Set the byte count to number of bytes received
  this->bytecount_ = bytes_read;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_HTTP_PARSER == 1*/

