/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "Reactor_Logging_Server_Adapter.h"

template <class ACCEPTOR> int
Reactor_Logging_Server_Adapter<ACCEPTOR>::init (int argc,
                                                ACE_TCHAR *argv[]) {
  ACE_DEBUG ((LM_DEBUG, "adapter init, argv[0] = %s\n", argv[0]));
  ACE_NEW_RETURN (server_,
                  Reactor_Logging_Server<ACCEPTOR>
                    (argc, argv, ACE_Reactor::instance ()), -1);
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
