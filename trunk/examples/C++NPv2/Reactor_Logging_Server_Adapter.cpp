/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _REACTOR_LOGGING_SERVER_ADAPTER_C
#define _REACTOR_LOGGING_SERVER_ADAPTER_C

#include "ace/ACE.h"
#include "Reactor_Logging_Server_Adapter.h"

template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::init (int argc,
                                                ACE_TCHAR *argv[]) {
  int i;
  const int MAX_ARGS = 10;
  char *char_argv[MAX_ARGS];
  for (i = 0; i < argc && i < MAX_ARGS; ++i)
    char_argv[i] = ACE::strnew (ACE_TEXT_ALWAYS_CHAR (argv[i]));
  ACE_NEW_RETURN (server_,
                  Reactor_Logging_Server<ACCEPTOR>
                    (i, char_argv, ACE_Reactor::instance ()), -1);
  for (i = 0; i < argc && i < MAX_ARGS; ++i)
    ACE::strdelete (char_argv[i]);
  return 0;
}


template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::fini () 
{ server_->handle_close (); server_ = 0; return 0; }


template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::info
  (ACE_TCHAR **bufferp, size_t length) const {
  ACE_INET_Addr local_addr;
  server_->acceptor ().get_local_addr (local_addr);

  ACE_TCHAR buf[BUFSIZ];
  ACE_OS::sprintf (buf,
                   ACE_TEXT ("%hu"),
                   local_addr.get_port_number ());
  ACE_OS_String::strcat 
    (buf, ACE_TEXT ("/tcp  # Reactor-based logging server\n"));
  if (*bufferp == 0)
    *bufferp = ACE::strnew (buf);
  else
    ACE_OS_String::strncpy (*bufferp, buf, length);
  return ACE_OS_String::strlen (*bufferp);
}


template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::suspend () 
{ return server_->reactor ()->suspend_handler (server_); }


template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::resume () 
{ return server_->reactor ()->resume_handler (server_); }

#endif /* _REACTOR_LOGGING_SERVER_ADAPTER_C */
