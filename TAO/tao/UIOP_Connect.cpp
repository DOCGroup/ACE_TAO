// $Id$

#if !defined(ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/debug.h"
#include "tao/GIOP.h"
#include "tao/GIOP_Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/POA.h"
#include "tao/CDR.h"

#include "tao/UIOP_Connect.h"
#include "tao/UIOP_Transport.h"

#if !defined (__ACE_INLINE__)
# include "tao/UIOP_Connect.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, UIOP_Connect, "$Id$")

TAO_UIOP_Handler_Base::TAO_UIOP_Handler_Base (TAO_ORB_Core *orb_core)
  : TAO_UIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0)
{
}

TAO_UIOP_Handler_Base::TAO_UIOP_Handler_Base (ACE_Thread_Manager *t)
  : TAO_UIOP_SVC_HANDLER (t, 0, 0)
{
}

int
TAO_UIOP_Handler_Base::resume_handler (ACE_Reactor *)
{
  errno = ENOTSUP;
  return -1;
}

// @@ For pluggable protocols, added a reference to
//    the corresponding transport obj.
TAO_UIOP_Server_Connection_Handler::TAO_UIOP_Server_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIOP_Handler_Base (t ? t : TAO_ORB_Core_instance()->thr_mgr ()),
    orb_core_ (TAO_ORB_Core_instance ())
{
  uiop_transport_ = new TAO_UIOP_Server_Transport (this);
}

// @@ For pluggable protocols, added a reference to the
//    corresponding transport obj.
TAO_UIOP_Server_Connection_Handler::TAO_UIOP_Server_Connection_Handler (TAO_ORB_Core *orb_core)
  : TAO_UIOP_Handler_Base (orb_core),
    orb_core_ (orb_core)
{
  uiop_transport_ = new TAO_UIOP_Server_Transport (this);
}

TAO_Transport *
TAO_UIOP_Server_Connection_Handler::transport (void)
{
  // @@ For now return nothing since all is not in place!
  return uiop_transport_;
}

int
TAO_UIOP_Server_Connection_Handler::open (void*)
{
  // Called by the <Strategy_Acceptor> when the handler is completely
  // connected.
  ACE_UNIX_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

#if !defined (ACE_LACKS_SOCKET_BUFSIZ)
  int sndbufsize =
    this->orb_core_->orb_params ()->sock_sndbuf_size ();
  int rcvbufsize =
    this->orb_core_->orb_params ()->sock_rcvbuf_size ();

  if (this->peer ().set_option (SOL_SOCKET,
                                SO_SNDBUF,
                                ACE_reinterpret_cast (void *, &sndbufsize),
                                sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->peer ().set_option (SOL_SOCKET,
                                     SO_RCVBUF,
                                    ACE_reinterpret_cast (void *, &rcvbufsize),
                                     sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return -1;
#endif /* !ACE_LACKS_SOCKET_BUFSIZ */

  (void) this->peer ().enable (ACE_CLOEXEC);
  // Set the close-on-exec flag for that file descriptor. If the
  // operation fails we are out of luck (some platforms do not support
  // it and return -1).

  char client[MAXPATHLEN + 1];

  addr.addr_to_string (client, sizeof (client));

  if (TAO_orbdebug)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) connection from client %s\n",
                client));
  return 0;
}

int
TAO_UIOP_Server_Connection_Handler::activate (long flags,
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
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "(%P|%t) TAO_UIOP_Server_Connection_Handler::activate"
                 " %d threads, flags = %d\n",
                 n_threads,
                 flags,
                 THR_BOUND));

  return TAO_UIOP_SVC_HANDLER::activate (flags,
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
TAO_UIOP_Server_Connection_Handler::handle_close (ACE_HANDLE handle,
                                             ACE_Reactor_Mask rm)
{
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "(%P|%t) TAO_UIOP_Server_Connection_Handler::handle_close"
                 " (%d, %d)\n",
                 handle,
                 rm));

  return TAO_UIOP_SVC_HANDLER::handle_close (handle, rm);
}

int
TAO_UIOP_Server_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e.,
  // turned into an active object.  Presumably, activation spawns a
  // thread with this method as the "worker function".
  int result = 0;

  // Inheriting the ORB_Core stuff from the parent thread.  WARNING:
  // this->orb_core_ is *not* the same as TAO_ORB_Core_instance(),
  // this thread was just created and we are in fact *initializing*
  // the ORB_Core based on the resources of the ORB that created
  // us....

  TAO_ORB_Core *tss_orb_core = TAO_ORB_Core_instance ();
  tss_orb_core->inherit_from_parent_thread (this->orb_core_);

  // We need to change this->orb_core_ so it points to the TSS ORB
  // Core, but we must preserve the old value
  TAO_ORB_Core* old_orb_core = this->orb_core_;
  this->orb_core_ = tss_orb_core;

  if (TAO_orbdebug)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) TAO_UIOP_Server_Connection_Handler::svc begin\n"));

  // Here we simply synthesize the "typical" event loop one might find
  // in a reactive handler, except that this can simply block waiting
  // for input.

  while ((result = handle_input ()) >= 0)
    continue;

  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "(%P|%t) TAO_UIOP_Server_Connection_Handler::svc end\n"));

  this->orb_core_ = old_orb_core;

  return result;
}

// Handle processing of the request residing in <input>, setting
// <response_required> to zero if the request is for a oneway or
// non-zero if for a two-way and <output> to any necessary response
// (including errors).  In case of errors, -1 is returned and
// additional information carried in <TAO_IN_ENV>.
// The request ID is needed by handle_input. It is passed back
// as reference.

int
TAO_UIOP_Server_Connection_Handler::handle_message (TAO_InputCDR &input,
                                                    TAO_OutputCDR &output,
                                                    CORBA::Boolean &response_required,
                                                    CORBA::ULong &request_id,
                                                    CORBA::Environment &ACE_TRY_ENV)
{
  // This will extract the request header, set <response_required> as
  // appropriate.
  GIOP_ServerRequest request (input,
                              output,
                              this->orb_core_,
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // The request_id_ field in request will be 0 if something went
  // wrong before it got a chance to read it out.
  request_id = request.request_id ();

  response_required = request.response_expected ();

  const CORBA::Octet *object_key = request.object_key ().get_buffer ();

#if !defined (TAO_NO_IOR_TABLE)
  if (ACE_OS::memcmp (object_key,
		      &TAO_POA::objectkey_prefix[0],
		      TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
    {
      ACE_CString object_id (ACE_reinterpret_cast (const char *, object_key),
			     TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE,
			     0,
			     0);

      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG,
		    "Simple Object key %s. Doing the Table Lookup ...\n",
		    object_id.c_str ()));

      CORBA::Object_ptr object_reference;

      // Do the Table Lookup.
      int status =
	this->orb_core_->orb ()->_tao_find_in_IOR_table (object_id,
							 object_reference);

      // If ObjectID not in table or reference is nil raise OBJECT_NOT_EXIST.

      if (CORBA::is_nil (object_reference) || status == -1)
	ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), -1);

      // ObjectID present in the table with an associated NON-NULL reference.
      // Throw a forward request exception.

      CORBA::Object_ptr dup = CORBA::Object::_duplicate (object_reference);

      ACE_THROW_RETURN (PortableServer::ForwardRequest (dup), -1);
    }

#endif

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

  this->orb_core_->object_adapter ()->dispatch_servant (request.object_key (),
                                                        request,
                                                        0,
                                                        ACE_TRY_ENV);
  // NEED TO CHECK FOR any errors present in <env> and set the return
  // code appropriately.
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_UIOP_Server_Connection_Handler::handle_locate (TAO_InputCDR &input,
                                                   TAO_OutputCDR &output,
                                                   CORBA::Boolean &response_required,
                                                   CORBA::ULong &request_id,
                                                   CORBA::Environment &env)
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_LOCATE_START);

  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_LocateRequestHeader locateRequestHeader;

  env.clear ();
  if (locateRequestHeader.init (input, env) == 0)
    {
      request_id = locateRequestHeader.request_id;
      response_required = 0;
      return -1;
    }

  // Copy the request ID to be able to respond in case of an
  // exception.
  request_id = locateRequestHeader.request_id;
  response_required = 1;

  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
  TAO_OutputCDR dummy_output (repbuf, sizeof(repbuf));
  // This output CDR is not used!

  TAO_ObjectKey tmp_key (locateRequestHeader.object_key.length (),
                         locateRequestHeader.object_key.length (),
                         locateRequestHeader.object_key.get_buffer (),
                         0);

  CORBA::Object_var forward_location_var;
  TAO_GIOP_LocateStatusType status;

// #if !defined (TAO_NO_IOR_TABLE)
//   if (ACE_OS::memcmp (tmp_key.get_buffer (),
// 		      &TAO_POA::objectkey_prefix[0],
// 		      TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) == 0)
//     {
//       ACE_DEBUG ((LM_DEBUG,
// 		  "TAO Object Key Prefix found in the object key.\n"));


//       // Do the Table Lookup. Raise a location forward exception or
//       // a non-exist exception.

//       // CORBA::Object_ptr object_reference;
//       // int s =
//       //     table->lookup (request.object_key (),
//       //                    object_reference);
//       // if (s == -1)
//       //   {
//       //      status = TAO_GIOP_UNKNOWN_OBJECT;
//       //   }
//       // else
//       //   {
//       //      status = TAO_GIOP_OBJECT_FORWARD;
//       //      forward_location_var =
//       //          CORBA::Object::_duplicate (object_reference);
//       //    }
//     }
//   // else
//   //   {
// #endif

  // this->handle_locate_i (....);

  GIOP_ServerRequest serverRequest (locateRequestHeader.request_id,
                                    response_required,
                                    tmp_key,
                                    "_non_existent",
                                    dummy_output,
                                    this->orb_core_,
                                    env);

  this->orb_core_->object_adapter ()->dispatch_servant (serverRequest.object_key (),
                                                        serverRequest,
                                                        0,
                                                        env);

  if (serverRequest.exception_type () == TAO_GIOP_NO_EXCEPTION
      && env.exception () == 0)
    {
      // we got no exception, so the object is here
      status = TAO_GIOP_OBJECT_HERE;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) handle_locate() : found\n"));
    }
  else if (serverRequest.exception_type () != TAO_GIOP_NO_EXCEPTION)
    {
      forward_location_var = serverRequest.forward_location ();
      if (!CORBA::is_nil (forward_location_var.in ()))
        {
          status = TAO_GIOP_OBJECT_FORWARD;
          ACE_DEBUG ((LM_DEBUG,
                      "handle_locate has been called: forwarding\n"));
        }
      else
        {
          // Normal exception, so the object is not here
          status = TAO_GIOP_UNKNOWN_OBJECT;
          ACE_DEBUG ((LM_DEBUG,
                      "handle_locate has been called: not here\n"));
        }

      // The locate_servant call might have thrown an exception but we
      // don't want to marshal it because it is no failure.  The
      // proper Locacte_Reply will tell the client what is going on.

      // Remove the exception
      env.clear ();
    }
  else
    {
      // Try to narrow to ForwardRequest
      PortableServer::ForwardRequest_ptr forward_request_ptr =
        PortableServer::ForwardRequest::_narrow (env.exception ());

      // If narrowing of exception succeeded
      if (forward_request_ptr != 0)
        {
          status = TAO_GIOP_OBJECT_FORWARD;
          forward_location_var = forward_request_ptr->forward_reference;
          ACE_DEBUG ((LM_DEBUG,
                      "handle_locate has been called: forwarding\n"));
        }
      else
        {
          // Normal exception, so the object is not here
          status = TAO_GIOP_UNKNOWN_OBJECT;
          ACE_DEBUG ((LM_DEBUG,
                      "handle_locate has been called: not here\n"));
        }

      // the locate_servant call might have thrown an exception but we
      // don't want to marshal it because it is no failure.  The
      // proper Locacte_Reply will tell the client what is going on.

      // Remove the exception
      env.clear ();
    }

#if !defined (TAO_NO_IOR_TABLE)
  //  }
#endif

  // Create the response.
  TAO_GIOP::start_message (TAO_GIOP::LocateReply, output,
                           this->orb_core_);
  output.write_ulong (locateRequestHeader.request_id);
  output.write_ulong (status);

  if (status == TAO_GIOP_OBJECT_FORWARD)
    {
      CORBA::Object_ptr object_ptr = forward_location_var.in ();
      if ((output << object_ptr) == 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Server_Connection_Handler::handle_locate - "
                          "error marshaling forwarded reference\n"));
            }
          response_required = 0;
          return -1;
        }
    }

  return 0;
}

void
TAO_UIOP_Server_Connection_Handler::send_response (TAO_OutputCDR &output)
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_START);

  TAO_GIOP::send_request (this->uiop_transport_,
                          output,
                          this->orb_core_);
}

// This method is designed to return system exceptions to the caller.

void
TAO_UIOP_Server_Connection_Handler::send_error (CORBA::ULong request_id,
                                           CORBA::Exception *x)
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_START);

  // The request_id is going to be not 0, if it was sucessfully read
  if (request_id != 0)
    {
      // Create a new output CDR stream
      TAO_OutputCDR output;

      // Construct a REPLY header.
      TAO_GIOP::start_message (TAO_GIOP::Reply, output,
                               this->orb_core_);

      // A new try/catch block, but if something goes wrong now we
      // have no hope, just abort.
      ACE_TRY_NEW_ENV
        {
          // create and write a dummy context
          TAO_GIOP_ServiceContextList resp_ctx;
          resp_ctx.length (0);
          output << resp_ctx;

          // Write the request ID
          output.write_ulong (request_id);

          // @@ TODO This is the place to conditionally compile
          // forwarding. It certainly seems easy to strategize too,
          // just invoke an strategy to finish marshalling the
          // response.

          // Now we check for Forwarding ***************************

          // Try to narrow to ForwardRequest
          PortableServer::ForwardRequest_ptr forward_request_ptr =
            PortableServer::ForwardRequest::_narrow (x);

          // If narrowing of exception succeeded
          if (forward_request_ptr != 0
              && !CORBA::is_nil (forward_request_ptr->forward_reference.in ()))
            {
              // write the reply_status
              output.write_ulong (TAO_GIOP_LOCATION_FORWARD);

              // write the object reference into the stream
              CORBA::Object_ptr object_ptr =
                forward_request_ptr->forward_reference.in();

              output << object_ptr;
            }
          // end of the forwarding code ****************************
          else
            {
              // Write the exception
              CORBA::TypeCode_ptr except_tc = x->_type ();

              CORBA::exception_type extype = CORBA::USER_EXCEPTION;
              if (CORBA::SystemException::_narrow (x) != 0)
                extype = CORBA::SYSTEM_EXCEPTION;

              // write the reply_status
              output.write_ulong (TAO_GIOP::convert_CORBA_to_GIOP_exception (extype));

              // write the actual exception
              output.encode (except_tc, x, 0, ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCH (CORBA_Exception, ex)
        {
          // now we know, that while handling the error an other error
          // happened -> no hope, close connection.

          // close the handle
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) closing conn %d after fault %p\n",
                      this->peer ().get_handle (),
                      "TAO_UIOP_Server_Connection_Handler::send_error"));
          this->handle_close ();
          return;
        }
      ACE_ENDTRY;

      // hand it to the next lower layer
      TAO_GIOP::send_request (this->uiop_transport_, output, this->orb_core_);
    }
}

int
TAO_UIOP_Server_Connection_Handler::handle_input (ACE_HANDLE)
{
  // CJCXXX The tasks of this method should change to something like
  // the following:
  // 1. call into GIOP to pull off the header
  // 2. construct a complete request
  // 3. dispatch that request and return any required reply and errors

  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_START);

  // @@ TODO This should take its memory from a specialized
  // allocator. It is better to use a message block than a on stack
  // buffer because we cannot minimize memory copies in that case.
  TAO_InputCDR input (this->orb_core_->create_input_cdr_data_block (ACE_CDR::DEFAULT_BUFSIZE),
                      TAO_ENCAP_BYTE_ORDER);

  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf, '\0', sizeof (repbuf));
#endif /* ACE_HAS_PURIFY */
  TAO_OutputCDR output (repbuf, sizeof(repbuf),
                        TAO_ENCAP_BYTE_ORDER,
                        this->orb_core_->output_cdr_buffer_allocator (),
                        this->orb_core_->output_cdr_buffer_allocator ());

  int result = 0;
  int error_encountered = 0;
  CORBA::Boolean response_required = 0;
  CORBA::ULong request_id = 0;

  ACE_TRY_NEW_ENV
    {
      // Try to recv a new request.
      TAO_GIOP::Message_Type type =
        TAO_GIOP::recv_request (this->uiop_transport_, input, this->orb_core_);

      //      TAO_MINIMAL_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_RECEIVE_REQUEST_END);

      // Check to see if we've been cancelled cooperatively.
      if (this->orb_core_->orb ()->should_shutdown () != 0)
        error_encountered = 1;
      else
        {
          switch (type)
            {
            case TAO_GIOP::Request:
              // Message was successfully read, so handle it.  If we
              // encounter any errors, <output> will be set
              // appropriately by the called code, and -1 will be
              // returned.
              if (this->handle_message (input,
                                        output,
                                        response_required,
                                        request_id,
                                        ACE_TRY_ENV) == -1)
                error_encountered = 1;
              ACE_TRY_CHECK;
              break;

            case TAO_GIOP::LocateRequest:
              if (this->handle_locate (input,
                                       output,
                                       response_required,
                                       request_id,
                                       ACE_TRY_ENV) == -1)
                error_encountered = 1;
              ACE_TRY_CHECK;
              break;

            case TAO_GIOP::EndOfFile:
              // Got a EOF
              result = -1;
              break;

              // These messages should never be sent to the server;
              // it's an error if the peer tries.  Set the environment
              // accordingly, as it's not yet been reported as an
              // error.
            case TAO_GIOP::Reply:
            case TAO_GIOP::LocateReply:
            case TAO_GIOP::CloseConnection:
            default:                                    // Unknown message
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) Illegal message received by server\n"));
              ACE_TRY_THROW (CORBA::COMM_FAILURE ());
              // NOTREACHED

            case TAO_GIOP::CommunicationError:
            case TAO_GIOP::MessageError:
              // Here, MessageError can either mean condition for
              // GIOP::MessageError happened or a GIOP message was
              // not successfully received.  Sending back of
              // GIOP::MessageError is handled in TAO_GIOP::parse_header.
              error_encountered = 1;
              break;
            }
        }
    }
  ACE_CATCHANY                  // Only CORBA exceptions are caught here.
    {
      if (response_required)
        this->send_error (request_id, &ex);
      else
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) exception thrown "
                          "but client is not waiting a response\n"));
              ACE_TRY_ENV.print_exception ("");
            }

          // It is unfotunate that an exception (probably a system
          // exception) was thrown by the upcall code (even by the
          // user) when the client was not expecting a response.
          // However, in this case, we cannot close the connection
          // down, since it really isn't the client's fault.
          result = 0;
        }
      return result;
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // @@ TODO some c++ exception or another, but what do we do with
      //    it?
      // We are supposed to map it into a CORBA::UNKNOWN exception.
      // BTW, this cannot be detected if using the <env> mapping.
      //   If we have native exceptions but no support for them
      //   in the ORB we should still be able to catch it.
      //   If we don't have native exceptions it couldn't have been
      //   raised in the first place!

      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) closing conn %d after fault %p\n",
                  this->peer().get_handle (),
                  "TAO_UIOP_Server_Connection_Handler::handle_input"));
      //      this->handle_close ();
      return -1;
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  if (response_required)
    {
      if (!error_encountered)
        this->send_response (output);
      else
        {
          // No exception but some kind of error, yet a response is
          // required.
          if (TAO_orbdebug)
            ACE_ERROR ((LM_ERROR,
                        "TAO: (%P|%t) %s: closing conn, no exception, "
                        "but expecting response\n",
                        "TAO_UIOP_Server_Connection_Handler::handle_input"));
          //          this->handle_close ();
          return -1;
        }
    }
  else if (error_encountered)
    {
      // No exception, no response expected, but an error ocurred,
      // close the socket.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_ERROR,
                    "TAO: (%P|%t) %s: closing conn, no exception, "
                    "but expecting response\n",
                    "TAO_UIOP_Server_Connection_Handler::handle_input"));
      //      this->handle_close ();
      return -1;
    }

  //  TAO_MINIMAL_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_END);

  return result;
}

// @@ For pluggable protocols, added a reference to the corresponding
//    transport obj.
TAO_UIOP_Client_Connection_Handler::TAO_UIOP_Client_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIOP_Handler_Base (t == 0 ? TAO_ORB_Core_instance ()->thr_mgr () : t),
    expecting_response_ (0),
    input_available_ (0)
{
  uiop_transport_ = new TAO_UIOP_Client_Transport(this);
}

// @@ Need to get rid of the Transport Objects!
TAO_UIOP_Client_Connection_Handler::~TAO_UIOP_Client_Connection_Handler (void)
{
  delete this->uiop_transport_;
  this->uiop_transport_ = 0;
}

TAO_Transport *
TAO_UIOP_Client_Connection_Handler::transport (void)
{
  return this->uiop_transport_;
}

int
TAO_UIOP_Client_Connection_Handler::open (void *)
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
                                ACE_reinterpret_cast (void *, &sndbufsize),
                                sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->peer ().set_option (SOL_SOCKET,
                                     SO_RCVBUF,
                                    ACE_reinterpret_cast (void *, &rcvbufsize),
                                     sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return -1;
#endif /* ACE_LACKS_SOCKET_BUFSIZ */

  (void) this->peer ().enable (ACE_CLOEXEC);
  // Set the close-on-exec flag for that file descriptor. If the
  // operation fails we are out of luck (some platforms do not support
  // it and return -1).

  // For now, we just return success
  return 0;
}

int
TAO_UIOP_Client_Connection_Handler::send_request (TAO_ORB_Core *,
                                                  TAO_OutputCDR &,
                                                  int)
{
  errno = ENOTSUP;
  return -1;
}

int
TAO_UIOP_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  errno = ENOTSUP;
  return -1;
}

int
TAO_UIOP_Client_Connection_Handler::check_unexpected_data (void)
{
  // We're a client, so we're not expecting to see input.  Still we
  // better check what it is!
  char ignored;
  ssize_t ret = this->peer().recv (&ignored,
                                   sizeof ignored,
                                   MSG_PEEK);
  switch (ret)
    {
    case 0:
    case -1:
      // 0 is a graceful shutdown
      // -1 is a somewhat ugly shutdown
      //
      // Both will result in us returning -1 and this connection
      // getting closed
      //
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_WARNING,
                    "UIOP_Client_Connection_Handler::handle_input: "
                    "closing connection on fd %d\n",
                    this->peer().get_handle ()));
      break;

    case 1:
      //
      // @@ Fix me!!
      //
      // This should be the close connection message.  Since we don't
      // handle this yet, log an error, and close the connection.
      ACE_ERROR ((LM_WARNING,
                  "UIOP_Client_Connection_Handler::handle_input received "
                  "input while not expecting a response; "
                  "closing connection on fd %d\n",
                  this->peer().get_handle ()));
      break;
    }

  // We're not expecting input at this time, so we'll always
  // return -1 for now.
  return -1;
}

int
TAO_UIOP_Client_Connection_Handler::handle_close (ACE_HANDLE handle,
                                                  ACE_Reactor_Mask rm)
{
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "(%P|%t) TAO_UIOP_Client_Connection_Handler::handle_close"
                 " (%d, %d)\n",
                 handle,
                 rm));

  if (this->recycler ())
    this->recycler ()->mark_as_closed (this->recycling_act ());

  // Deregister this handler with the ACE_Reactor.
  if (this->reactor ())
    {
      ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK |
        ACE_Event_Handler::DONT_CALL;

      // Make sure there are no timers.
      this->reactor ()->cancel_timer (this);

      // Remove self from reactor.
      this->reactor ()->remove_handler (this, mask);
    }

  this->peer ().close ();

  return 0;
}

int
TAO_UIOP_Client_Connection_Handler::close (u_long)
{
  this->destroy ();

  return 0;
}

// ****************************************************************

TAO_RW_UIOP_Client_Connection_Handler::TAO_RW_UIOP_Client_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIOP_Client_Connection_Handler (t)
{
}

TAO_RW_UIOP_Client_Connection_Handler::~TAO_RW_UIOP_Client_Connection_Handler (void)
{
}

int
TAO_RW_UIOP_Client_Connection_Handler::send_request (TAO_ORB_Core* orb_core,
                                                TAO_OutputCDR &stream,
                                                int is_twoway)
{
  // TAO_FUNCTION_PP_TIMEPROBE (TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_START);

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

  // Send the request
  int success =
    ACE_static_cast (int, TAO_GIOP::send_request (this->uiop_transport_,
                                                  stream,
                                                  orb_core));

  //  TAO_MINIMAL_TIMEPROBE (GIOP_SEND_REQUEST_RETURN);

  if (!success)
    return -1;

  return 0;
}

int
TAO_RW_UIOP_Client_Connection_Handler::resume_handler (ACE_Reactor *)
{
  // Since we don't suspend, we don't have to resume.
  return 0;
}

// ****************************************************************

TAO_ST_UIOP_Client_Connection_Handler::TAO_ST_UIOP_Client_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIOP_Client_Connection_Handler (t)
{
}

TAO_ST_UIOP_Client_Connection_Handler::~TAO_ST_UIOP_Client_Connection_Handler (void)
{
}

int
TAO_ST_UIOP_Client_Connection_Handler::open (void *something)
{
  int result = TAO_UIOP_Client_Connection_Handler::open (something);

  if (result != 0)
    return result;

  // Now we must register ourselves with the reactor for input events
  // which will detect GIOP Reply messages and EOF conditions.
  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();
  return r->register_handler (this,
                              ACE_Event_Handler::READ_MASK);
}

// @@ this seems odd that the connection handler would call methods in the
//    GIOP object.  Some of this mothod's functionality should be moved
//    to GIOP. fredk
int
TAO_ST_UIOP_Client_Connection_Handler::send_request (TAO_ORB_Core* orb_core,
                                                TAO_OutputCDR &stream,
                                                int is_twoway)
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_START);

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

  // Send the request
  int success  = (int) TAO_GIOP::send_request (this->uiop_transport_,
                                               stream,
                                               orb_core);
  //  TAO_MINIMAL_TIMEPROBE (GIOP_SEND_REQUEST_RETURN);

  if (!success)
    return -1;

  if (is_twoway)
    {
      // Set the state so that we know we're looking for a response.
      this->expecting_response_ = 1;

      // Go into a loop, waiting until it's safe to try to read
      // something on the socket.  The handle_input() method doesn't
      // actualy do the read, though, proper behavior based on what is
      // read may be different if we're not using GIOP above here.
      // So, we leave the reading of the response to the caller of
      // this method, and simply insure that this method doesn't
      // return until such time as doing a recv() on the socket would
      // actually produce fruit.
      ACE_Reactor *r = orb_core->reactor ();

      int ret = 0;

      while (ret != -1 && ! this->input_available_)
        ret = r->handle_events ();

      this->input_available_ = 0;
      // We can get events now, b/c we want them!

      // We're no longer expecting a response!
      this->expecting_response_ = 0;
    }

  return 0;
}

int
TAO_ST_UIOP_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  int retval = 0;

  if (this->expecting_response_)
    {
      this->input_available_ = 1;
      // Temporarily remove ourself from notification so that if
      // another sub event loop is in effect still waiting for its
      // response, it doesn't spin tightly gobbling up CPU.
      TAO_ORB_Core_instance ()->reactor ()->suspend_handler (this);
    }
  else
    retval = this->check_unexpected_data ();

  return retval;
}

int
TAO_ST_UIOP_Client_Connection_Handler::resume_handler (ACE_Reactor *reactor)
{
  return reactor->resume_handler (this);
}

// ****************************************************************

TAO_MT_UIOP_Client_Connection_Handler::TAO_MT_UIOP_Client_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIOP_Client_Connection_Handler (t),
    calling_thread_ (ACE_OS::NULL_thread),
    cond_response_available_ (0),
    orb_core_ (0)
{
}

TAO_MT_UIOP_Client_Connection_Handler::~TAO_MT_UIOP_Client_Connection_Handler (void)
{
  delete this->cond_response_available_;
}

int
TAO_MT_UIOP_Client_Connection_Handler::open (void *something)
{
  int result = TAO_UIOP_Client_Connection_Handler::open (something);

  if (result != 0)
    return result;

  // Now we must register ourselves with the reactor for input events
  // which will detect GIOP Reply messages and EOF conditions.
  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();
  return r->register_handler (this,
                              ACE_Event_Handler::READ_MASK);
}

ACE_SYNCH_CONDITION*
TAO_MT_UIOP_Client_Connection_Handler::cond_response_available (TAO_ORB_Core* orb_core)
{
  // @@ TODO This condition variable should per-ORB-per-thread, not
  // per-connection, it is a waste to have more than one of this in
  // the same thread.
  if (this->cond_response_available_ == 0)
    {
      ACE_NEW_RETURN (this->cond_response_available_,
                      ACE_SYNCH_CONDITION (orb_core->leader_follower_lock ()),
                      0);
    }
  return this->cond_response_available_;
}

int
TAO_MT_UIOP_Client_Connection_Handler::send_request (TAO_ORB_Core *orb_core,
                                                     TAO_OutputCDR &stream,
                                                     int is_twoway)
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_START);

  // Save the ORB_Core for the handle_input callback...
  this->orb_core_ = orb_core;

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
      // Send the request
      int success  = (int) TAO_GIOP::send_request (this->uiop_transport_,
                                                   stream,
                                                   this->orb_core_);

      //      TAO_MINIMAL_TIMEPROBE (GIOP_SEND_REQUEST_RETURN);

      if (!success)
        return -1;
    }
  else // is_twoway
    {
      ACE_Reactor *r = this->orb_core_->reactor ();

      if (this->reactor () != r)
        {
          ACE_Reactor_Mask mask =
            ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
          this->reactor ()->remove_handler (this, mask);

          r->register_handler (this,
                               ACE_Event_Handler::READ_MASK);
        }

      if (this->orb_core_->leader_follower_lock ().acquire() == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::send_request: "
                           "Failed to get the lock.\n"),
                          -1);

      // Set the state so that we know we're looking for a response.
      this->expecting_response_ = 1;
      // remember in which thread the client connection handler was running
      this->calling_thread_ = ACE_Thread::self ();

      // Send the request
      int success = (int) TAO_GIOP::send_request (this->uiop_transport_,
                                                  stream,
                                                  orb_core);

      //      TAO_MINIMAL_TIMEPROBE (GIOP_SEND_REQUEST_RETURN);

      if (!success)
        {
          this->orb_core_->leader_follower_lock ().release ();
          return -1;
        }

      // check if there is a leader, but the leader is not us
      if (this->orb_core_->leader_available () &&
          !this->orb_core_->I_am_the_leader_thread ())
        {
          // wait as long as no input is available and/or
          // no leader is available
          while (!this->input_available_ &&
                 this->orb_core_->leader_available ())
            {
              ACE_SYNCH_CONDITION* cond =
                this->cond_response_available (orb_core);
              if (this->orb_core_->add_follower (cond) == -1)
                ACE_ERROR ((LM_ERROR,
                            "(%P|%t) TAO_UIOP_Client_Connection_Handler::send_request: "
                            "Failed to add a follower thread\n"));
              cond->wait ();
            }
          // now somebody woke us up to become a leader or to handle
          // our input. We are already removed from the follower queue
          if (this->input_available_)
            {
              // there is input waiting for me
              if (this->orb_core_->leader_follower_lock ().release () == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%P|%t) TAO_UIOP_Client_Connection_Handler::send_request: "
                                   "Failed to release the lock.\n"),
                                  -1);
              // The following variables are safe, because we are not
              // registered with the reactor any more.
              this->input_available_ = 0;
              this->expecting_response_ = 0;
              this->calling_thread_ = ACE_OS::NULL_thread;
              return 0;
            }
        }

      // Become a leader, because there is no leader or we have to
      // update to a leader or we are doing nested upcalls in this
      // case we do increase the refcount on the leader in
      // TAO_ORB_Core.

      this->orb_core_->set_leader_thread ();
      // this might increase the recount of the leader

      if (this->orb_core_->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::send_request: "
                           "Failed to release the lock.\n"),
                          -1);

      r->owner (ACE_Thread::self ());

      int ret = 0;

      while (ret != -1 && !this->input_available_)
        ret = r->handle_events ();

      if (ret == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::send_request: "
                           "handle_events failed.\n"),
                          -1);

      // Wake up the next leader, we cannot do that in handle_input,
      // because the woken up thread would try to get into
      // handle_events, which is at the time in handle_input still
      // occupied.

      if (this->orb_core_->unset_leader_wake_up_follower () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::send_request: "
                           "Failed to unset the leader and wake up a new follower.\n"),
                          -1);
      // Make use reusable
      this->input_available_ = 0;
      this->expecting_response_ = 0;
      this->calling_thread_ = ACE_OS::NULL_thread;
    }

  return 0;
}

int
TAO_MT_UIOP_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  if (this->orb_core_ == 0)
    this->orb_core_ = TAO_ORB_Core_instance ();

  if (this->orb_core_->leader_follower_lock ().acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) TAO_UIOP_Client_Connection_Handler::handle_input: "
                       "Failed to get the lock.\n"),
                      -1);

  if (!this->expecting_response_)
    {
      // we got something, but did not want
      // @@ wake up an other thread, we are lost

      if (this->orb_core_->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::handle_input: "
                           "Failed to release the lock.\n"),
                          -1);
      return this->check_unexpected_data ();
    }

  if (ACE_OS::thr_equal (this->calling_thread_,
                         ACE_Thread::self ()))
    {
      // We are now a leader getting its response.
      this->input_available_ = 1;

      if (this->orb_core_->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::handle_input: "
                           "Failed to release the lock.\n"),
                          -1);
      this->orb_core_->reactor ()->suspend_handler (this);
      // resume_handler is called in TAO_GIOP_Invocation::invoke
      return 0;
    }
  else
    {
      // We are a leader, which got a response for one of the
      // followers, which means we are now a thread running the wrong
      // Client_Connection_Handler

      // At this point we might fail to remove the follower, because
      // it has been already chosen to become the leader, so it is
      // awake and will get this too.
      ACE_SYNCH_CONDITION* cond =
        this->cond_response_available (this->orb_core_);

      this->orb_core_->remove_follower (cond);

      if (this->orb_core_->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::handle_input: "
                           "Failed to release the lock.\n"),
                          -1);

      this->orb_core_->reactor ()->suspend_handler (this);
      // We should wake suspend the thread before we wake him up.
      // resume_handler is called in TAO_GIOP_Invocation::invoke

      // @@ TODO (Michael): We might be able to optimize this in
      // doing the suspend_handler as last thing, but I am not sure
      // if a race condition would occur.

      if (this->orb_core_->leader_follower_lock ().acquire () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::handle_input: "
                           "Failed to acquire the lock.\n"),
                          -1);
      // The thread was already selected to become a leader, so we
      // will be called again.
      this->input_available_ = 1;
      cond->signal ();

      if (this->orb_core_->leader_follower_lock ().release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_UIOP_Client_Connection_Handler::handle_input: "
                           "Failed to release the lock.\n"),
                          -1);
      return 0;
    }
}

int
TAO_MT_UIOP_Client_Connection_Handler::resume_handler (ACE_Reactor *reactor)
{
  return reactor->resume_handler (this);
}

// ****************************************************************

#define TAO_UIOP_SVC_TUPLE ACE_Svc_Tuple<TAO_UIOP_Client_Connection_Handler>
#define UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR ACE_Refcounted_Hash_Recyclable<ACE_UNIX_Addr>

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>;
template class UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR;
template class TAO_UIOP_SVC_TUPLE;
template class ACE_Map_Manager<int, TAO_UIOP_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, TAO_UIOP_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<int, TAO_UIOP_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int, TAO_UIOP_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int, TAO_UIOP_SVC_TUPLE*>;

template class ACE_Hash_Map_Manager<ACE_UNIX_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Manager_Ex<ACE_UNIX_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_UNIX_Addr>, ACE_Equal_To<ACE_UNIX_Addr>, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Entry<ACE_UNIX_Addr, TAO_Object_Adapter *>;
template class ACE_Hash<ACE_UNIX_Addr>;
template class ACE_Equal_To<ACE_UNIX_Addr>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_UNIX_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_UNIX_Addr>, ACE_Equal_To<ACE_UNIX_Addr>, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Iterator<ACE_UNIX_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Iterator_Ex<ACE_UNIX_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_UNIX_Addr>, ACE_Equal_To<ACE_UNIX_Addr>, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_UNIX_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_UNIX_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_UNIX_Addr>, ACE_Equal_To<ACE_UNIX_Addr>, TAO_Collocation_Table_Lock>;

template class ACE_Hash_Map_Entry<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *>;
template class ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>;
template class ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>;
template class ACE_Hash_Map_Manager<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR
#pragma instantiate TAO_UIOP_SVC_TUPLE
#pragma instantiate ACE_Map_Manager<int, TAO_UIOP_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, TAO_UIOP_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<int, TAO_UIOP_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int, TAO_UIOP_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int, TAO_UIOP_SVC_TUPLE*>

#pragma instantiate ACE_Hash_Map_Manager<ACE_UNIX_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_UNIX_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_UNIX_Addr>, ACE_Equal_To<ACE_UNIX_Addr>, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Entry<ACE_UNIX_Addr, TAO_Object_Adapter *>
#pragma instantiate ACE_Hash<ACE_UNIX_Addr>
#pragma instantiate ACE_Equal_To<ACE_UNIX_Addr>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_UNIX_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_UNIX_Addr>, ACE_Equal_To<ACE_UNIX_Addr>, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_UNIX_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_UNIX_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_UNIX_Addr>, ACE_Equal_To<ACE_UNIX_Addr>, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_UNIX_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_UNIX_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_UNIX_Addr>, ACE_Equal_To<ACE_UNIX_Addr>, TAO_Collocation_Table_Lock>

#pragma instantiate ACE_Hash_Map_Entry<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *>
#pragma instantiate ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>
#pragma instantiate ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>
#pragma instantiate ACE_Hash_Map_Manager<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_UIOP_Client_Connection_Handler *, ACE_Hash<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<UIOP_REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
