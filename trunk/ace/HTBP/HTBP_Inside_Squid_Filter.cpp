// ACE_HTBP_Filter.cpp
// $Id$

#include "ace/Log_Msg.h"

//#include <sstream>

#include "HTBP_Session.h"
#include "HTBP_Inside_Squid_Filter.h"

#if !defined (__ACE_INLINE__)
#include "HTBP_Inside_Squid_Filter.inl"
#endif

ACE_RCSID(HTBP,ACE_HTBP_Inside_Squid_Filter,"$ID:$")

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
  const char *format[] = {"http://",
                          "/",
                          "/",
                          "/request",
                          ".html HTTP/1.1\n",
                          0};
  char *buf = buffer;
  size_t space = buffer_size;
  ACE::HTBP::Session *session = ch->session();
  for (int i = 0; format[i >> 1]; i++)
    {
      char addr_buff[BUFSIZ];
      const char *token = 0;
      size_t toklen = 0;
      if ( (( i >> 1) << 1) != i)
        {
          token = format [i >> 1];
          toklen = ACE_OS::strlen (token);
        }
      else
        {
          token = addr_buff;
          switch (i) {
          case 0:
            {
              token = cmd;
              toklen = ACE_OS::strlen (token);
              break;
            }
          case 2:
            {
              if (session->peer_addr().addr_to_string(addr_buff,
                                                      sizeof addr_buff) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "could not convert peer_addr to string\n"),
                                  -1);
              toklen = ACE_OS::strlen (token);
              break;
            }
          case 4:
            {
              if (session->local_addr().addr_to_string(addr_buff,
                                                       sizeof addr_buff) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "could not convert peer_addr to string\n"),
                                  -1);
              toklen = ACE_OS::strlen (token);
              if (toklen && addr_buff[toklen-1] == '\n')
                toklen --;
              break;
            }
          case 6:
            {
              toklen = ACE_OS::strlen (ACE_OS::itoa(session->session_id().id_,
                                                    addr_buff,
                                                    10));
              break;
            }
          case 8:
            {
              toklen = ACE_OS::strlen (ACE_OS::itoa(ch->request_count(),
                                                    addr_buff,
                                                    10));
              break;
            }
          case 10:
            break;
          default:
            {
              ACE_DEBUG ((LM_DEBUG,
                          "ACE::HTBP::Inside_Squid_Filter::make_request_header: "
                          "Oops, switch hit with i = %d\n",i));
            }
          }
        }
      if (toklen > space)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "request header buffer too small "
                           "segment %d, need %d, have %d of total %d\n",
                           i,toklen, space, buffer_size),
                          -1);
      ACE_OS::strncpy (buf,token,space);
      buf += toklen;
      space -= toklen;
      *buf = '\0';
    }
  return ACE_OS::strlen(buffer);
}

ssize_t
ACE::HTBP::Inside_Squid_Filter::send_data_header (ssize_t data_len,
                                                ACE::HTBP::Channel *ch)
{
  char *buffer = new char[BUFSIZ];
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
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Inside_Squid_Filter::recv_data_header, ")
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
                         ACE_TEXT("Inside_Squid_Filter::recv_data_header, ")
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
      ch->data_len (strtol(tpos,0,10));
      start = nl+1;
    }

  ch->leftovers().rd_ptr(header_end);
  if (this->http_code() != 200)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Inside_Squid_Filter::recv_data_header, ")
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
  ssize_t result = -1;
  if (ch->state() == ACE::HTBP::Channel::Ack_Sent)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("Inside Filter::send_ack: ")
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
                         ACE_TEXT("Inside_Squid_Filter::recv_data_header, ")
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
      ch->data_len (strtol(tpos,0,10));
      start = nl+1;
    }

  ch->leftovers().rd_ptr(header_end);
  errno = ch->consume_error () ? EINVAL : EWOULDBLOCK;
  return 0;
}
