// $Id$

#include "HTBP_ID_Requestor.h"
#include "HTBP_Environment.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/UUID.h"

ACE_RCSID(HTBP,
          ACE_HTBP_ID_Requestor,
          "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_TString ACE::HTBP::ID_Requestor::htid_;
ACE_SYNCH_MUTEX ACE::HTBP::ID_Requestor::htid_lock_;

ACE::HTBP::ID_Requestor::ID_Requestor (ACE::HTBP::Environment *env)
  : port_ (0),
    host_ (),
    url_()
{
  if (env)
    {
      env->get_htid_url(url_);
      int via_proxy = 0;
      env->get_htid_via_proxy (via_proxy);
      if (via_proxy && env->get_proxy_host (host_) == 0)
        env->get_proxy_port (port_);
    }
}

int
ACE::HTBP::ID_Requestor::connect_to_server (ACE_SOCK_Stream *cli_stream)
{
  if (port_ == 0 || host_.length() == 0)
    {
      int host_start = url_.find (ACE_TEXT("http://")) + 7;
      int port_sep = 0;
      int sep = 0;
      if (host_start == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("(%P|%t) ACE::HTBP::ID_Requestor::")
                           ACE_TEXT("connect_to_server: ")
                           ACE_TEXT("invalid URL: \"%s\"\n"),
                           url_.c_str()),
                          -1);
      port_sep = url_.find (ACE_TEXT(":"),(size_t)host_start);
      sep = url_.find (ACE_TEXT("/"),(size_t)host_start);
      if (sep == -1 || sep == host_start +1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("(%P|%t) ACE::HTBP::ID_Requestor::")
                           ACE_TEXT("connect_to_server: ")
                           ACE_TEXT("invalid URL: \"%s\"\n"),
                           url_.c_str()),
                          -1);
      if (port_sep == -1)
        {
          port_sep = sep;
          port_ = 80; // should be a default?
        }
      host_ = url_.substr(host_start,port_sep - host_start);
    }

  ACE_INET_Addr remote_addr (static_cast<u_short> (port_), host_.c_str());
  ACE_SOCK_Connector con;
  if (con.connect (*cli_stream,
                   remote_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P|%t) ACE::HTBP::ID_Requestor::")
                       ACE_TEXT("connect_to_server: ")
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("socket connect")),
                      -1);
  return 0;
}

int
ACE::HTBP::ID_Requestor::send_request (ACE_SOCK_Stream *cli_stream)
{
  char *buffer;
  ACE_NEW_RETURN (buffer, char[this->url_.length()+16],-1);
  ACE_Auto_Array_Ptr<char> guard (buffer);
  ACE_OS::sprintf (buffer,"GET %s HTTP/1.0\n\n",
                   ACE_TEXT_ALWAYS_CHAR(url_.c_str()));
  int result = cli_stream->send_n (buffer,ACE_OS::strlen(buffer));
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P|%t) ACE::HTBP::ID_Requestor::")
                       ACE_TEXT("send_request %p\n"),
                       ACE_TEXT("socket send")), -1);
  return 0;
}

ACE_TCHAR *
ACE::HTBP::ID_Requestor::get_HTID ()
{
  if (ACE::HTBP::ID_Requestor::htid_.length() != 0)
    return ACE::HTBP::ID_Requestor::htid_.rep();

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, ACE::HTBP::ID_Requestor::htid_lock_, 0);

  if (ACE::HTBP::ID_Requestor::htid_.length() != 0)
    return ACE::HTBP::ID_Requestor::htid_.rep();

  ACE_SOCK_Stream cli_stream;
  ACE_TCHAR * htid = 0;

  if (this->url_.length() == 0 ||
      this->connect_to_server (&cli_stream) == -1 ||
      this->send_request (&cli_stream) == -1)
    {
      ACE_Utils::UUID_Generator gen;
      ACE_Utils::UUID *uuid = gen.generate_UUID ();
      const ACE_CString *uuidstr = uuid->to_string();
      ACE::HTBP::ID_Requestor::htid_ = ACE_TEXT_CHAR_TO_TCHAR (uuidstr->c_str());
      delete uuid;
      return ACE::HTBP::ID_Requestor::htid_.rep();
    }
  iovec recv_buf;
  ssize_t result = cli_stream.recvv (&recv_buf);
  cli_stream.close();

  if (result > 0)
    {
      ACE_CString answer ((char *)recv_buf.iov_base,recv_buf.iov_len);
      ACE_CString::size_type start = answer.rfind (ACE_TEXT('\n'));
      if (start == ACE_CString::npos)
        start = 0;
      else
        start++;
      ACE::HTBP::ID_Requestor::htid_ = ACE_TEXT_CHAR_TO_TCHAR(answer.substr (start).c_str());
      htid = ACE::HTBP::ID_Requestor::htid_.rep();
    }
  return htid;
}

ACE_END_VERSIONED_NAMESPACE_DECL
