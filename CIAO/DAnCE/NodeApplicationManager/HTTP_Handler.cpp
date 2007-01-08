// $Id$

// HTTP_Handler.cpp, Stoyan

#include "HTTP_Handler.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"

// Empty constructor for compliance with new Connector behavior.
HTTP_Handler::HTTP_Handler (void)
{
}

// Always use this constructor
HTTP_Handler::HTTP_Handler (ACE_Message_Block * mb,
                            ACE_TCHAR *filename) :
  mb_ (mb),
  filename_ (ACE_OS::strdup (filename)),
  bytecount_ (0)
{
}

HTTP_Handler::~HTTP_Handler (void)
{
  if (filename_)
    {
      ACE_OS::free (filename_);
      filename_ = 0;
    }
}

// Called by Connector after connection is established
int
HTTP_Handler::open (void *)
{
  if (this->send_request () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Handler::open():send_request failed\n"), -1);

  if (this->receive_reply () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Handler::open():receive_reply failed\n"), -1);
  return 0;

}

// No-op
int
HTTP_Handler::close (u_long)
{
  return 0;
}

// Always overridden by the derived classes
int
HTTP_Handler::send_request (void)
{
  return -1;
}

// Always overridden by the derived classes
int
HTTP_Handler::receive_reply (void)
{
  return -1;
}

// used to retrieve the number of bytes read/written by the
// last operation on the Blob
size_t
HTTP_Handler::byte_count (void)
{
  return bytecount_;
}

// Reader **************************************************

HTTP_Reader::HTTP_Reader (ACE_Message_Block * mb,
                          ACE_TCHAR *filename,
                          const char *request_prefix,
                          const char *request_suffix) :
  HTTP_Handler (mb, filename),
  request_prefix_ (request_prefix),
  request_suffix_ (request_suffix)
{
}

// Send the HTTP request
int
HTTP_Reader::send_request (void)
{
  char mesg [MAX_HEADER_SIZE];

  // Check to see if the request is too big
  if (MAX_HEADER_SIZE < (ACE_OS::strlen (request_prefix_)
                         + ACE_OS::strlen (filename_)
                         + ACE_OS::strlen (request_suffix_) + 4))
    ACE_ERROR_RETURN((LM_ERROR,"Request too large!"), -1);

  // Create a message to send to the server requesting retrieval of the file
  int const len =
    ACE_OS::sprintf (mesg, "%s %s %s", request_prefix_, filename_, request_suffix_);

  // Send the message to server
  if (peer ().send_n (mesg, len) != len)
    ACE_ERROR_RETURN((LM_ERROR,"Error sending request\n"), -1);


  return 0;
}

// Recieve the HTTP Reply
int
HTTP_Reader::receive_reply (void)
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
                            "HTTP_Reader::receiveReply(): Response is not 200 OK\n" ), -1);

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
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Reader::receiveReply():Error while reading header\n"), -1);

  // ***************************************************************
  // At this point, we have stripped off the header and are ready to
  // process data. buf_ptr points to the data

  //set the size of the ACE_Message_Block to the current bytes read
  //NOTE: this function reallocates if necessary
  //this->mb_->size (bytes_read);

  ACE_Message_Block* temp  = 0;   //pointer used temporarily
                             //for memory allocations before
                             //chaining to Message_Block

  ACE_Message_Block* curr = this->mb_;

  ACE_NEW_RETURN (temp, ACE_Message_Block (bytes_read), -1);
  curr->cont (temp);

  curr = curr->cont ();

  // Copy over all the data bytes into our message buffer.
  if (curr->copy (buf_ptr, bytes_read) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n",
                          "HTTP_Reader::receiveReply():Error copying data into Message_Block\n" ), -1);

  //read the rest of the data into a number of ACE_Message_Blocks and
  //chain them together in a link list fashion

  num_recvd = 0;

  do
  {
    if (curr->space () == 0)
    {
      ACE_NEW_RETURN (temp, ACE_Message_Block (MTU), -1);
      curr->cont (temp);
      curr = curr->cont ();
    }

  if (peer ().recv_n (curr->wr_ptr (), curr->space (), 0, &num_recvd) >= 0)
    {
      //move the write pointer
      curr->wr_ptr (num_recvd);

      //increment bytes_read
      bytes_read += num_recvd;

    }
  else
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Reader::receiveReply():Error while reading header\n"), -1);

  }while (num_recvd != 0);

  // Set the byte count to number of bytes received
  this->bytecount_ = bytes_read;

  return 0;
}

