/* -*- C++ -*- */
// $Id$

#define ACE_BUILD_SVC_DLL
#include "ace/Get_Opt.h"
#include "Config_Files.h"
#include "ace/Service_Config.h"
#include "Event_Channel.h"
#include "Gateway.h"

class ACE_Svc_Export Gateway : public ACE_Service_Object
  // = TITLE
  //     Integrates the whole Gateway application.
  //
  // = DESCRIPTION
  //     This implementation uses the <ACE_Event_Channel> as the basis
  //     for the <Gateway> routing.
{
protected:
  // = Service configurator hooks.
  virtual int init (int argc, char *argv[]);
  // Perform initialization.

  virtual int fini (void);
  // Perform termination when unlinked dynamically.

  virtual int info (char **, size_t) const;
  // Return info about this service.

  // = Configuration methods.
  int parse_proxy_config_file (void);
  // Parse the proxy configuration file.

  int parse_consumer_config_file (void);
  // Parse the consumer configuration file.

  // = Lifecycle management methods.
  int handle_input (ACE_HANDLE);
  // Shut down the Gateway when input comes in from the controlling
  // console.

  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Shut down the Gateway when a signal arrives.

  int parse_args (int argc, char *argv[]); 
  // Parse gateway configuration arguments obtained from svc.conf
  // file.

  char proxy_config_file_[MAXPATHLEN + 1];
  // Name of the connection configuration file.

  char consumer_config_file_[MAXPATHLEN + 1];
  // Name of the consumer map configuration file.

  ACE_Event_Channel event_channel_;
  // The Event Channel routes events from Supplier(s) to Consumer(s)
  // using <Supplier_Proxy> and <Consumer_Proxy> objects.

  Proxy_Handler_Factory proxy_handler_factory_;
  // Creates the appropriate type of <Proxy_Handlers>.

  int debug_;
  // Are we debugging?
};

int
Gateway::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  if (signum > 0)
    ACE_DEBUG ((LM_DEBUG, "(%t) %S\n", signum));

  // Shut down the main event loop.
  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

int
Gateway::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ];
  // Consume the input...
  ACE_OS::read (h, buf, sizeof (buf));

  // Shut us down.
  return this->handle_signal ((int) h);
}

// Parse the "command-line" arguments and set the corresponding flags.

int
Gateway::parse_args (int argc, char *argv[])
{
  // Assign defaults.
  ACE_OS::strcpy (this->proxy_config_file_, "proxy_config");
  ACE_OS::strcpy (this->consumer_config_file_, "consumer_config");
  this->debug_ = 0;

  ACE_Get_Opt get_opt (argc, argv, "abC:cdP:pq:t:vw:", 0);

  for (int c; (c = get_opt ()) != EOF; )
    {
      switch (c)
	{
	case 'a': // We are (also?) playing the Acceptor role.
	  this->event_channel_.options ().acceptor_role_ = 1;
	  break;

	case 'b': // Use blocking connection establishment.
	  this->event_channel_.options ().blocking_semantics_ = 1;
	  break;
	case 'C': // Use a different proxy config filename.
	  ACE_OS::strncpy (this->consumer_config_file_,
			   get_opt.optarg, 
			   sizeof this->consumer_config_file_);
	  break;
	case 'c': // We are (also?) playing the Connector role.
	  this->event_channel_.options ().connector_role_ = 1;
	  break;
	case 'd': // We are debugging.
	  this->debug_ = 1;
	  break;
	case 'P': // Use a different consumer config filename.
	  ACE_OS::strncpy (this->proxy_config_file_, 
			   get_opt.optarg, 
			   sizeof this->proxy_config_file_);
	  break;
	case 'p': // Use a different acceptor port.
	  this->event_channel_.options ().acceptor_port_ = 
	    ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'q': // Use a different socket queue size.
	  this->event_channel_.options ().socket_queue_size_ = 
	    ACE_OS::atoi (get_opt.optarg);
	  break;
	case 't': // Use a different threading strategy.
	  {
	    for (char *flag = ACE_OS::strtok (get_opt.optarg, "|");
		 flag != 0;
		 flag = ACE_OS::strtok (0, "|"))
	      {
		if (ACE_OS::strcmp (flag, "OUTPUT_MT") == 0)
		  ACE_SET_BITS (this->event_channel_.options ().threading_strategy_, 
				ACE_Event_Channel_Options::OUTPUT_MT);
		else if (ACE_OS::strcmp (flag, "INPUT_MT") == 0)
		  ACE_SET_BITS (this->event_channel_.options ().threading_strategy_, 
				ACE_Event_Channel_Options::INPUT_MT);
	      }

	    break;
	  }
	case 'v': // Verbose mode.
	  this->event_channel_.options ().verbose_ = 1;
	  break;
	case 'w': // Time performance for a designated amount of time.
	  this->event_channel_.options ().performance_window_ = 
	    ACE_OS::atoi (get_opt.optarg);
	  // Use blocking connection semantics so that we get accurate
	  // timings (since all connections start at once).
	  this->event_channel_.options ().blocking_semantics_ = 0;
	  break;
	default:
	  break;
	}
    }
  
  // Nonblocking connects are currently broken 
  this->event_channel_.options ().blocking_semantics_ = 1;

  return 0;
}

int
Gateway::init (int argc, char *argv[])
{
  // Parse the "command-line" arguments.
  this->parse_args (argc, argv);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive SIGINT and SIGQUIT so we can shut
  // down gracefully via signals.
  
#if defined (ACE_WIN32)
  if (ACE_Service_Config::reactor ()->register_handler 
      (SIGINT, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);
#else
  if (ACE_Service_Config::reactor ()->register_handler 
      (sig_set, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);
#endif

  // Register this handler to receive events on stdin.  We use this to
  // shutdown the Gateway gracefully.
  if (ACE::register_stdin_handler (this,
				   ACE_Service_Config::reactor (),
				   ACE_Service_Config::thr_mgr ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_stdin_handler"), -1);

  // If this->performance_window_ > 0 start a timer.

  if (this->event_channel_.options ().performance_window_ > 0)
    {
      if (ACE_Service_Config::reactor ()->schedule_timer 
	  (&this->event_channel_, 0, 
	   this->event_channel_.options ().performance_window_) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "schedule_timer"));
      else
	ACE_DEBUG ((LM_DEBUG, "starting timer for %d seconds...\n", 
		   this->event_channel_.options ().performance_window_));
    }

  if (this->event_channel_.options ().connector_role_)
    {
      // Parse the proxy configuration file.
      this->parse_proxy_config_file ();

      // Parse the consumer config file and build the event forwarding
      // discriminator.
      this->parse_consumer_config_file ();
    }

  // Initialize the Event_Channel.
  return this->event_channel_.open ();
}

// This method is automatically called when the Gateway is shutdown.

int
Gateway::fini (void)
{
  // Close down the event channel.
  return this->event_channel_.close ();
}

// Returns information on the currently active service. 

int
Gateway::info (char **strp, size_t length) const
{
  char buf[BUFSIZ];

  ACE_OS::sprintf (buf, "%s\t %s", "Gateway daemon", 
	     "# Application-level gateway\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

// Parse and build the proxy table.

int
Gateway::parse_proxy_config_file (void)
{
  // File that contains the proxy configuration information.
  Proxy_Config_File_Parser proxy_file;
  int file_empty = 1;
  int line_number = 0;

  if (proxy_file.open (this->proxy_config_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "(%t) %p\n", 
		       this->proxy_config_file_), 
		      -1);

  // Read config file one line at a time.
  for (Proxy_Config_Info pci;
       proxy_file.read_entry (pci, line_number) != FP::EOFILE;
       )
    {
      file_empty = 0;

      if (this->debug_)
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) conn id = %d, host = %s, remote port = %d, proxy role = %c, "
		    "max retry timeout = %d, local port = %d, priority = %d\n",
		    pci.proxy_id_, 
		    pci.host_, 
		    pci.remote_port_, 
		    pci.proxy_role_,
		    pci.max_retry_timeout_, 
		    pci.local_port_,
		    pci.priority_));

      pci.event_channel_ = &this->event_channel_;

      // Create the appropriate type of Proxy.
      Proxy_Handler *proxy_handler = 
	this->proxy_handler_factory_.make_proxy_handler (pci);

      if (proxy_handler == 0)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "make_proxy_handler"), -1);

      // Bind the new Proxy_Handler to the connection ID.
      this->event_channel_.bind_proxy (proxy_handler);
    }

  if (file_empty)
    ACE_ERROR ((LM_WARNING, 
	       "warning: connection proxy_handler configuration file was empty\n"));
  return 0;
}

int
Gateway::parse_consumer_config_file (void)
{
  // File that contains the consumer event forwarding information.
  Consumer_Config_File_Parser consumer_file;
  int file_empty = 1;
  int line_number = 0;

  if (consumer_file.open (this->consumer_config_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", this->consumer_config_file_), -1);

  // Read config file line at a time.
  for (Consumer_Config_Info cci;
       consumer_file.read_entry (cci, line_number) != FP::EOFILE;
       )
    {
      file_empty = 0;

      if (this->debug_)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%t) conn id = %d, supplier id = %d, payload = %d, "
		      "number of consumers = %d\n",
		      cci.proxy_id_, 
		      cci.supplier_id_, 
		      cci.type_,
		      cci.total_consumers_));

	  for (int i = 0; i < cci.total_consumers_; i++)
	    ACE_DEBUG ((LM_DEBUG, "(%t) destination[%d] = %d\n",
		       i, cci.consumers_[i]));
	}

      Consumer_Dispatch_Set *dispatch_set;
      ACE_NEW_RETURN (dispatch_set, Consumer_Dispatch_Set, -1);

      Event_Key event_addr (cci.proxy_id_, 
			    cci.supplier_id_, 
			    cci.type_);

      // Add the Consumers to the Dispatch_Set.
      for (int i = 0; i < cci.total_consumers_; i++)
	{
	  Proxy_Handler *proxy_handler = 0;

	  // Lookup destination and add to Consumer_Dispatch_Set set
	  // if found.
	  if (this->event_channel_.find_proxy (cci.consumers_[i], 
					       proxy_handler) != -1)
	    dispatch_set->insert (proxy_handler);
	  else
	    ACE_ERROR ((LM_ERROR, "(%t) not found: destination[%d] = %d\n", 
		       i, cci.consumers_[i]));
	}

      this->event_channel_.subscribe (event_addr, dispatch_set);
    }

  if (file_empty)
    ACE_ERROR ((LM_WARNING, 
	       "warning: consumer map configuration file was empty\n"));
  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Gateway.

ACE_SVC_FACTORY_DEFINE (Gateway)

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Node<Proxy_Handler *>;
template class ACE_Unbounded_Set<Proxy_Handler *>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
