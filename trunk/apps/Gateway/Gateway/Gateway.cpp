/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "Config_Files.h"
#include "ace/Service_Config.h"
#include "Event_Channel.h"
#include "Gateway.h"

class Gateway : public ACE_Service_Object
  // = TITLE
  //     Integrates the whole Gateway application.
  //
  // = DESCRIPTION
  //     This implementation uses the <ACE_Event_Channel> as the basis
  //     for the <Gateway> routing.
{
public:
  // = Service configurator hooks.
  virtual int init (int argc, char *argv[]);
  // Perform initialization.

  virtual int fini (void);
  // Perform termination.

  virtual int info (char **, size_t) const;
  // Return info about this service.

  int parse_connection_config_file (void);
  // Parse the connection configuration file.

  int parse_consumer_config_file (void);
  // Parse the consumer configuration file.

protected:
  int handle_input (ACE_HANDLE);
  // Shut down the Gateway when input comes in from the controlling
  // console.

  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Shut down the Gateway when a signal arrives.

  int parse_args (int argc, char *argv[]); 
  // Parse gateway configuration arguments obtained from svc.conf
  // file.

  char connection_config_file_[MAXPATHLEN + 1];
  // Name of the connection configuration file.

  char consumer_config_file_[MAXPATHLEN + 1];
  // Name of the consumer map configuration file.

  ACE_Event_Channel event_channel_;
  // The Event Channel routes events from Supplier(s) to Consumer(s).

  int active_connector_role_;
  // Enabled if we are playing the role of the active Connector.

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
  if (ACE_Service_Config::reactor ()->remove_handler 
      (0, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "remove_handler"), -1);

  char buf[BUFSIZ];
  // Consume the input...
  ACE_OS::read (h, buf, sizeof (buf));

  // Shut us down.
  return this->handle_signal (h);
}

// Parse the "command-line" arguments and set the corresponding flags.

int
Gateway::parse_args (int argc, char *argv[])
{
  ACE_OS::strcpy (this->connection_config_file_, "connection_config");
  ACE_OS::strcpy (this->consumer_config_file_, "consumer_config");
  this->active_connector_role_ = 1;
  this->debug_ = 0;

  ACE_Get_Opt get_opt (argc, argv, "bc:dpr:q:w:", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'b': // Use blocking connection establishment.
	  this->event_channel_.options ().blocking_semantics_ = 0;
	  break;
	case 'c':
	  ACE_OS::strncpy (this->connection_config_file_, 
			   get_opt.optarg, 
			   sizeof this->connection_config_file_);
	  break;
	case 'd':
	  this->debug_ = 1;
	  break;
	case 'p':
	  // We are not playing the active Connector role.
	  this->active_connector_role_ = 0;
	  break;
	case 'q':
	  this->event_channel_.options ().socket_queue_size_ = 
	    ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'r':
	  ACE_OS::strncpy (this->consumer_config_file_,
			   get_opt.optarg, 
			   sizeof this->consumer_config_file_);
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
  return 0;
}

int
Gateway::init (int argc, char *argv[])
{
  // Initialize the Event_Channel.
  if (this->event_channel_.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "open"), -1);

  // Parse the "command-line" arguments.
  this->parse_args (argc, argv);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive SIGINT and SIGQUIT so we can shut
  // down gracefully via signals.
  
  if (ACE_Service_Config::reactor ()->register_handler 
      (sig_set, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);

  if (ACE_Service_Config::reactor ()->register_handler 
      (0, this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);

  if (this->active_connector_role_)
    {
      // Parse the connection configuration file.
      this->parse_connection_config_file ();

      // Parse the consumer map config file and build the consumer
      // map.
      this->parse_consumer_config_file ();

      // Initiate connections with the peers.
      this->event_channel_.initiate_connections ();
    }

  return 0;
}

// This method is automatically called when the gateway is shutdown.
// It closes down the Event Channel.

int
Gateway::fini (void)
{
  this->event_channel_.close ();
  return 0;
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

// Parse and build the connection table.

int
Gateway::parse_connection_config_file (void)
{
  // File that contains the consumer map configuration information.
  Connection_Config_File_Parser connection_file;
  Connection_Config_File_Entry entry;
  int file_empty = 1;
  int line_number = 0;

  if (connection_file.open (this->connection_config_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", this->connection_config_file_), -1);

  // Read config file one line at a time.
  while (connection_file.read_entry (entry, line_number) != FP::EOFILE)
    {
      file_empty = 0;

      if (this->debug_)
	ACE_DEBUG ((LM_DEBUG, "(%t) conn id = %d, host = %s, remote port = %d, "
		    "proxy role = %c, max retry timeout = %d, local port = %d\n",
		    entry.conn_id_, 
		    entry.host_, 
		    entry.remote_port_, 
		    entry.proxy_role_,
		    entry.max_retry_delay_, 
		    entry.local_port_));

      Proxy_Handler *proxy_handler = 0;

      // Initialize the entry's peer addressing info.

      ACE_INET_Addr remote_addr (entry.remote_port_, entry.host_);
      ACE_INET_Addr local_addr (entry.local_port_);

      // The next few lines of code are dependent on whether we are
      // making an Supplier_Proxy or an Consumer_Proxy.

      if (entry.proxy_role_ == 'C') // Configure a Consumer_Proxy.
	ACE_NEW_RETURN (proxy_handler,
			CONSUMER_PROXY (this->event_channel_, remote_addr, 
					local_addr, entry.conn_id_),
			-1);
      else // proxy_role == 'S', so configure a Supplier_Proxy.
	ACE_NEW_RETURN (proxy_handler,
			SUPPLIER_PROXY (this->event_channel_, remote_addr, 
					local_addr, entry.conn_id_),
			-1);

      // The following code is common to both Supplier_Proxys_ and
      // Consumer_Proxys.

      // Initialize max timeout.
      proxy_handler->max_timeout (entry.max_retry_delay_);

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
  Consumer_Config_File_Entry  entry;  
  int file_empty = 1;
  int line_number = 0;

  if (consumer_file.open (this->consumer_config_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", this->consumer_config_file_), -1);

  // Read config file line at a time.
  while (consumer_file.read_entry (entry, line_number) != FP::EOFILE)
    {
      file_empty = 0;

      if (this->debug_)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%t) conn id = %d, logical id = %d, payload = %d, "
		      "number of consumers = %d\n",
		      entry.conn_id_, 
		      entry.supplier_id_, 
		      entry.type_,
		      entry.total_consumers_));
	  for (int i = 0; i < entry.total_consumers_; i++)
	    ACE_DEBUG ((LM_DEBUG, "(%t) destination[%d] = %d\n",
		       i, entry.consumers_[i]));
	}

      Consumer_Dispatch_Set *dispatch_set;
      ACE_NEW_RETURN (dispatch_set, Consumer_Dispatch_Set, -1);

      Event_Key event_addr (entry.conn_id_, 
			    entry.supplier_id_, 
			    entry.type_);

      // Add the Consumers to the Dispatch_Set.
      for (int i = 0; i < entry.total_consumers_; i++)
	{
	  Proxy_Handler *proxy_handler = 0;

	  // Lookup destination and add to Consumer_Dispatch_Set set
	  // if found.
	  if (this->event_channel_.find_proxy (entry.consumers_[i], 
					       proxy_handler) != -1)
	    dispatch_set->insert (proxy_handler);
	  else
	    ACE_ERROR ((LM_ERROR, "(%t) not found: destination[%d] = %d\n", 
		       i, entry.consumers_[i]));
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
