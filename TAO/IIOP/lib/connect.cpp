// = LIB
//     TAO
// = FILENAME
//     connect.cpp
// = AUTHOR
//     Chris Cleeland
// = VERSION
//     $Id$

#include "connect.h"
#include "roa.h"
#include "debug.h"


ROA_Handler::ROA_Handler(ACE_Thread_Manager* t)
  : SUPERCLASS(t)
{
  // Grab the singleton...at some later point in time
  // we can provide an argumented CTOR to have per-instance
  // parameters.
  params_ = ROA_PARAMS::instance();
  ACE_ASSERT (params_ != 0);
}

int
ROA_Handler::open(void*)
{
  ACE_INET_Addr addr;

  if (this->peer().get_remote_addr(addr) == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG, 
             "(%P|%t) %sconnection from client %s\n", 
             params_->using_threads() ? "threaded " : "",
	      addr.get_host_name())); 

  return 0;
}

int
ROA_Handler::handle_close(ACE_HANDLE fd, ACE_Reactor_Mask rm)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ROA_Handler::handle_close(%d,%d)\n", fd, rm));
  return SUPERCLASS::handle_close(fd, rm);
}

int
ROA_Handler::svc()
{
  int result = 0;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ROA_Handler::svc begin\n"));
  while ((result = handle_input()) >= 0)
    continue;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ROA_Handler::svc end\n"));
  
  return result;
}

int
ROA_Handler::handle_input(ACE_HANDLE fd)
{
  // CJCXXX The tasks of this method should change to something like
  // the following:
  // 1. call into GIOP to pull off the header
  // 2. construct a complete request
  // 3. dispatch that request and return any required reply and errors

  CORBA_Environment env;
  Dispatch_Context ctx;

  ctx.oa = params_->oa();
  ctx.endpoint = peer();

  // CJCXXX Knowledge of these will move into the OA so they don't
  // have to be copied every time.  Also, these should be set in the
  // by a call to register_object() or somesuch, because there will
  // be a different upcall and forwarder for every object (or is it
  // for every TYPE of object?).  I need to rename "context" so that
  // it has a more meaningful name.
  ctx.skeleton = params_->upcall();
  ctx.context = params_->context();
  ctx.check_forward = params_->forwarder();

  int ret;

  switch(params_->oa()->handle_message (ctx, env))
    {
    case 1:
    default:
      ret = 0;
      break;

    case 0:
    case -1:
      ret = -1;
      break;
    }

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
  return ret;
}

#if ! defined(__ACE_INLINE__)
#  include "connect.i"
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
