// $Id$

#if !defined(ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/UIOP_Connect.h"
#include "tao/Timeprobe.h"
#include "tao/UIOP_Transport.h"
#include "tao/debug.h"
#include "tao/GIOP.h"
#include "tao/GIOP_Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/POA.h"
#include "tao/CDR.h"
#include "tao/Wait_Strategy.h"

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

// ****************************************************************

// @@ For pluggable protocols, added a reference to
//    the corresponding transport obj.
TAO_UIOP_Server_Connection_Handler::TAO_UIOP_Server_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIOP_Handler_Base (t ? t : TAO_ORB_Core_instance()->thr_mgr ()),
    orb_core_ (TAO_ORB_Core_instance ()),
    tss_resources_ (TAO_ORB_CORE_TSS_RESOURCES::instance ())
{
  uiop_transport_ = new TAO_UIOP_Server_Transport (this);
}

// @@ For pluggable protocols, added a reference to the
//    corresponding transport obj.
TAO_UIOP_Server_Connection_Handler::TAO_UIOP_Server_Connection_Handler (TAO_ORB_Core *orb_core)
  : TAO_UIOP_Handler_Base (orb_core),
    orb_core_ (orb_core),
    tss_resources_ (TAO_ORB_CORE_TSS_RESOURCES::instance ())
{
  uiop_transport_ = new TAO_UIOP_Server_Transport (this);
}

TAO_UIOP_Server_Connection_Handler::~TAO_UIOP_Server_Connection_Handler (void)
{
  delete uiop_transport_;
  uiop_transport_ = 0;
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

  // Called by the <Strategy_Acceptor> when the handler is completely
  // connected.
  ACE_UNIX_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  char client[MAXPATHLEN + 16];
  addr.addr_to_string (client, sizeof (client));

  if (TAO_debug_level > 0)
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

  // Inheriting the ORB_Core tss stuff from the parent thread.
  this->orb_core_->inherit_from_parent_thread (this->tss_resources_);

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

#if !defined (TAO_NO_IOR_TABLE)

  const CORBA::Octet *object_key = request.object_key ().get_buffer ();

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

#endif /* TAO_NO_IOR_TABLE */

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

#if !defined (TAO_HAS_MINIMUM_CORBA)

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

#endif /* TAO_HAS_MINIMUM_CORBA */

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

#if !defined (TAO_HAS_MINIMUM_CORBA)

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

#endif /* TAO_HAS_MINIMUM_CORBA */

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
                      TAO_ENCAP_BYTE_ORDER,
                      this->orb_core_);

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
  TAO_GIOP_Version version;

  CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ();
  ACE_TRY
    {
      // Try to recv a new request.

      // Init the input message states in Transport.
      this->uiop_transport_->message_size (0);

      // Try to recv a new request.
      TAO_GIOP::Message_Type type =
        TAO_GIOP::recv_message (this->uiop_transport_,
                                input,
                                this->orb_core_,
                                version,
                                1);

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

// ****************************************************************

// @@ For pluggable protocols, added a reference to the corresponding
//    transport obj.
TAO_UIOP_Client_Connection_Handler::TAO_UIOP_Client_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_UIOP_Handler_Base (t == 0 ? TAO_ORB_Core_instance ()->thr_mgr () : t)
{
  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  uiop_transport_ = new TAO_UIOP_Client_Transport(this,
                                                  orb_core);
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

// @@ Should I do something here to enable non-blocking?? (Alex).
// @@ Alex: I don't know if this is the place to do it, but the way to
//    do it is:
//    if (this->peer ().enable (ACE_NONBLOCK) == -1)
//       return -1;
//    Probably we will need to use the transport to decide if it is
//    needed or not.

int
TAO_UIOP_Client_Connection_Handler::open (void *)
{
  // @@ TODO: This flags should be set using the RT CORBA policies...

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

  // Called by the <Strategy_Acceptor> when the handler is completely
  // connected.
  ACE_UNIX_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  char server[MAXHOSTNAMELEN + 16];

  (void) addr.addr_to_string (server, sizeof (server));

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) connection to server <%s> on %d\n",
                server, this->peer ().get_handle ()));

  // Register the handler with the Reactor if necessary.
  return this->transport ()->wait_strategy ()->register_handler ();
}

int
TAO_UIOP_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  // Call the waiter to handle the input.
  return this->transport ()->wait_strategy ()->handle_input ();
}

int
TAO_UIOP_Client_Connection_Handler::handle_close (ACE_HANDLE handle,
                                                  ACE_Reactor_Mask rm)
{
  // @@ Alex: we need to figure out if the transport decides to close
  //    us or something else.  If it is something else (for example
  //    the cached connector trying to make room for other
  //    connections) then we should let the transport know, so it can
  //    in turn take appropiate action (such as sending exceptions to
  //    all waiting reply handlers).

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

  this->transport ()->handle_close ();

  return 0;
}

int
TAO_UIOP_Client_Connection_Handler::close (u_long)
{
  this->destroy ();

  return 0;
}

// ****************************************************************

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
