// Name_Handler.cpp
// @(#)Name_Handler.cpp	1.1	10/18/96

#define ACE_BUILD_SVC_DLL
#include "ace/SString.h"
#include "ace/Set.h"
#include "ace/Get_Opt.h"
#include "ace/Naming_Context.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Name_Request_Reply.h"
#include "Name_Handler.h"

// Simple macro that does bitwise AND -- useful in table lookup
#define ACE_TABLE_MAP(INDEX, MASK) (INDEX & MASK)

// Simple macro that does bitwise AND and then right shift bits by 3
#define ACE_LIST_MAP(INDEX, MASK) (((unsigned long) (INDEX & MASK)) >> 3)

// Forward declaration.
class ACE_Naming_Context;

class ACE_Svc_Export ACE_Name_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //    Product object created by <ACE_Name_Acceptor>.  An
  //    <ACE_Name_Handler> exchanges messages with a <ACE_Name_Proxy>
  //    object on the client-side.
  //
  // = DESCRIPTION
  //   This class is the main workhorse of the <ACE_Name_Server>.  It
  //   handles client requests to bind, rebind, resolve, and unbind
  //   names.  It also schedules and handles timeouts that are used to
  //   support "timed waits."  Clients used timed waits to bound the
  //   amount of time they block trying to get a name.
{
  friend class ACE_Shutup_GPlusPlus;  // Turn off g++ warning
public:
  typedef int (ACE_Name_Handler::*OPERATION) (void);
  // Pointer to a member function of ACE_Name_Handler returning int

  typedef int (ACE_Naming_Context::*LIST_OP) (ACE_PWSTRING_SET &, const ACE_WString &);
  // Pointer to a member function of ACE_Naming_Context returning int

  typedef ACE_Name_Request (ACE_Name_Handler::*REQUEST) (ACE_WString *);
  // Pointer to a member function of ACE_Name_Handler returning ACE_Name_Request

  // = Initialization and termination.

  ACE_Name_Handler (ACE_Thread_Manager * = 0);
  // Default constructor.

  virtual int open (void * = 0);
  // Activate this instance of the <ACE_Name_Handler> (called by the
  // <ACE_Strategy_Acceptor>).

protected:
  // = Helper routines for the operations exported to clients.

  virtual int abandon (void);
  // Give up waiting (e.g., when a timeout occurs or a client shuts
  // down unexpectedly).

  // = Low level routines for framing requests, dispatching
  // operations, and returning replies.

  virtual int recv_request (void);
  // Receive, frame, and decode the client's request.
  
  virtual int dispatch (void);
  // Dispatch the appropriate operation to handle the client's
  // request.
  
  virtual int send_reply (ACE_UINT32 status, ACE_UINT32 errnum = 0);
  // Create and send a reply to the client.

  virtual int send_request (ACE_Name_Request &);
  // Special kind of reply

  // = Demultiplexing hooks.
  virtual ACE_HANDLE get_handle (void) const;
  // Return the underlying <ACE_HANDLE>.

  virtual int handle_input (ACE_HANDLE);
  // Callback method invoked by the <ACE_Reactor> when client events
  // arrive.

  // = Timer hook.
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);
  // Enable clients to limit the amount of time they wait for a name.

private:

  OPERATION op_table_[ACE_Name_Request::MAX_ENUM];
  // Table of pointers to member functions
  
  struct LIST_ENTRY
  {
    LIST_OP operation_;
    // A member function pointer that performs the appropriate
    // operation (e.g., LIST_NAMES, LIST_VALUES, or LIST_TYPES).

    REQUEST request_factory_;
    // A member function pointer that serves as a factory to create a
    // request that is passed back to the client.

    char *description_;
    // Name of the operation we're dispatching (used for debugging).
  };
  
  LIST_ENTRY list_table_[ACE_Name_Request::MAX_LIST];
  // This is the table of pointers to functions that we use to
  // simplify the handling of list requests.

  ACE_Naming_Context *naming_context_;
  // ACE_Naming_Context of this Handler.

  ACE_Name_Request name_request_;
  // Cache request from the client.

  ACE_Name_Request name_request_back_;
  // Special kind of reply for resolve and listnames.

  ACE_Name_Reply name_reply_;
  // Cache reply to the client.

  ACE_INET_Addr addr_;
  // Address of client we are connected with.

  ~ACE_Name_Handler (void);
  // Ensure dynamic allocation...

  int bind (void);
  // Handle binds.

  int rebind (void);
  // Handle rebinds.

  int shared_bind (int rebind);
  // Handle binds and rebinds.

  int resolve (void);
  // Handle find requests.

  int unbind (void);
  // Handle unbind requests.

  int lists (void);
  // Handle LIST_NAMES, LIST_VALUES, and LIST_TYPES requests.

  int lists_entries (void);
  // Handle LIST_NAME_ENTRIES, LIST_VALUE_ENTRIES, and
  // LIST_TYPE_ENTRIES requests.

  ACE_Name_Request name_request (ACE_WString *one_name);
  // Create a name request.

  ACE_Name_Request value_request (ACE_WString *one_name);
  // Create a value request.

  ACE_Name_Request type_request (ACE_WString *one_name);
  // Create a type request.
};

class ACE_Name_Acceptor : public ACE_Strategy_Acceptor<ACE_Name_Handler, ACE_SOCK_ACCEPTOR>
  // = TITLE
  //     This class contains the service-specific methods that can't
  //     easily be factored into the <ACE_Strategy_Acceptor>.
{
public:
  virtual int init (int argc, char *argv[]);
  // Dynamic linking hook.

  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.

  int handle_signal (int, siginfo_t *, ucontext_t *);
  
private:
  ACE_Schedule_All_Reactive_Strategy<ACE_Name_Handler> scheduling_strategy_;
  // The scheduling strategy is designed for Reactive services.
};

int
ACE_Name_Acceptor::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Name_Acceptor::handle_signal got called\n"));
  return 0;
}

int
ACE_Name_Acceptor::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Name_Acceptor::parse_args");

  this->service_port_ = ACE_DEFAULT_SERVER_PORT;

  ACE_LOG_MSG->open ("Name Service");

  ACE_Get_Opt get_opt (argc, argv, "p:", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'p':
	  this->service_port_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	default:
	  ACE_ERROR_RETURN ((LM_ERROR, 
			    "%n:\n[-p server-port]\n%a", 1),
			   -1);
	  break;
	}
    }

  this->service_addr_.set (this->service_port_);
  return 0;
}

int
ACE_Name_Acceptor::init (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Name_Acceptor::init");

  // Use the options hook to parse the command line arguments and set
  // options.
  this->parse_args (argc, argv);

  // Set the acceptor endpoint into listen mode (use the Singleton
  // global Reactor...).
  if (this->open (this->service_addr_, ACE_Service_Config::reactor (),
		  0, 0, 0, 
		  &this->scheduling_strategy_,
		  "Name Server", "ACE naming service") == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p on port %d\n", 
		       "acceptor::open failed", 
		       this->service_addr_.get_port_number ()), -1);

  // Register ourselves to receive SIGINT so we can shutdown
  // gracefully.
  if (this->reactor ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n", 
		      "register_handler (SIGINT)"), -1);

  // Ignore SIGPIPE so that each <SVC_HANDLER> can handle this on its
  // own.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);

  ACE_INET_Addr server_addr;

  // Figure out what port we're really bound to.
  if (this->acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_local_addr"), -1);
    
  ACE_DEBUG ((LM_DEBUG, 
	      "starting up Name Server at port %d on handle %d\n", 
	     server_addr.get_port_number (),
	     this->acceptor ().get_handle ()));
  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Naming
// Server.

ACE_SVC_FACTORY_DEFINE (ACE_Name_Acceptor)

// Default constructor.
ACE_Name_Handler::ACE_Name_Handler (ACE_Thread_Manager *tm)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> (tm)
{
  ACE_TRACE ("ACE_Name_Handler::ACE_Name_Handler");

  // Set up pointers to member functions for the top-level dispatching
  // of client requests.
  this->op_table_[ACE_Name_Request::BIND] = &ACE_Name_Handler::bind;
  this->op_table_[ACE_Name_Request::REBIND] = &ACE_Name_Handler::rebind;
  this->op_table_[ACE_Name_Request::RESOLVE] = &ACE_Name_Handler::resolve; 
  this->op_table_[ACE_Name_Request::UNBIND] = &ACE_Name_Handler::unbind; 
  this->op_table_[ACE_Name_Request::LIST_NAMES] = &ACE_Name_Handler::lists;
  this->op_table_[ACE_Name_Request::LIST_NAME_ENTRIES] = &ACE_Name_Handler::lists_entries;

  // Assign references to simplify subsequent code.
  LIST_ENTRY &list_names_ref = this->list_table_[ACE_LIST_MAP (ACE_Name_Request::LIST_NAMES, 
							       ACE_Name_Request::LIST_OP_MASK)];
  LIST_ENTRY &list_values_ref = this->list_table_[ACE_LIST_MAP (ACE_Name_Request::LIST_VALUES, 
								ACE_Name_Request::LIST_OP_MASK)];
  LIST_ENTRY &list_types_ref = this->list_table_[ACE_LIST_MAP (ACE_Name_Request::LIST_TYPES, 
							       ACE_Name_Request::LIST_OP_MASK)];

  // Set up pointers to member functions for dispatching within the
  // LIST_{NAMES,VALUES,TYPES} methods.

  list_names_ref.operation_ = &ACE_Naming_Context::list_names;
  list_names_ref.request_factory_ = &ACE_Name_Handler::name_request;
  list_names_ref.description_ = "request for LIST_NAMES\n";

  list_values_ref.operation_ = &ACE_Naming_Context::list_values;
  list_values_ref.request_factory_ = &ACE_Name_Handler::value_request;
  list_values_ref.description_ = "request for LIST_VALUES\n";

  list_types_ref.operation_ = &ACE_Naming_Context::list_types;
  list_types_ref.request_factory_ = &ACE_Name_Handler::type_request;
  list_types_ref.description_ = "request for LIST_TYPES\n";
}

// Activate this instance of the ACE_Name_Handler (called by the
// ACE_Name_Acceptor).

/* VIRTUAL */ int 
ACE_Name_Handler::open (void *)
{
  ACE_TRACE ("ACE_Name_Handler::open");

  // Call down to our parent to register ourselves with the Reactor.
  if (ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open (0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  // Instantiate our associated ACE_Naming_Context
  ACE_NEW_RETURN (naming_context_, ACE_Naming_Context (ACE_Naming_Context::NET_LOCAL), -1);

  return 0;
}

// Create and send a reply to the client.

/* VIRTUAL */ int 
ACE_Name_Handler::send_reply (ACE_UINT32 status, ACE_UINT32 err)
{
  ACE_TRACE ("ACE_Name_Handler::send_reply");
  void *buf;
  this->name_reply_.msg_type (status);
  this->name_reply_.errnum (err);

  this->name_reply_.init ();
  int len = this->name_reply_.encode (buf);

  if (len == -1)
    return -1;
    
  ssize_t n = this->peer ().send (buf, len);

  if (n != len)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n, expected len = %d, actual len = %d", 
		      "send failed", len, n), -1);
  else
    return 0;
}

/* VIRTUAL */ int
ACE_Name_Handler::send_request (ACE_Name_Request &request)
{
  ACE_TRACE ("ACE_Name_Handler::send_request");
  void    *buffer;
  ssize_t length = request.encode (buffer);
 
  if (length == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "encode failed"), -1);
 
  // Transmit request via a blocking send.
 
  if (this->peer ().send_n (buffer, length) != length)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send_n failed"), -1);

  return 0;
}

// Give up waiting (e.g., when a timeout occurs or a client shuts down
// unexpectedly).
 
/* VIRTUAL */ int
ACE_Name_Handler::abandon (void)
{
  ACE_TRACE ("ACE_Name_Handler::abandon");
  int failure_reason = errno;
  return this->send_reply (ACE_Name_Reply::FAILURE, failure_reason);
}

// Enable clients to limit the amount of time they'll wait
 
/* VIRTUAL */ int
ACE_Name_Handler::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_TRACE ("ACE_Name_Handler::handle_timeout");
  return this->abandon ();
}

// Return the underlying ACE_HANDLE.

/* VIRTUAL */ ACE_HANDLE 
ACE_Name_Handler::get_handle (void) const
{
  ACE_TRACE ("ACE_Name_Handler::get_handle");
  return this->peer ().get_handle ();
}

// Dispatch the appropriate operation to handle the client request.

/* VIRTUAL */ int 
ACE_Name_Handler::dispatch (void)
{
  ACE_TRACE ("ACE_Name_Handler::dispatch");
  // Dispatch the appropriate request.
  int index = this->name_request_.msg_type ();

  // Invoke the appropriate member function obtained by indexing into
  // the op_table_. ACE_TABLE_MAP returns the same index (by bitwise
  // AND) for list_names, list_values, and list_types since they are
  // all handled by the same method. Similarly, it returns the same
  // index for list_name_entries, list_value_entries, and
  // list_type_entries.
  return (this->*op_table_[ACE_TABLE_MAP (index, ACE_Name_Request::OP_TABLE_MASK)]) ();
}
  
// Receive, frame, and decode the client's request.  Note, this method
// should use non-blocking I/O.

/* VIRTUAL */ int 
ACE_Name_Handler::recv_request (void)
{
  ACE_TRACE ("ACE_Name_Handler::recv_request");
  // Read the first 4 bytes to get the length of the message This
  // implementation assumes that the first 4 bytes are the length of
  // the message.
  ssize_t n = this->peer ().recv ((void *) &this->name_request_, sizeof (ACE_UINT32));

  switch (n)
    {
    case -1:
      /* FALLTHROUGH */
      ACE_DEBUG ((LM_DEBUG, "****************** recv_request returned -1\n"));
    default:
      ACE_ERROR ((LM_ERROR, "%p got %d bytes, expected %d bytes\n", 
		 "recv failed", n, sizeof (ACE_UINT32)));
      /* FALLTHROUGH */
    case 0:
      // We've shutdown unexpectedly, let's abandon the connection.
      this->abandon ();
      return -1;
      /* NOTREACHED */
    case sizeof (ACE_UINT32):
      {
	// Transform the length into host byte order.
	ssize_t length = ntohl (this->name_request_.length ());

	// Do a sanity check on the length of the message.
	if (length > sizeof this->name_request_)
	  {
	    ACE_ERROR ((LM_ERROR, "length %d too long\n", length));
	    return this->abandon ();
	  }

	// Receive the rest of the request message.
	// @@ beware of blocking read!!!.
	n = this->peer ().recv ((void *) (((char *) &this->name_request_) 
					+ sizeof (ACE_UINT32)), 
				length - sizeof (ACE_UINT32));

	// Subtract off the size of the part we skipped over...
	if (n != (length - sizeof (ACE_UINT32)))
	  {
	    ACE_ERROR ((LM_ERROR, "%p expected %d, got %d\n", 
		       "invalid length", length, n));
	    return this->abandon ();
	  }

	// Decode the request into host byte order.
	if (this->name_request_.decode () == -1)
	  {
	    ACE_ERROR ((LM_ERROR, "%p\n", "decode failed"));
	    return this->abandon ();
	  }
      }
    }
  return 0;
}

// Callback method invoked by the ACE_Reactor when events arrive from
// the client.

/* VIRTUAL */ int 
ACE_Name_Handler::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Name_Handler::handle_input");

  if (this->recv_request () == -1)
    return -1;
  else
    return this->dispatch ();
}

int
ACE_Name_Handler::bind (void)
{
  ACE_TRACE ("ACE_Name_Handler::bind");
  return this->shared_bind (0);
}

int
ACE_Name_Handler::rebind (void)
{
  ACE_TRACE ("ACE_Name_Handler::rebind");
  int result = this->shared_bind (1);
  return result == 1 ? 0 : result;
}

int
ACE_Name_Handler::shared_bind (int rebind)
{
  ACE_TRACE ("ACE_Name_Handler::shared_bind");
  ACE_WString a_name (this->name_request_.name (),
		      this->name_request_.name_len () / sizeof (ACE_USHORT16));
  ACE_WString a_value (this->name_request_.value (),
		       this->name_request_.value_len () / sizeof (ACE_USHORT16));
  int result;
  if (rebind == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "request for BIND \n"));
      result = this->naming_context_->bind (a_name, a_value, 
					    this->name_request_.type ());
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "request for REBIND \n"));
      result = this->naming_context_->rebind (a_name, a_value, 
					      this->name_request_.type ());
      if (result == 1)
	result = 0;
    }
  if (result == 0)
    return this->send_reply (ACE_Name_Reply::SUCCESS);
  else return this->send_reply (ACE_Name_Reply::FAILURE);
}

int
ACE_Name_Handler::resolve (void)
{
  ACE_TRACE ("ACE_Name_Handler::resolve");
  ACE_DEBUG ((LM_DEBUG, "request for RESOLVE \n"));
  ACE_WString a_name (this->name_request_.name (),
		      this->name_request_.name_len () / sizeof (ACE_USHORT16));

  // The following will deliver our reply back to client we
  // pre-suppose success (indicated by type RESOLVE).

  ACE_WString avalue;
  char *atype; 
  if (this->naming_context_->resolve (a_name, avalue, atype) == 0)
    {
      ACE_Name_Request nrq (ACE_Name_Request::RESOLVE,
			    NULL, 0,
			    avalue.rep (),
			    avalue.length () * sizeof (ACE_USHORT16),
			    atype, ACE_OS::strlen (atype));
      return this->send_request (nrq);
    }
  
  ACE_Name_Request nrq (ACE_Name_Request::BIND, NULL, 0, NULL, 0, NULL, 0);  
  this->send_request (nrq);
  return 0;
}

int
ACE_Name_Handler::unbind (void)
{
  ACE_TRACE ("ACE_Name_Handler::unbind");
  ACE_DEBUG ((LM_DEBUG, "request for UNBIND \n"));
  ACE_WString a_name (this->name_request_.name (),
		      this->name_request_.name_len () / sizeof (ACE_USHORT16));
  if (this->naming_context_->unbind (a_name) == 0)
    return this->send_reply (ACE_Name_Reply::SUCCESS);
  else return this->send_reply (ACE_Name_Reply::FAILURE);
}

ACE_Name_Request 
ACE_Name_Handler::name_request (ACE_WString *one_name)
{
  ACE_TRACE ("ACE_Name_Handler::name_request");
  return ACE_Name_Request (ACE_Name_Request::LIST_NAMES,
			   one_name->rep (),
			   one_name->length () * sizeof (ACE_USHORT16),
			   NULL, 0,
			   NULL, 0);
}

ACE_Name_Request
ACE_Name_Handler::value_request (ACE_WString *one_value)
{
  ACE_TRACE ("ACE_Name_Handler::value_request");
  return ACE_Name_Request (ACE_Name_Request::LIST_VALUES,
			   NULL, 0,
			   one_value->rep (),
			   one_value->length () * sizeof (ACE_USHORT16),
			   NULL, 0);
}

ACE_Name_Request
ACE_Name_Handler::type_request (ACE_WString *one_type)
{
  ACE_TRACE ("ACE_Name_Handler::type_request");
  return ACE_Name_Request (ACE_Name_Request::LIST_TYPES,
			   NULL, 0,
			   NULL, 0,
			   one_type->char_rep (),
			   one_type->length ());
}

int
ACE_Name_Handler::lists (void)
{
  ACE_TRACE ("ACE_Name_Handler::lists");

  ACE_PWSTRING_SET set;	
  ACE_WString pattern (this->name_request_.name (),
		       this->name_request_.name_len () / sizeof (ACE_USHORT16));

  // Get the index into the list table
  int index = ACE_LIST_MAP (this->name_request_.msg_type (), 
			    ACE_Name_Request::LIST_OP_MASK);

  // Print the message type
  ACE_DEBUG ((LM_DEBUG, list_table_[index].description_));

  // Call the appropriate method
  if ((this->naming_context_->*list_table_[index].operation_) (set, pattern) != 0)
    {
      // None found so send blank request back
      ACE_Name_Request end_rq (ACE_Name_Request::MAX_ENUM, NULL, 0, NULL, 0, NULL, 0);

      if (this->send_request (end_rq) == -1)
	return -1;
    }
  else
    {
      ACE_WString *one_entry = 0;
      
      for (ACE_Unbounded_Set_Iterator<ACE_WString> set_iterator (set);
	   set_iterator.next (one_entry) !=0;
	   set_iterator.advance())
	{
	  ACE_Name_Request nrq ((this->*list_table_[index].request_factory_) (one_entry));

	  // Create a request by calling the appropriate method obtained
	  // by accessing into the table. Then send the request across.
	  if (this->send_request (nrq) == -1)
	    return -1;
	}

      // Send last message indicator.
      ACE_Name_Request nrq (ACE_Name_Request::MAX_ENUM,
			    NULL, 0,
			    NULL, 0,
			    NULL, 0);
      return this->send_request (nrq);
    }
  return 0;
}

int
ACE_Name_Handler::lists_entries (void)
{
  ACE_TRACE ("ACE_Name_Handler::lists_entries");
  ACE_BINDING_SET set;  
  ACE_WString pattern (this->name_request_.name (),
		       this->name_request_.name_len () / sizeof (ACE_USHORT16));

  int (ACE_Naming_Context::*ptmf) (ACE_BINDING_SET &, const ACE_WString &);
  
  switch (this->name_request_.msg_type ())
    {
    case ACE_Name_Request::LIST_NAME_ENTRIES:
      ACE_DEBUG ((LM_DEBUG, "request for LIST_NAME_ENTRIES \n"));
      ptmf = &ACE_Naming_Context::list_name_entries;
      break;
    case ACE_Name_Request::LIST_VALUE_ENTRIES:
      ACE_DEBUG ((LM_DEBUG, "request for LIST_VALUE_ENTRIES \n"));
      ptmf = &ACE_Naming_Context::list_value_entries;
      break;
    case ACE_Name_Request::LIST_TYPE_ENTRIES:
      ACE_DEBUG ((LM_DEBUG, "request for LIST_TYPE_ENTRIES \n"));
      ptmf = &ACE_Naming_Context::list_type_entries;
      break;
    default:
      return -1;                
    }
  
  if ((this->naming_context_->*ptmf) (set, pattern) != 0)
    {
      // None found so send blank request back.
      ACE_Name_Request end_rq (ACE_Name_Request::MAX_ENUM, NULL, 0, NULL, 0, NULL, 0);

      if (this->send_request (end_rq) == -1)
	return -1;
    }
  else
    {
      ACE_Name_Binding *one_entry = 0;
      
      for (ACE_Unbounded_Set_Iterator<ACE_Name_Binding> set_iterator (set);
	   set_iterator.next (one_entry) !=0;
	   set_iterator.advance())
	{
	  ACE_Name_Request mynrq (this->name_request_.msg_type (),
				  one_entry->name_.rep (),
				  one_entry->name_.length () * sizeof (ACE_USHORT16),
				  one_entry->value_.rep (),
				  one_entry->value_.length () * sizeof (ACE_USHORT16),
                                  one_entry->type_,
				  ACE_OS::strlen (one_entry->type_));

	  if (this->send_request (mynrq) == -1)
	    return -1;
	}
      
      // send last message indicator
      ACE_Name_Request nrq (ACE_Name_Request::MAX_ENUM, NULL, 0, NULL, 0, NULL, 0);      

      if (this->send_request (nrq) == -1)
        return -1;
    }
  return 0;
}

ACE_Name_Handler::~ACE_Name_Handler (void)
{
  ACE_TRACE ("ACE_Name_Handler::~ACE_Name_Handler");
  ACE_DEBUG ((LM_DEBUG, "closing down Handle  %d\n",
	      this->get_handle ()));

  // Delete associated Naming Context.
  delete this->naming_context_;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Strategy_Acceptor<ACE_Name_Handler, ACE_SOCK_Acceptor, ACE_INET_Addr>;
template class ACE_Schedule_All_Reactive_Strategy<ACE_Name_Handler>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
