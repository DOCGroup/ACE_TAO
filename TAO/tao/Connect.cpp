// $Id$

#include "tao/corba.h"
#include "tao/Timeprobe.h"

#if !defined (__ACE_INLINE__)
# include "tao/Connect.i"
#endif /* ! __ACE_INLINE__ */

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Connect_Timeprobe_Description[] =
{
  "Server_Connection_Handler::send_response - start",
  "Server_Connection_Handler::send_response - end",

  "Server_Connection_Handler::handle_input - start",
  "Server_Connection_Handler::handle_input - end",

  "Client_Connection_Handler::send_request - start",
  "Client_Connection_Handler::send_request - end",
};

enum
{
  // Timeprobe description table start key 
  TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_START = 300,
  TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_END,

  TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_START,
  TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_END,

  TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_START,
  TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Connect_Timeprobe_Description,
                                  TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_Server_Connection_Handler::TAO_Server_Connection_Handler (ACE_Thread_Manager* t)
  : TAO_SVC_HANDLER (t, 0, 0),
    parent_ (0)
{
  // Grab the singleton...at some later point in time we can provide
  // an argumented CTOR to have per-instance parameters.

  this->params_ = TAO_ORB_Core_instance ()->oa_params ();
  ACE_ASSERT (params_ != 0);
}

int
TAO_Server_Connection_Handler::open (void*)
{
  // Called by the <Strategy_Acceptor> when the handler is completely
  // connected.
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

#if !defined (ACE_LACKS_SOCKET_BUFSIZ)
  int sndbufsize =
    TAO_ORB_Core_instance ()->orb_params ()->sock_sndbuf_size ();
  int rcvbufsize =
    TAO_ORB_Core_instance ()->orb_params ()->sock_rcvbuf_size ();

  if (this->peer ().set_option (SOL_SOCKET,
                                SO_SNDBUF,
                                (void *) &sndbufsize,
                                sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->peer ().set_option (SOL_SOCKET,
                                     SO_RCVBUF,
                                     (void *) &rcvbufsize,
                                     sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return -1;
#endif /* !ACE_LACKS_SOCKET_BUFSIZ */

#if defined (TCP_NODELAY)
  int nodelay = 1;
  if (this->peer ().set_option (IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &nodelay,
                                sizeof (nodelay)) == -1)
    return -1;
#endif /* TCP_NODELAY */

  char client[MAXHOSTNAMELEN + 1];

  if (addr.get_host_name (client, MAXHOSTNAMELEN) == -1)
    {
      addr.addr_to_string (client, sizeof (client));
    }

  if (TAO_orbdebug)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) connection from client %s\n",
                client));
  return 0;
}

int
TAO_Server_Connection_Handler::activate (long flags,
                                         int n_threads,
                                         int force_active,
                                         long priority,
                                         int grp_id,
                                         ACE_Task_Base *task,
                                         ACE_hthread_t thread_handles[],
                                         void *stack[],
                                         size_t stack_size[],
                                         ACE_thread_t  thread_names[])
{
  this->parent_ = TAO_ORB_Core_instance ();
  this->thr_mgr (this->parent_->thr_mgr ());
  return TAO_SVC_HANDLER::activate (flags,
                                    n_threads,
                                    force_active,
                                    priority,
                                    grp_id,
                                    task,
                                    thread_handles,
                                    stack,
                                    stack_size,
                                    thread_names);
}

int
TAO_Server_Connection_Handler::handle_close (ACE_HANDLE handle,
                                             ACE_Reactor_Mask rm)
{
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "(%P|%t) TAO_Server_Connection_Handler::handle_close (%d, %d)\n",
                 handle,
                 rm));

  return TAO_SVC_HANDLER::handle_close (handle, rm);
}

int
TAO_Server_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e.,
  // turned into an active object.  Presumably, activation spawns a
  // thread with this method as the "worker function".
  int result = 0;

  // Inheriting the ORB_Core stuff from the parent thread.

  TAO_ORB_Core_instance ()->inherit_from_parent_thread (this->parent_);

  if (TAO_orbdebug)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) TAO_Server_Connection_Handler::svc begin\n"));

  // Here we simply synthesize the "typical" event loop one might find
  // in a reactive handler, except that this can simply block waiting
  // for input.

  while ((result = handle_input ()) >= 0)
    continue;

  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "(%P|%t) TAO_Server_Connection_Handler::svc end\n"));

  return result;
}

// Handle processing of the request residing in <input>, setting
// <response_required> to zero if the request is for a oneway or
// non-zero if for a two-way and <output> to any necessary response
// (including errors).  In case of errors, -1 is returned and
// additional information carried in <env>.
// The request ID is needed by handle_input. It is passed back
// as reference.

int
TAO_Server_Connection_Handler::handle_message (TAO_InputCDR &input,
                                               TAO_OutputCDR &output,
                                               int &response_required,
                                               CORBA::ULong &request_id,
                                               CORBA::Environment &env)
{
  TAO_POA *the_poa = TAO_ORB_Core_instance ()->root_poa ();

  // This will extract the request header, set <response_required> as
  // appropriate.
  IIOP_ServerRequest request (input,
                              output,
                              TAO_ORB_Core_instance ()->orb (),
                              the_poa,
                              env);

  // The request_id_ field in request will be 0 if something went
  // wrong before it got a chance to read it out.
  request_id = request.request_id ();

  if (env.exception ())
    return -1;

  response_required = request.response_expected ();

  // So, we read a request, now handle it using something more
  // primitive than a CORBA2 ServerRequest pseudo-object.

  // @@ (CJC) We need to create a TAO-specific request which will hold
  // context for a request such as the connection handler ("this") over
  // which the request was received so that the servicer of the request
  // has sufficient context to send a response on its own.
  //
  // One thing which me must be careful of is that responses are sent
  // with a single write so that they're not accidentally interleaved
  // over the transport (as could happen using TCP).

  the_poa->dispatch_servant (request.object_key (),
                             request,
                             0,
                             env);


  // Need to check for any errors present in <env> and set the return
  // code appropriately.
  if (env.exception () != 0)
    return -1;
  else
    return 0;
}

int
TAO_Server_Connection_Handler::handle_locate (TAO_InputCDR &input,
                                              TAO_OutputCDR &output,
                                              int &response_required,
                                              CORBA::ULong &request_id,
                                              CORBA::Environment &env)
{
  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_LocateRequestHeader req;

  env.clear ();
  if (! req.init (input, env))
    {
      // @@ FIXME! Need to set <env>.
      request_id = req.request_id;
      response_required = 0;
      return -1;
    }
  // Copy the request ID to be able to respond in case of an exception
  request_id = req.request_id;

  response_required = 1; // Be optimistic

  // So, we read a request, now handle it using something more
  // primitive than a CORBA2 ServerRequest pseudo-object.
  TAO_POA *the_poa = TAO_ORB_Core_instance ()->root_poa ();
  TAO_GIOP_LocateStatusType status;

  if ((the_poa->locate_servant (req.object_key, env) == 0)
      && (env.exception() == 0))
    // We found it!
    status = TAO_GIOP_OBJECT_HERE;
  else
    {
      // not found, report unknown
      status = TAO_GIOP_UNKNOWN_OBJECT;

      // the locate_servant call might have thrown an exception
      // but we don't want to marshal it because it is no failure.
      // The proper Locacte_Reply will tell the client what is going on.

      // // Remove the exception
      env.clear ();
    }

  // Create the response.
  TAO_GIOP::start_message (TAO_GIOP::LocateReply, output);
  output.write_ulong (req.request_id);
  output.write_ulong (status);

  if (env.exception () != 0)
    // An exception was thrown
    return -1;
  else
    // Everything is ok.
    return 0;
}

void
TAO_Server_Connection_Handler::send_response (TAO_OutputCDR &output)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_START);

  TAO_SVC_HANDLER *this_ptr = this;
  TAO_GIOP::send_request (this_ptr, output);
}

// This method is designed to return system exceptions to the caller
void
TAO_Server_Connection_Handler::send_error (CORBA::ULong request_id,
                                           CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_START);

  // The request_id is going to be not 0, if it was sucessfully read
  if (request_id != 0)
  {
    // Create a new output CDR stream
    TAO_OutputCDR output;

    // Construct a REPLY header.
    TAO_GIOP::start_message (TAO_GIOP::Reply, output);

    // A new envrionment, if something goes wrong now -> no hope!
    CORBA::Environment env2;

    // create and write a dummy context
    TAO_GIOP_ServiceContextList resp_ctx;
    resp_ctx.length (0);
    output.encode (TC_ServiceContextList,
                   &resp_ctx,
                   0,
                   env2);

    if (env2.exception() == 0)
    {
      // Write the request ID
      output.write_ulong (request_id);

      // Write the exception
      CORBA::Exception *x = env.exception ();
      CORBA::TypeCode_ptr except_tc = x->_type ();

      // Now we check for Forwarding ***************************

      // Try to narrow to ForwardRequest
      PortableServer::ForwardRequest_ptr forward_request_ptr =
        PortableServer::ForwardRequest::_narrow (env.exception());

      // If narrowing of exception succeeded
      if (forward_request_ptr != 0 &&
          !CORBA::is_nil (forward_request_ptr->forward_reference.in ()))
      {
        // write the reply_status
        output.write_ulong (TAO_GIOP_LOCATION_FORWARD);

        // write the object reference into the stream
        CORBA::Object_ptr object_ptr = forward_request_ptr->forward_reference.in();

        output.encode (CORBA::_tc_Object,
                       &object_ptr,
                       0,
                       env2);
      }
      // end of the forwarding code ****************************
      else
      {
        // write the reply_status
        output.write_ulong (TAO_GIOP::convert_CORBA_to_GIOP_exception (env.exception_type ()));

        // write the actual exception
        output.encode (except_tc, x, 0, env2);
      }

      // exception handling for both alternatives
      if (env2.exception() == 0)
      {
        // hand it to the next lower layer
        TAO_SVC_HANDLER *this_ptr = this;
        TAO_GIOP::send_request (this_ptr, output);
        // now we have done all what was possible,
        // send_request might have had an error
        // and closed the connection, but we are done.
        return;
      }
    }
  }
  // now we know, that while handling the error an other
  // error happened -> no hope, close connection.

  // close the handle
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) closing conn %d after fault %p\n",
              this->peer().get_handle (), "TAO_Server_ConnectionHandler::send_error"));
  this->close ();
}


int
TAO_Server_Connection_Handler::handle_input (ACE_HANDLE)
{
  // CJCXXX The tasks of this method should change to something like
  // the following:
  // 1. call into GIOP to pull off the header
  // 2. construct a complete request
  // 3. dispatch that request and return any required reply and errors

  ACE_FUNCTION_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_START);



  // @@ TODO This should take its memory from a specialized
  // allocator. It is better to use a message block than a on stack
  // buffer because we cannot minimize memory copies in that case.
  TAO_InputCDR input (CDR::DEFAULT_BUFSIZE);

  char repbuf[CDR::DEFAULT_BUFSIZE];
#if defined(ACE_PURIFY)
  (void) ACE_OS::memset (repbuf, '\0', sizeof (repbuf));
#endif /* ACE_PURIFY */
  TAO_OutputCDR output (repbuf, sizeof(repbuf));

  int result = 0;
  int error_encountered = 0;
  int response_required;
  TAO_SVC_HANDLER *this_ptr = this;
  CORBA::Environment env;
  CORBA::ULong request_id;

  switch (TAO_GIOP::recv_request (this_ptr, input))
    {
    case TAO_GIOP::Request:
      // Message was successfully read, so handle it.  If we encounter
      // any errors, <output> will be set appropriately by the called
      // code, and -1 will be returned.
      if (this->handle_message (input,
                                output,
                                response_required,
                                request_id,
                                env) == -1)
        error_encountered = 1;
      break;

    case TAO_GIOP::LocateRequest:
      if (this->handle_locate (input,
                               output,
                               response_required,
                               request_id,
                               env) == -1)
        error_encountered = 1;
      break;

    case TAO_GIOP::EndOfFile:
      // Got a EOF
      errno = EPIPE;
      response_required = error_encountered = 0;
      result = -1;
      break;

      // These messages should never be sent to the server; it's an
      // error if the peer tries.  Set the environment accordingly, as
      // it's not yet been reported as an error.
    case TAO_GIOP::Reply:
    case TAO_GIOP::LocateReply:
    case TAO_GIOP::CloseConnection:
    default:                                    // Unknown message
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Illegal message received by server\n"));
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
      // FALLTHROUGH

    case TAO_GIOP::MessageError:
      error_encountered = 1;
      break;
    }

  if (response_required && !error_encountered)
    // Normal response
    this->send_response (output);
  else if (error_encountered && (env.exception() != 0))
    // Something happened and we know why
    this->send_error (request_id, env);
  else if (error_encountered)
  {
    // Now we are completely lost
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) closing conn %d after fault %p\n",
                this->peer().get_handle (),
                "TAO_Server_ConnectionHandler::handle_input"));
    this->close ();
    result = -1;
  }
  // else there was no repsonse expected and no error happened

  return result;
}

TAO_Client_Connection_Handler::TAO_Client_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_SVC_HANDLER (t, 0, 0),
    input_available_ (0),
    calling_thread_ (0)
{
  this->cond_response_available_ = 
    new ACE_SYNCH_CONDITION (TAO_ORB_Core_instance ()->leader_follower_lock ());
}

TAO_Client_Connection_Handler::~TAO_Client_Connection_Handler ()
{
  delete this->cond_response_available_;
}

int
TAO_Client_Connection_Handler::open (void *)
{
  // Here is where we could enable all sorts of things such as
  // nonblock I/O, sock buf sizes, TCP no-delay, etc.

#if !defined (ACE_LACKS_SOCKET_BUFSIZ)
  int sndbufsize =
    TAO_ORB_Core_instance ()->orb_params ()->sock_sndbuf_size ();
  int rcvbufsize =
    TAO_ORB_Core_instance ()->orb_params ()->sock_rcvbuf_size ();

  if (this->peer ().set_option (SOL_SOCKET,
                                SO_SNDBUF,
                                (void *) &sndbufsize,
                                sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->peer ().set_option (SOL_SOCKET,
                                     SO_RCVBUF,
                                     (void *) &rcvbufsize,
                                     sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return -1;
#endif /* ACE_LACKS_SOCKET_BUFSIZ */

  int nodelay = 1;
  if (this->peer ().set_option (IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &nodelay,
                                sizeof (nodelay)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "NODELAY failed\n"), -1);

  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();

  // Now we must register ourselves with the reactor for input events
  // which will detect GIOP Reply messages and EOF conditions.
  r->register_handler (this, ACE_Event_Handler::READ_MASK);

  // For now, we just return success
  return 0;
}

int
TAO_Client_Connection_Handler::close (u_long flags)
{
  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();

  // Now we must register ourselves with the reactor for input events
  // which will detect GIOP Reply messages and EOF conditions.
  r->remove_handler (this, ACE_Event_Handler::DONT_CALL);

  return BASECLASS::close (flags);
}

int
TAO_Client_Connection_Handler::send_request (TAO_OutputCDR &stream,
                                             int is_twoway)
{
  ACE_FUNCTION_TIMEPROBE (TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_START);

  // NOTE: Here would also be a fine place to calculate a digital
  // signature for the message and place it into a preallocated slot
  // in the "ServiceContext".  Similarly, this is a good spot to
  // encrypt messages (or just the message bodies) if that's needed in
  // this particular environment and that isn't handled by the
  // networking infrastructure (e.g. IPSEC).
  //
  // We could call a template method to do all this stuff, and if the
  // connection handler were obtained from a factory, then this could
  // be dynamically linked in (wouldn't that be cool/freaky?)

  if (!is_twoway)
  {
    int success  = (int) TAO_GIOP::send_request (this, stream);
    
    if (!success)
      return -1;
  }
  else // is_twoway
    {
      if (TAO_ORB_Core_instance ()->leader_follower_lock ().acquire() == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::send_request: "
			   "Failed to get the lock.\n"),
			  -1);

      // Set the state so that we know we're looking for a response.
      this->expecting_response_ = 1;
      // remember in which thread the client connection handler was running
      this->calling_thread_ = ACE_Thread::self ();

      
      int success  = (int) TAO_GIOP::send_request (this, stream);
      
      if (!success)
	{
	  TAO_ORB_Core_instance ()->leader_follower_lock ().release ();
	  return -1;
	}
      
      // check if there is a leader, but the leader is not us
      if (TAO_ORB_Core_instance ()->leader_available ()
	  && !TAO_ORB_Core_instance ()->I_am_the_leader_thread ())
	{
	  // wait as long as no input is available and/or 
	  // no leader is available
	  while (!this->input_available_ 
		 && TAO_ORB_Core_instance ()->leader_available ())
	    {
	      if (TAO_ORB_Core_instance ()->add_follower (this->cond_response_available_) == -1)
		ACE_ERROR ((LM_ERROR,
			    "(%P|%t) TAO_Client_Connection_Handler::send_request: "
			    "Failed to add a follower thread\n"));
	      this->cond_response_available_->wait ();     
	    }
	  // now somebody woke us up to become a leader or
	  // to handle our input. We are already removed from the 
	  // follower queue
	  if (this->input_available_)
	    {
	      // there is input waiting for me
	      if (TAO_ORB_Core_instance ()->leader_follower_lock ().release () == -1)
		ACE_ERROR_RETURN ((LM_ERROR,
				   "(%P|%t) TAO_Client_Connection_Handler::send_request: "
				   "Failed to release the lock.\n"),
				  -1);
	      
	      int ret = 0; 
	      if (ret < 0)
		{
		  ACE_DEBUG ((LM_DEBUG,
			      "Client_Connection_Handler::send_request: (%d) "
			      "failure faking handle_input\n",
			      ACE_Thread::self ()));
		  TAO_ORB_Core_instance ()->reactor ()->remove_handler (this, 
                                                                ACE_Event_Handler::ALL_EVENTS_MASK);
		  // failure handling
		  return -1;
		}
	      /* else if (ret > 0)
		 // we have to reschedule, not implemented yet 
	      */
	      
	      // the following variables are safe, because we are not registered with 
	      // the reactor any more.
	      this->input_available_ = 0;
	      this->expecting_response_ = 0;
	      this->calling_thread_ = 0;
	      return 0;
	    }
	}
      
      // become a leader, because there is no leader or we have to update to a leader
      // or we are doing nested upcalls in this case we do increase the refcount
      // on the leader in TAO_ORB_Core.
      
      TAO_ORB_Core_instance ()->set_leader_thread ();
      // this might increase the recount of the leader
      
      if (TAO_ORB_Core_instance ()->leader_follower_lock ().release () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::send_request: "
			   "Failed to release the lock.\n"),
			  -1);
      
      ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();
      r->owner (ACE_Thread::self ());
      
      int ret = 0;
      
      while (ret != -1 && !this->input_available_)
	ret = r->handle_events ();
      
      if (ret == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::send_request: "
			   "handle_events failed.\n"),
			  -1);
      
      
      // wake up the next leader, we cannot do that in handle_input,
      // because the woken up thread would try to get into handle_events,
      // which is at the time in handle_input still occupied.
      
      if (TAO_ORB_Core_instance ()->unset_leader_wake_up_follower () == -1) 
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::send_request: "
			   "Failed to unset the leader and wake up a new follower.\n"),
			  -1);
      
      // Make use reusable
      this->input_available_ = 0;
      this->expecting_response_ = 0;
      this->calling_thread_ = 0;
    }
  
  return 0;
}

int
TAO_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  if (TAO_ORB_Core_instance ()->leader_follower_lock ().acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "(%P|%t) TAO_Client_Connection_Handler::handle_input: "
		       "Failed to get the lock.\n"),
		      -1);
  
  if (!this->expecting_response_)
    {
      // we got something, but did not want  
      // @@ wake up an other thread, we are lost
      
      if (TAO_ORB_Core_instance ()->leader_follower_lock ().release () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::handle_input: "
			   "Failed to release the lock.\n"),
			  -1);
      
      // We're a client, so we're not expecting to see input.  Still
      // we better check what it is!
      char ignored;
      ssize_t ret;
      
    ret = this->peer().recv_n (&ignored, sizeof ignored);
    
    switch (ret)
      {
      case -1:
        // Error...but we weren't expecting input, either...what
        // should we do?
        ACE_ERROR ((LM_WARNING,
                    "Client_Connection_Handler::handle_input: closing connection on fd %d\n",
                    this->peer().get_handle ()));
        break;
	
      case 1:
        // We weren't expecting input, so what should we do with it?
        // Log an error, and close the connection.
        ACE_ERROR ((LM_WARNING,
                    "Client_Connection_Handler::handle_input received "
                    "input while not expecting a response; closing connection on fd %d\n",
                    this->peer().get_handle ()));
        break;
	
      case 0:
        // This is an EOF, so we will return -1 and let
        // handle_close() take over.  As long as handle_close()
        // calls the Svc_Handler<>::handle_close(), the socket will
        // be shutdown properly.
        break;
      }
    
    // We're not expecting input at this time, so we'll always
    // return -1 for now.
    return -1;
    }
  
  if (this->calling_thread_ == ACE_Thread::self ())
    {
      // we are now a leader getting its response
      // or a follower faking the handle_input
      
      this->input_available_ = 1;
      
      if (TAO_ORB_Core_instance ()->leader_follower_lock ().release () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::handle_input: "
			   "Failed to release the lock.\n"),
			  -1);
      
      TAO_ORB_Core_instance ()->reactor ()->suspend_handler (this);
      // resume_handler is called in TAO_GIOP_Invocation::invoke
      
      return 0;
    }
  else 
    {
      // we are a leader, which got a response for one of the followers, 
      // which means we are now a thread running the wrong Client_Connection_Handler
      
      TAO_ORB_Core_instance ()->remove_follower (this->cond_response_available_);
      
      if (TAO_ORB_Core_instance ()->leader_follower_lock ().release () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::handle_input: "
			   "Failed to release the lock.\n"),
			  -1);
      
      
      TAO_ORB_Core_instance ()->reactor ()->suspend_handler (this);
      // @@ Michael: we should wake suspend the thread before we wake him up.
      // resume_handler is called in TAO_GIOP_Invocation::invoke
      
      if (TAO_ORB_Core_instance ()->leader_follower_lock ().acquire () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::handle_input: "
			   "Failed to acquire the lock.\n"),
			  -1);

      // the thread was already selected to become a leader,
      // so we will be called again.
      this->input_available_ = 1;
      this->cond_response_available_->signal ();
      
      if (TAO_ORB_Core_instance ()->leader_follower_lock ().release () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) TAO_Client_Connection_Handler::handle_input: "
			   "Failed to release the lock.\n"),
			  -1);     
      return 0;
    }
}

int
TAO_Client_Connection_Handler::handle_close (ACE_HANDLE handle,
                                             ACE_Reactor_Mask rm)
{
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "(%P|%t) TAO_Client_Connection_Handler::handle_close (%d, %d)\n",
                 handle,
                 rm));

  return BASECLASS::handle_close (handle, rm);
}

#define TAO_SVC_TUPLE ACE_Svc_Tuple<TAO_Client_Connection_Handler>
#define TAO_HASH_ADDR ACE_Hash_Addr<ACE_INET_Addr>
#define TAO_RECYCLABLE_ADDR ACE_Recyclable<TAO_HASH_ADDR>
#define TAO_HASH_RECYCLABLE_ADDR ACE_Hash_Recyclable<TAO_HASH_ADDR>

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Svc_Handler<TAO_SOCK_STREAM, ACE_NULL_SYNCH>;
template class TAO_HASH_ADDR;
template class TAO_RECYCLABLE_ADDR;
template class TAO_HASH_RECYCLABLE_ADDR;
template class TAO_SVC_TUPLE;
template class ACE_Map_Manager<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int, TAO_SVC_TUPLE*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler<TAO_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate TAO_HASH_ADDR
#pragma instantiate TAO_RECYCLABLE_ADDR
#pragma instantiate TAO_HASH_RECYCLABLE_ADDR
#pragma instantiate TAO_SVC_TUPLE
#pragma instantiate ACE_Map_Manager<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int, TAO_SVC_TUPLE*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
