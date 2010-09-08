// $Id$

#if !defined (ACE_HANDLE_THR_STREAM_C)
#define ACE_HANDLE_THR_STREAM_C

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
#include "ace/INET_Addr.h"
#include "ace/TLI_Acceptor.h"
#include "Handle_Thr_Stream.h"

#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_time.h"

// Shorthand names.
#define SVH SVC_HANDLER
#define PR_AC_1 ACE_PEER_ACCEPTOR_1
#define PR_AC_2 ACE_PEER_ACCEPTOR_2
#define PR_ST_1 ACE_PEER_STREAM_1
#define PR_ST_2 ACE_PEER_STREAM_2

template <class SVH, PR_AC_1>
Handle_Thr_Acceptor<SVH, PR_AC_2>::~Handle_Thr_Acceptor (void)
{
}

template <class SVH, PR_AC_1>
Handle_Thr_Acceptor<SVH, PR_AC_2>::Handle_Thr_Acceptor (void)
  : thr_flags_ (THR_DETACHED | THR_NEW_LWP)
{
}

template <class SVH, PR_AC_1> int
Handle_Thr_Acceptor<SVH, PR_AC_2>::info (ACE_TCHAR **strp,
                                         size_t length) const
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_INET_Addr sa;

  if (this->acceptor ().get_local_addr (sa) == -1)
    {
      return -1;
    }

  ACE_OS::sprintf (buf, ACE_TEXT("%d/"), sa.get_port_number ());
  ACE_OS::strcat (buf, ACE_TEXT("tcp # tests threaded remote stream\n"));

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    {
      return -1;
    }
  else
    {
      ACE_OS::strncpy (*strp, buf, length);
    }

  return ACE_Utils::truncate_cast<int> (ACE_OS::strlen (buf));
}

template <class SVH, PR_AC_1> int
Handle_Thr_Acceptor<SVH, PR_AC_2>::init (int argc, ACE_TCHAR *argv[])
{
  ACE_INET_Addr local_addr (ACE_DEFAULT_THR_PORT);
  int n_threads = ACE_DEFAULT_THREADS;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("p:t:"), 0);

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'p':
        local_addr.set (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 't':
        n_threads = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        break;
      }

  // Initialize the threading strategy.
  if (this->thr_strategy_.open (&this->thr_mgr_,
                                this->thr_flags_,
                                n_threads) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);

  // Initialize the Acceptor base class, passing in the desired
  // concurrency strategy.
  else if (this->open (local_addr,
                       ACE_Reactor::instance (),
                       0,
                       0,
                       &this->thr_strategy_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);
  else
    return 0;
}

template <class SVH, PR_AC_1> int
Handle_Thr_Acceptor<SVH, PR_AC_2>::fini (void)
{
  return ACE_Reactor::instance ()->remove_handler
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

template <PR_ST_1>
CLI_Stream<PR_ST_2>::CLI_Stream (ACE_Thread_Manager *thr_mgr)
  : inherited (thr_mgr)
{
}

template <PR_ST_1> int
CLI_Stream<PR_ST_2>::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) client stream object closing down\n")));
  this->peer ().close ();

  // Must be allocated dynamically!
  delete this;
  return 0;
}

template <PR_ST_1> int
CLI_Stream<PR_ST_2>::open (void *)
{
  ACE_INET_Addr sa;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) client handle = %d\n"),
              this->peer ().get_handle ()));

  if (this->peer ().get_remote_addr (sa) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) accepted at port %d\n"),
              sa.get_port_number ()));
  return 0;
}

template <PR_ST_1> int
CLI_Stream<PR_ST_2>::svc (void)
{
  char buf[BUFSIZ];
  char login_name[ACE_MAX_USERID];
  int bytes;

  ACE_OS::puts ("----------------------------------------");

  while ((bytes = this->peer ().recv (buf, sizeof buf)) > 0)
    ACE_OS::write (ACE_STDOUT, buf, bytes);

  ACE_OS::puts ("----------------------------------------");
  ACE_OS::fflush (stdout);

  time_t t = ACE_OS::time (0L);
  ACE_OS::cuserid (login_name);
  ACE_OS::sprintf (buf,
                   "user %s %s",
                   login_name,
                   ACE_TEXT_ALWAYS_CHAR (ACE_OS::ctime ((const time_t *) &t)));

  if (this->peer ().send_n (buf, ACE_OS::strlen (buf) + 1) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG, "(%t) sent reply %s", buf));
  return 0;
}

#undef SVH
#undef PR_AC_1
#undef PR_AC_2
#undef PR_ST_1
#undef PR_ST_2

//----------------------------------------

#if defined (ACE_HAS_TLI)
#include "ace/TLI_Stream.h"
#include "ace/TLI_Acceptor.h"
#define THR_STREAM ACE_TLI_STREAM
#define THR_ACCEPTOR ACE_TLI_ACCEPTOR
#else
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#define THR_STREAM ACE_SOCK_STREAM
#define THR_ACCEPTOR ACE_SOCK_ACCEPTOR
#endif /* ACE_HAS_TLI */
#include "ace/INET_Addr.h"

typedef CLI_Stream <THR_STREAM> CLI_STREAM;
typedef Handle_Thr_Acceptor<CLI_STREAM, THR_ACCEPTOR> HANDLE_THR_ACCEPTOR;

// Service object.
HANDLE_THR_ACCEPTOR remote_thr_stream;
ACE_Service_Object_Type rts (&remote_thr_stream, ACE_TEXT("Remote_Thr_Stream"));

#endif /* ACE_HAS_THREADS */
#endif /* ACE_HANDLE_THR_STREAM_C */
