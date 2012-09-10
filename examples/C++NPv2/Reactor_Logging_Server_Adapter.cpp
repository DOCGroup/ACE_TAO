/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _REACTOR_LOGGING_SERVER_ADAPTER_C
#define _REACTOR_LOGGING_SERVER_ADAPTER_C

#include "ace/ACE.h"
#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/Truncate.h"
#include "Reactor_Logging_Server_Adapter.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::init (int argc,
                                                ACE_TCHAR *argv[]) {
  int i;
  char **array = 0;
  ACE_NEW_RETURN (array, char*[argc], -1);
  ACE_Auto_Array_Ptr<char *> char_argv (array);

  for (i = 0; i < argc; ++i)
    char_argv[i] = ACE::strnew (ACE_TEXT_ALWAYS_CHAR (argv[i]));
  ACE_NEW_NORETURN (server_, Reactor_Logging_Server<ACCEPTOR>
                               (i, char_argv.get (),
                                ACE_Reactor::instance ()));
  for (i = 0; i < argc; ++i) ACE::strdelete (char_argv[i]);
  return server_ == 0 ? -1 : 0;
}

// NOTE! The (ACCEPTOR*) cast below is NOT technically necessary, but it
// works around a bug in IBM XL C++ 9 that resolves to the wrong version of
// handle_close().
template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::fini ()
{ ((ACCEPTOR*)server_)->handle_close (); server_ = 0; return 0; }


template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::info
  (ACE_TCHAR **bufferp, size_t length) const {
  ACE_INET_Addr local_addr;
  server_->acceptor ().get_local_addr (local_addr);

  ACE_TCHAR buf[BUFSIZ];
  ACE_OS::sprintf (buf,
                   ACE_TEXT ("%hu"),
                   local_addr.get_port_number ());
  ACE_OS::strcat
    (buf, ACE_TEXT ("/tcp  # Reactor-based logging server\n"));
  if (*bufferp == 0)
    *bufferp = ACE::strnew (buf);
  else
    ACE_OS::strncpy (*bufferp, buf, length);

  return ACE_Utils::truncate_cast<int> (ACE_OS::strlen (*bufferp));
}


template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::suspend ()
{ return server_->reactor ()->suspend_handler (server_); }


template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::resume ()
{ return server_->reactor ()->resume_handler (server_); }

#endif /* _REACTOR_LOGGING_SERVER_ADAPTER_C */
