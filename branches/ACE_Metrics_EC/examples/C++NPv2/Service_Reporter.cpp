/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Types.h"
#include "ace/SOCK_Stream.h"
#include "Service_Reporter.h"


int Service_Reporter::init (int argc, ACE_TCHAR *argv[]) {
  ACE_INET_Addr local_addr (Service_Reporter::DEFAULT_PORT);

  // Start at argv[0].
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:"), 0);
  get_opt.long_option (ACE_TEXT ("port"),
                       'p',
                       ACE_Get_Opt::ARG_REQUIRED);

  for (int c; (c = get_opt ()) != -1; )
    switch (c) {
    case 'p':
      local_addr.set_port_number
        (ACE_OS::atoi (get_opt.opt_arg ()));
    }

  acceptor_.open (local_addr);
  return reactor ()->register_handler
           (this,
            ACE_Event_Handler::ACCEPT_MASK);
}


int Service_Reporter::handle_input (ACE_HANDLE) {
  ACE_SOCK_Stream peer_stream;
  acceptor_.accept (peer_stream);

  ACE_Service_Repository_Iterator iterator
    (*ACE_Service_Repository::instance (), 0);

  for (const ACE_Service_Type *st;
       iterator.next (st) != 0;
       iterator.advance ()) {
    iovec iov[3];
    iov[0].iov_base = ACE_const_cast (char *, st->name ());
    iov[0].iov_len =
      ACE_OS_String::strlen (st->name ()) * sizeof (ACE_TCHAR);
    const ACE_TCHAR *state = st->active () ?
           ACE_TEXT (" (active) ") : ACE_TEXT (" (paused) ");
    iov[1].iov_base = ACE_const_cast (char *, state);
    iov[1].iov_len =
      ACE_OS_String::strlen (state) * sizeof (ACE_TCHAR);
    ACE_TCHAR *report = 0;   // Ask info() to allocate buffer
    int len = st->type ()->info (&report, 0);
    iov[2].iov_base = ACE_static_cast (char *, report);
    iov[2].iov_len = ACE_static_cast (size_t, len);
    iov[2].iov_len *= sizeof (ACE_TCHAR);
    peer_stream.sendv_n (iov, 3);
    ACE::strdelete (report);
  }

  peer_stream.close ();
  return 0;
}


int Service_Reporter::info (ACE_TCHAR **bufferp,
                            size_t length) const {
  ACE_INET_Addr local_addr;
  acceptor_.get_local_addr (local_addr);

  ACE_TCHAR buf[BUFSIZ];
  ACE_OS::sprintf
    (buf, ACE_TEXT ("%hu"), local_addr.get_port_number ());
  ACE_OS_String::strcat
    (buf, ACE_TEXT ("/tcp # lists services in daemon\n"));
  if (*bufferp == 0)
    *bufferp = ACE::strnew (buf);
  else
    ACE_OS_String::strncpy (*bufferp, buf, length);
  return ACE_OS_String::strlen (*bufferp);
}


int Service_Reporter::suspend ()
{ return reactor ()->suspend_handler (this); }

int Service_Reporter::resume ()
{ return reactor ()->resume_handler (this); }


int Service_Reporter::fini () {
  reactor ()->remove_handler
    (this,
     ACE_Event_Handler::ACCEPT_MASK
     | ACE_Event_Handler::DONT_CALL);
  return acceptor_.close ();
}

// Define the service object make and gobble functions.
ACE_FACTORY_DEFINE (ACE_Local_Service, Service_Reporter)

// Define the ACE_Static_Svc_Descriptor that conveys the service information
// to the ACE_Service_Config.
ACE_STATIC_SVC_DEFINE (
  Reporter_Descriptor,
  ACE_TEXT ("Service_Reporter"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (Service_Reporter),
  ACE_Service_Type::DELETE_THIS
  | ACE_Service_Type::DELETE_OBJ,
  0 // This object is not initially active.
)

// Define the class that will register this service with ACE_Service_Config
// at program startup.
ACE_STATIC_SVC_REQUIRE (Reporter_Descriptor)
