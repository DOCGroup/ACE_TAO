#include "roa.hh"
#include "tcpoa.hh"
#include "debug.hh"

int         ACE_ROA::end_reactor_event_loop_ = 0;
ACE_Reactor ACE_ROA::theReactor;
int         ACE_ROA::usingThreads_ = 0;
void*       ACE_ROA::context_p = 0;
ACE_ROA::UpcallFunc ACE_ROA::theUpcall = 0;
TCP_OA_ptr     ACE_ROA::theOA = 0;

ROA_Handler::ROA_Handler()
{
}

int
ROA_Handler::open(void*)
{
  ACE_INET_Addr addr;

  if (this->peer().get_remote_addr(addr) == -1)
    return -1;
  else
    {
      if (ACE_ROA::reactor()->register_handler(this, ACE_Event_Handler::READ_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) can't register with reactor\n"), -1);
      else
	ACE_DEBUG ((LM_DEBUG,
		    "(%P|%t) connection from client %s\n",
		    addr.get_host_name()));
      return 0;
    }
}

int
ROA_Handler::handle_close(ACE_HANDLE fd, ACE_Reactor_Mask rm)
{
}

int
ROA_Handler::handle_input(ACE_HANDLE fd)
{
  //
  // Handle it in this thread.  We can do it without any need
  // to dynamically allocate memory.
  //
  CORBA_Environment env;
  TCP_OA::dispatch_context ctx;

  ctx.skeleton = ACE_ROA::upcall();
  ctx.context = ACE_ROA::context();
  // For right now, the value here doesn't matter.  But, when we do
  // threads, we'll need to figure out some way to get commute a
  // handle to the OA to the handling mechanism (i.e., thread).
  ctx.oa = ACE_ROA::oa();
  ctx.endpoint = fd;

#ifdef	_POSIX_THREADS
  ctx.aggressive = CORBA_B_FALSE;
#endif

  // Need to have a handle to a TCP_OA instance to make this call!!
  ACE_ROA::oa()->handle_message (ctx, env);

  //
  // Don't report any errors from the application/skeleton back to the
  // top level ... the application already has a variety of means to
  // pass whatever data needs passing, and we don't need to confuse
  // things by mixing ORB and application errors here.
  //
  if (env.exception () != 0) {
    dexc (env, "ROA_Handler, handle incoming message");
    env.clear ();
  }
}

#if ! defined(__ACE_INLINE__)
#  include "roa.i"
#endif

#if defined(ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
// Direct
template class ACE_Acceptor<ROA_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
// Indirect
template class ACE_Task<ACE_NULL_SYNCH>;// ACE_Svc_Handler
template class ACE_TSS<ACE_Dynamic>; // ACE_Svc_Handler
template class ACE_Message_Queue<ACE_NULL_SYNCH>; // ACE_Task
template class ACE_Module<ACE_NULL_SYNCH>; // ACE_Task
template class ACE_Thru_Task<ACE_NULL_SYNCH>; // ACE_Module
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
