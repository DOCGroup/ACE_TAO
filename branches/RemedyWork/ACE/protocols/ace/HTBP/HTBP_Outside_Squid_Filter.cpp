// $Id$

#include "ace/Log_Msg.h"

#include "HTBP_Session.h"
#include "HTBP_Outside_Squid_Filter.h"

#if !defined (__ACE_INLINE__)
#include "HTBP_Outside_Squid_Filter.inl"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ssize_t
ACE::HTBP::Outside_Squid_Filter::recv_data_header (ACE::HTBP::Channel *ch)
{
  // on the outside of the firewall, this method must do the details
  // necessary to call replace_session or whatever to migrate the sock
  // stream to the appropriate ACE::HTBP::Channel, then to finally
  // assign the right filter to that stream. That filter will an
  // ACE::HTBP::Outside_[Send|Recv]_Filter.

  // in the case of an ACE::HTBP::Outside_Recv_Filter, the assigned
  // filter must be a null filter first, with a replacement.

  // recv header details
  char *header_end = this->header_complete(ch);
  if (header_end == 0)
    {
      if (ch->state() != ACE::HTBP::Channel::Closed)
        {
          ch->state(ACE::HTBP::Channel::Header_Pending);
          errno = EWOULDBLOCK;
        }
      return 0;
    }

  char *start = ch->leftovers().rd_ptr();

  int is_inbound = 0;
  ACE_CString token ("POST ");
  if (ACE_OS::strncmp (start,token.c_str(),token.length()) == 0)
    is_inbound = 1;
  else
    {
      token = "GET ";
      if (ACE_OS::strncmp (start,
                           token.c_str(),
                           token.length()) != 0)
        {
          ch->leftovers().length(0);
          errno = EINVAL;
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("ACE::HTBP::Outside_Squid_Filter::")
                             ACE_TEXT("recv_data_header ")
                             ACE_TEXT("bad request header\n")),0);
        }
    }
  start += token.length();
  // "http://" is stripped by squid, leaving only "/"
  start += (ACE_OS::strncmp (start,"http://",7) == 0) ? 7 : 1;

  // set up the actual session and stream
  ACE::HTBP::Session_Id_t session_id;
  char * slash = ACE_OS::strchr(start,'/');
  char * nl = ACE_OS::strchr (start,'\n');
  if (slash == 0)
    {
      ch->leftovers().length(0);
      errno = EINVAL;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE::HTBP::Outside_Squid_Filter::")
                         ACE_TEXT("recv_data_header ")
                         ACE_TEXT("missing sender key\n")),0);
    }
  *slash = 0;
  session_id.local_.string_to_addr (start);
  start = slash+1;

  slash =  ACE_OS::strchr(start,'/');
  if (slash == 0)
    {
      ch->leftovers().length(0);
      errno = EINVAL;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE::HTBP::Outside_Squid_Filter::")
                         ACE_TEXT("recv_data_header ")
                         ACE_TEXT("missing sender key\n")),0);
    }
  *slash = 0;
  session_id.peer_.string_to_addr (start);
  start = slash + 1;

  slash =  ACE_OS::strchr(start,' ');
  if (slash == 0)
    {
      ch->leftovers().length (0);
      errno = EINVAL;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE::HTBP::Outside_Squid_Filter::")
                         ACE_TEXT("recv_data_header ")
                         ACE_TEXT("missing sender key")),0);
    }
  *slash = 0;
  session_id.id_ = ACE_OS::strtol(start,0,10);
  start = slash + 1;

  if (is_inbound)
    {
      token = "Content-Length: ";
      char *tpos = ACE_OS::strstr(start,token.c_str());
      if (tpos != 0)
        {
          nl = ACE_OS::strchr(tpos,'\n');
          tpos += token.length();
          *nl = 0;
          ch->data_len(ACE_OS::strtol(tpos,0,10));
          start = nl+1;
        }
    }
  ch->leftovers().rd_ptr(header_end);

  ACE::HTBP::Session *session = 0;
  if (ACE::HTBP::Session::find_session (session_id, session) == -1)
    {
      ACE_NEW_RETURN (session, ACE::HTBP::Session (session_id), 0);
      if (ACE::HTBP::Session::add_session (session) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("ACE::HTBP::Outside_Squid_Filter::")
                           ACE_TEXT("recv_data_header %p"),
                           ACE_TEXT("add_session")),0);
    }
  ch->session(session);

  if (is_inbound)
    {
      ch->state(ACE::HTBP::Channel::Data_Queued);
      session->inbound (ch);
    }
  else
    {
      ch->state(ACE::HTBP::Channel::Ready);
      session->outbound (ch);
    }
  return 1;
}

ssize_t
ACE::HTBP::Outside_Squid_Filter::recv_data_trailer (ACE::HTBP::Channel *ch)
{
  ch->state(ACE::HTBP::Channel::Send_Ack);
  return 1;
}

int
ACE::HTBP::Outside_Squid_Filter::send_ack (ACE::HTBP::Channel *ch)
{
  this->send_data_header (0,ch);
  if (ch->state() == ACE::HTBP::Channel::Header_Sent)
    ch->state(ACE::HTBP::Channel::Detached);
  return 1;
}

ssize_t
ACE::HTBP::Outside_Squid_Filter::send_data_header (ssize_t data_len,
                                                 ACE::HTBP::Channel *ch)
{
  ACE_CString header ("HTTP/1.1 200 OK\n"
                      "Content-Type: application/octet-stream\n"
                      "Content-Length: ");
  char datalenstr[20];
  ACE_OS::itoa (data_len,datalenstr,10);
  header += datalenstr;
  header += "\n\n";
  ssize_t result = ch->ace_stream().send(header.c_str(),header.length());
  ch->state(result == -1 ?
            ACE::HTBP::Channel::Closed : ACE::HTBP::Channel::Header_Sent);
  this->reset_http_code();
  return 1;
}

ssize_t
ACE::HTBP::Outside_Squid_Filter::send_data_trailer (ACE::HTBP::Channel *ch)
{
  ch->state(ACE::HTBP::Channel::Detached);
  return 1;
}

int
ACE::HTBP::Outside_Squid_Filter::recv_ack (ACE::HTBP::Channel *)
{
  return 1;
}

ACE_END_VERSIONED_NAMESPACE_DECL
