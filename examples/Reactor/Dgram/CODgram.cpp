/* Exercise the ACE_SOCK_CODgram wrapper along with the ACE_Reactor.
// @(#)CODgram.cpp	1.1	10/18/96


   Typical invocation sequence is:

   % CODgram 10000 localhost 10001 &
   % CODgram 10001 localhost 10000

   This will start two interacting copies of the CODgram
   application. */

#include "ace/Reactor.h"
#include "ace/SOCK_CODgram.h"
#include "ace/INET_Addr.h"

class AAL_CP : public ACE_Event_Handler, public ACE_SOCK_CODgram
{
public:
  AAL_CP (const ACE_INET_Addr &remote_addr, 
	  const ACE_INET_Addr &local_addr);

  virtual int get_handle () const;

  virtual int handle_input (int fd);

  virtual int handle_timeout (const ACE_Time_Value & tv, 
			      const void *arg = 0);
};

AAL_CP::AAL_CP (const ACE_INET_Addr &remote_addr, 
		const ACE_INET_Addr &local_addr)
  : ACE_SOCK_CODgram (remote_addr, local_addr)
{
}

int
AAL_CP::get_handle () const
{
  return ACE_SOCK_CODgram::get_handle ();
}

int
AAL_CP::handle_input (int)
{
  char buf[128];
  int n;
  ACE_DEBUG ((LM_DEBUG, "Activity occurred on handle %d!\n",
	      ACE_SOCK_CODgram::get_handle ()));
  if ((n = ACE_SOCK_CODgram::recv (buf, sizeof buf)) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "handle_input"));
  else
    ACE_DEBUG ((LM_DEBUG, "got buf = %s\n", buf));
  
  return 0;
}

int
AAL_CP::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_DEBUG ((LM_DEBUG, "timed out for aa1\n"));
  return 0;
}

main(int argc, char *argv[])
{
  /* Estabish call backs, and socket names */
  if (argc != 4)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "usage: %s localport remotehost remoteport\n", 
		       argv[0]), -1);

  ACE_Reactor reactor;
  char	  buf[128];
  u_short localport  = ACE_OS::atoi (argv[1]);
  u_short remoteport = ACE_OS::atoi (argv[3]);
  char	  *remotehost = argv[2];

  ACE_INET_Addr remote_addr (remoteport, remotehost);
  ACE_INET_Addr local_addr (localport);

  AAL_CP aal (remote_addr, local_addr);

  if (localport == 10000)	// HACK
  {
    ACE_OS::memcpy (buf, "Data to transmit", sizeof buf);
    ACE_DEBUG ((LM_DEBUG, "sending data\n"));

    for (int i = 0; i < 20; i++)
      {
	aal.send (buf, sizeof buf);
	ACE_DEBUG ((LM_DEBUG, ".\n"));
	ACE_OS::sleep (1);
      }
  }

  /* read data from other side */
  if (reactor.register_handler (&aal, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_Reactor::register_handler"), -1);

  if (reactor.schedule_timer (&aal, 0, 
			      ACE_Time_Value (1, 0), 
			      ACE_Time_Value (0, 3500000)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_Reactor::schedule_timer"), -1);

  ACE_OS::memcpy (buf, "Data to transmit", sizeof buf);

  for (;;)
    {
      /* Wait at most two seconds */
      ACE_Time_Value tv (2, 0);

      reactor.handle_events (tv);

      ACE_DEBUG ((LM_DEBUG, "return from handle events\n"));
      aal.send (buf, sizeof buf);
      ACE_DEBUG ((LM_DEBUG, ".\n"));
    }

  return 0;
}
