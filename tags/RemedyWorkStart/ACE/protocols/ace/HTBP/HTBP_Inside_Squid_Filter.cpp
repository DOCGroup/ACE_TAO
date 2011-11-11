// ACE_HTBP_Filter.cpp
// $Id$

#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "HTBP_Session.h"
#include "HTBP_Inside_Squid_Filter.h"

#if !defined (__ACE_INLINE__)
#include "HTBP_Inside_Squid_Filter.inl"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ssize_t
ACE::HTBP::Inside_Squid_Filter::send_data_trailer (ACE::HTBP::Channel *ch)
{
  ch->state(ACE::HTBP::Channel::Wait_For_Ack);
  return 0;
}

int
ACE::HTBP::Inside_Squid_Filter::make_request_header (ACE::HTBP::Channel *ch,
                                                     const char *cmd,
                                                     char *buffer,
                                                     size_t buffer_size)
{
  // the string is formatted as:
  // command  http://host:port/htid/sessionId/request<requestId>.html HTTP/1.1\n
  // host:port is the remote host and port from the channel,
  // htid is the local endpoint identifier
  // sessionId is the discreet session counter between these peers
  // requestId is the discreet request sent for this session.

  ACE::HTBP::Session *session = ch->session();

  const char * format = "%shttp://%s:%d/%s/%d/request%d.html HTTP/1.1\n";
  char remote_host[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  unsigned remote_port = session->peer_addr().get_port_number();
  const char *local_htid = session->local_addr().get_htid();

  ACE_UINT32 tempId = session->session_id().id_;
  size_t sid_size = 1;
  size_t rid_size = 1;
  while (tempId /= 10) sid_size++;
  tempId = ch->request_count();
  while (tempId /= 10) rid_size++;

  // This test was originally get_host_name() == -1, but this is
  // problematic if the address doesn't resolve to a name. I think
  // that it should be configurable, or maybe the hostname needs to
  // be carried independent of the address to work with hosts that may
  // have dynamic IP addresses. For now that isn't a problem.
  if (session->peer_addr().get_host_addr(remote_host,
                                         sizeof remote_host) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("HTBP::Inside_Squid_Filter:could not get ")
                       ACE_TEXT("peer_addr hostname\n")),
                      -1);


  size_t size =
    ACE_OS::strlen(format)
    - 12    // subtract out the size of the format specifiers
    + ACE_OS::strlen (cmd)
    + ACE_OS::strlen (remote_host)
    + 5     // maximum size for a the remote port number
    + ACE_OS::strlen (local_htid)
    + sid_size // size of session id
    + rid_size; // size of request id

  if (size > buffer_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("HTBP::Inside_Squid_Filter: insufficient ")
                       ACE_TEXT("buffer space for request header, need %d ")
                       ACE_TEXT("got %d\n"),
                       size, buffer_size),
                      -1);

  ACE_OS::sprintf (buffer,format,
                   cmd,remote_host,remote_port,
                   local_htid, session->session_id().id_,
                   ch->request_count());

  return ACE_OS::strlen(buffer);
}

ssize_t
ACE::HTBP::Inside_Squid_Filter::send_data_header (ssize_t data_len,
                                                ACE::HTBP::Channel *ch)
{
  char *buffer = new char[BUFSIZ];
  ACE_Auto_Array_Ptr<char> guard (buffer);
  ssize_t result = -1;
  if (this->make_request_header (ch,"POST ",buffer,BUFSIZ) != -1)
    {
      ACE_CString header (buffer);
      header += "Content-Type: application/octet-stream\nContent-Length: ";
      char datalenstr[20];
      ACE_OS::itoa (data_len,datalenstr,10);
      header += datalenstr;
      header += "\n\n";
      result = ch->ace_stream().send(header.c_str(),header.length());
    }
  ch->state(result == -1 ? ACE::HTBP::Channel::Closed :
            ACE::HTBP::Channel::Header_Sent);
  this->reset_http_code();
  return 1;
}

ssize_t
ACE::HTBP::Inside_Squid_Filter::recv_data_header (ACE::HTBP::Channel *ch)
{
  if (this->http_code() != 200 && this->http_code() != 0)
    {
      if (ACE::debug())
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("HTBP::Inside_Squid_Filter::recv_data_header, ")
                    ACE_TEXT("non-OK result code %d recvd\n"),
                    this->http_code()));

      errno = ch->consume_error () ? EINVAL : EWOULDBLOCK;
      return 0;
    }
  char *header_end = this->header_complete(ch);
  if (header_end == 0)
    {
      if (ch->state() != ACE::HTBP::Channel::Closed)
        {
          ch->state (ACE::HTBP::Channel::Header_Pending);
          errno = EWOULDBLOCK;
        }
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("HTBP::Inside_Squid_Filter::")
                         ACE_TEXT("recv_data_header, ")
                         ACE_TEXT("header not complete\n")),
                        0);
    }
  char *start = ch->leftovers().rd_ptr();
  ACE_CString token = "Content-Length: ";
  char *tpos = ACE_OS::strstr(start,token.c_str());
  char *nl = ACE_OS::strchr (start,'\n');
  if (tpos != 0)
    {
      tpos += token.length();
      *nl = 0;
      ch->data_len (ACE_OS::strtol(tpos,0,10));
      start = nl+1;
    }

  ch->leftovers().rd_ptr(header_end);
  if (this->http_code() != 200)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("HTBP::Inside_Squid_Filter::recv_data_header, ")
                  ACE_TEXT("non-OK result code %d recvd\n"),
                  this->http_code()));

      errno = ch->consume_error () ? EINVAL : EWOULDBLOCK;
      return 0;
    }

  ch->state(ACE::HTBP::Channel::Data_Queued);
  return 1;
}

ssize_t
ACE::HTBP::Inside_Squid_Filter::recv_data_trailer (ACE::HTBP::Channel *ch)
{
  ch->state(ACE::HTBP::Channel::Send_Ack);
  return 0;
}

int
ACE::HTBP::Inside_Squid_Filter::send_ack (ACE::HTBP::Channel *ch)
{
  char *buffer = new char[BUFSIZ];
  ACE_Auto_Array_Ptr<char> guard (buffer);
  ssize_t result = -1;
  if (ch->state() == ACE::HTBP::Channel::Ack_Sent)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("HTBP::Inside Filter::send_ack: ")
                         ACE_TEXT("state is already ACK_SENT\n")),
                        1);
    }
  if (this->make_request_header (ch,"GET ",buffer,BUFSIZ) != -1)
    {
      ACE_CString header (buffer);
      header += "\n";
      result = ch->ace_stream().send(header.c_str(),header.length());
    }
  ch->state(result == -1 ?
            ACE::HTBP::Channel::Closed : ACE::HTBP::Channel::Ack_Sent);
  this->reset_http_code();
  return 1;
}

int
ACE::HTBP::Inside_Squid_Filter::recv_ack (ACE::HTBP::Channel *ch)
{

  char *header_end = this->header_complete(ch);
  if (header_end == 0)
    {
      if (ch->state() != ACE::HTBP::Channel::Closed)
        errno = EWOULDBLOCK;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("HTBP::Inside_Squid_Filter::")
                         ACE_TEXT("recv_ack, ")
                         ACE_TEXT("header not complete\n")),0);
    }
  if (this->http_code() == 200)
    {
      ch->leftovers().length(0);
      ch->state(ACE::HTBP::Channel::Ready);
      return 1;
    }

  char *start = ch->leftovers().rd_ptr();
  ACE_CString token = "Content-Length: ";
  char *tpos = ACE_OS::strstr(start,token.c_str());
  char *nl = ACE_OS::strchr (start,'\n');
  if (tpos != 0)
    {
      tpos += token.length();
      *nl = 0;
      ch->data_len (ACE_OS::strtol(tpos,0,10));
      start = nl+1;
    }

  ch->leftovers().rd_ptr(header_end);
  errno = ch->consume_error () ? EINVAL : EWOULDBLOCK;
  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
