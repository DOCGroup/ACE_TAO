/* -*- C++ -*- */
// $Id$



#include "ace/Get_Opt.h"
#include "ace/Service_Config.h"
#include "Config_Files.h"
#include "Gateway.h"
#include "Channel_Connector.h"

template <class INPUT_CHANNEL, class OUTPUT_CHANNEL>
class Gateway : public ACE_Service_Object
{
public:
  Gateway (ACE_Thread_Manager * = 0);

  virtual int init (int argc, char *argv[]);
  // Perform initialization.

  virtual int fini (void);
  // Perform termination.

protected:
  int handle_input (ACE_HANDLE);

  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);

  typedef ACE_Map_Manager<CONN_ID, Channel *, MUTEX> CONFIG_TABLE;
  typedef ACE_Map_Iterator<CONN_ID, Channel *, MUTEX> CONFIG_ITERATOR;

  CONFIG_TABLE config_table_;
  // Table that maps Connection IDs to Channel *'s.

  ROUTING_TABLE routing_table_;
  // Table that maps Peer addresses to a set of Channel *'s for output.

  virtual int info (char **, size_t) const;
  // Return info about this service.

  int parse_args (int argc, char *argv[]); 
  // Parse gateway configuration arguments obtained from svc.conf file.

  int parse_cc_config_file (void);
  // Parse the channel connection configuration file.

  int parse_rt_config_file (void);
  // Parse the routing table configuration file.

  int initiate_connections (void);
  // Initiate connections to the peers.

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg);
  // Perform timer-based performance profiling.

  const char *cc_config_file_;
  // Name of the channel connection configuration file.

  const char *rt_config_file_;
  // Name of the routing table configuration file.

  int performance_window_;
  // Number of seconds after connection establishment to report throughput.
  
  int blocking_semantics_;
  // 0 == blocking connects, ACE_NONBLOCK == non-blocking connects.

  int debug_;
  // Are we debugging?

  Channel_Connector *connector_;
  // This is used to establish the connections actively.

  int socket_queue_size_;
  // Size of the socket queue (0 means "use default").

  // = Manage output and input channel threads (if used.)
  // if both input and output mt is used, they will share thr_mgr_,
  // thr_mgr_ will always reference the thread manager being used
  // regardless of whether input, output, or both channels are using mt.
  ACE_Thread_Manager *thr_mgr_;
  ACE_Thread_Manager *input_thr_mgr_;
  ACE_Thread_Manager *output_thr_mgr_;
};

// Convenient shorthands.

#define IC INPUT_CHANNEL
#define OC OUTPUT_CHANNEL

template <class IC, class OC> int
Gateway<IC, OC>::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  if (signum > 0)
    ACE_DEBUG ((LM_DEBUG, "(%t) %S\n", signum));

  if (this->thr_mgr_ != 0)
    {
#if defined (ACE_HAS_THREADS)
      ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads\n"));
      if (this->thr_mgr_->suspend_all () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
#endif /* ACE_HAS_THREADS */
    }

  // Shut down the main event loop.
  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

template <class IC, class OC> int
Gateway<IC, OC>::handle_input (ACE_HANDLE h)
{
  if (ACE_Service_Config::reactor ()->remove_handler (0, 
					       ACE_Event_Handler::READ_MASK
					       | ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "remove_handler"), -1);
  char buf[BUFSIZ];
  // Consume the input...
  ACE_OS::read (h, buf, sizeof (buf));
  return this->handle_signal (h);
}

template <class IC, class OC> int
Gateway<IC, OC>::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) doing the performance timeout here...\n"));
  CONFIG_ITERATOR cti (this->config_table_);

  // If we've got a ACE_Thread Manager then use it to suspend all 
  // the threads.  This will enable us to get an accurate count.

  if (this->thr_mgr_ != 0)
    {
#if defined (ACE_HAS_THREADS)
      if (this->thr_mgr_->suspend_all () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
      ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads..."));
#endif /* ACE_HAS_THREADS */
    }

  size_t total_bytes_in = 0;
  size_t total_bytes_out = 0;
  
  // Iterate through the routing table connecting all the channels.

  for (ACE_Map_Entry <CONN_ID, Channel *> *me = 0; 
       cti.next (me) != 0; 
       cti.advance ())
    {
      Channel *channel = me->int_id_;
      if (channel->direction () == 'O')
	total_bytes_out += channel->total_bytes ();
      else
	total_bytes_in += channel->total_bytes ();
    }

#if defined (ACE_NLOGGING)
  ACE_OS::fprintf (stderr, "After %d seconds, \ntotal_bytes_in = %d\ntotal_bytes_out = %d\n",
	     performance_window_,
	     total_bytes_in, 
	     total_bytes_out);

  ACE_OS::fprintf (stderr, "%f Mbits/sec received.\n", 
	     (float) (total_bytes_in * 8 / (float) (1024*1024*this->performance_window_)));

  ACE_OS::fprintf (stderr, "%f Mbits/sec sent.\n", 
	     (float) (total_bytes_out * 8 / (float) (1024*1024*this->performance_window_)));
#else
  ACE_DEBUG ((LM_DEBUG, "(%t) after %d seconds, \ntotal_bytes_in = %d\ntotal_bytes_out = %d\n",
	     this->performance_window_,
	     total_bytes_in, 
	     total_bytes_out));
  ACE_DEBUG ((LM_DEBUG, "(%t) %f Mbits/sec received.\n", 
	     (float) (total_bytes_in * 8 / (float) (1024*1024*this->performance_window_))));
  ACE_DEBUG ((LM_DEBUG, "(%t) %f Mbits/sec sent.\n", 
	     (float) (total_bytes_out * 8 / (float) (1024*1024*this->performance_window_))));
#endif /* ACE_NLOGGING */
  // Resume all the threads again.
  if (this->thr_mgr_ != 0)
    {
#if defined (ACE_HAS_THREADS)
      this->thr_mgr_->resume_all ();
      ACE_DEBUG ((LM_DEBUG, "(%t) resuming all threads..."));
#endif /* ACE_HAS_THREADS */
    }
  return 0;
}

// Give default values to data members.

template <class IC, class OC>
Gateway<IC, OC>::Gateway (ACE_Thread_Manager *thr_mgr)
  : cc_config_file_ ("cc_config"),
    rt_config_file_ ("rt_config"),
    performance_window_ (0),
    blocking_semantics_ (ACE_NONBLOCK),
    debug_ (0),
    connector_ (0),
    socket_queue_size_ (0),
    thr_mgr_ (thr_mgr),
    input_thr_mgr_ (thr_mgr),
    output_thr_mgr_ (thr_mgr)
{
}

// Parse the "command-line" arguments and set the corresponding flags.

template <class IC, class OC> int
Gateway<IC, OC>::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "bc:dr:q:w:", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'b': // Use blocking connection establishment.
	  this->blocking_semantics_ = 0;
	  break;
	case 'c':
	  this->cc_config_file_ = get_opt.optarg;
	  break;
	case 'd':
	  this->debug_ = 1;
	  break;
	case 'q':
	  this->socket_queue_size_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'r':
	  this->rt_config_file_ = get_opt.optarg;
	  break;
	case 'w': // Time performance for a designated amount of time.
	  this->performance_window_ = ACE_OS::atoi (get_opt.optarg);
	  // Use blocking connection semantics so that we get accurate
	  // timings (since all connections start at once).
	  this->blocking_semantics_ = 0;
	  break;
	default:
	  break;
	}
    }
  return 0;
}

// Initiate connections with the peers.

template <class IC, class OC> int
Gateway<IC, OC>::initiate_connections (void)
{
  CONFIG_ITERATOR cti (this->config_table_);
				
  // Iterate through the routing table connecting all the channels.

  for (ACE_Map_Entry <CONN_ID, Channel *> *me = 0; 
       cti.next (me) != 0; 
       cti.advance ())
    {
      Channel *channel = me->int_id_;
      if (this->connector_->initiate_connection 
	  (channel, this->blocking_semantics_ == ACE_NONBLOCK 
	   ? ACE_Synch_Options::asynch : ACE_Synch_Options::synch) == -1)
	continue;
    }

  return 0;
}

// This method is automatically called when the gateway
// is shutdown.  It gracefully shuts down all the Channels
// in the Channel connection Config_Table.

template <class IC, class OC> int
Gateway<IC, OC>::fini (void)
{
  // Question: do we need to do anything special about the Routing_Table?
 
  CONFIG_ITERATOR cti (this->config_table_);

  for (ACE_Map_Entry <CONN_ID, Channel *> *me; 
       cti.next (me) != 0; 
       cti.advance ())
    {
      Channel *channel = me->int_id_;
      ACE_DEBUG ((LM_DEBUG, "(%t) closing down route %d\n", 
		 channel->id ()));
      if (channel->state () != Channel::IDLE)
	// Mark channel as DISCONNECTING so we don't try to reconnect...
	channel->state (Channel::DISCONNECTING);

      // Deallocate Channel resources.
      channel->destroy (); // Will trigger a delete.
    }

  // Free up the resources allocated dynamically by the ACE_Connector.
  delete this->connector_;
  delete this->thr_mgr_;

  return 0;
}

template <class IC, class OC> int
Gateway<IC, OC>::init (int argc, char *argv[])
{
  this->parse_args (argc, argv);

  ACE_NEW_RETURN (this->connector_, Channel_Connector (), -1);

  if (this->connector_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "out of memory"), -1);

  // Ignore SIPPIPE so each Output_Channel can handle it.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive SIGINT and SIGQUIT 
  // so we can shut down gracefully via signals.
  
  if (ACE_Service_Config::reactor ()->register_handler (sig_set, 
						 this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);

  if (ACE_Service_Config::reactor ()->register_handler (0,
						 this,
						 ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);

  if (this->thr_mgr_ == 0)
  // Create a thread manager if using some combination of multi-threaded channels.
#if defined (USE_OUTPUT_MT) && defined (USE_INPUT_MT)
    this->thr_mgr_ = this->output_thr_mgr_ = 
      this->input_thr_mgr_ = ACE_Service_Config::thr_mgr ();
#elif defined (USE_OUTPUT_MT)
    this->thr_mgr_ = this->output_thr_mgr_ = ACE_Service_Config::thr_mgr ();
#elif defined (USE_INPUT_MT)
    this->thr_mgr_ = this->input_thr_mgr_ = ACE_Service_Config::thr_mgr ();
#endif

  // Parse the connection configuration file.
  this->parse_cc_config_file ();

  // Parse the routing table config file and build the routing table.
  this->parse_rt_config_file ();

  // Initiate connections with the peers.
  this->initiate_connections ();

  // If this->performance_window_ > 0 start a timer.

  if (this->performance_window_ > 0)
    {
      if (ACE_Service_Config::reactor ()->schedule_timer (this, 0,
						   this->performance_window_) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "schedule_timer"));
      else
	ACE_DEBUG ((LM_DEBUG, "starting timer for %d seconds...\n", 
		   this->performance_window_));
    }

  return 0;
}

// Returns information on the currently active service. 

template <class IC, class OC> int
Gateway<IC, OC>::info (char **strp, size_t length) const
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

template <class IC, class OC> int
Gateway<IC, OC>::parse_cc_config_file (void)
{
  // File that contains the routing table configuration information.
  CC_Config_File_Parser cc_file;
  CC_Config_File_Entry  entry;
  int file_empty = 1;
  int line_number = 0;

  if (cc_file.open (this->cc_config_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", this->cc_config_file_), -1);

  // Read config file line at a time.
  while (cc_file.read_entry (entry, line_number) != FP::EOFILE)
    {
      file_empty = 0;

      if (this->debug_)
	ACE_DEBUG ((LM_DEBUG, "(%t) conn id = %d, host = %s, remote port = %d, "
		    "direction = %c, max retry timeout = %d, local port = %d\n",
		    entry.conn_id_, entry.host_, entry.remote_port_, entry.direction_,
		    entry.max_retry_delay_, entry.local_port_));

      Channel *channel = 0;

      // The next few lines of code are dependent on whether we are making
      // an Input_Channel or an Output_Channel.

      if (entry.direction_ == 'O') // Configure an output channel.
	ACE_NEW_RETURN (channel,
			OUTPUT_CHANNEL (&this->routing_table_, 
					this->connector_, 
					this->output_thr_mgr_,
					this->socket_queue_size_),
			-1);
      else /* direction == 'I' */ // Configure an input channel.
	ACE_NEW_RETURN (channel,
			INPUT_CHANNEL (&this->routing_table_, 
				       this->connector_, 
				       this->input_thr_mgr_,
				       this->socket_queue_size_),
			-1);
      if (channel == 0)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) out of memory\n"), -1);

      // The following code is common to both Input_ and Output_Channels.

      // Initialize the routing entry's peer addressing info.
      channel->bind (ACE_INET_Addr (entry.remote_port_, entry.host_), 
		     ACE_INET_Addr (entry.local_port_), entry.conn_id_);

      // Initialize max timeout.
      channel->max_timeout (entry.max_retry_delay_);

      // Try to bind the new Channel to the connection ID.
      switch (this->config_table_.bind (entry.conn_id_, channel))
	{
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR, "(%t) bind failed for connection %d\n", 
			    entry.conn_id_), -1);
	  /* NOTREACHED */
	case 1: // Oops, found a duplicate!
	  ACE_DEBUG ((LM_DEBUG, "(%t) duplicate connection %d, already bound\n", 
		     entry.conn_id_));
	  break;
	case 0:
	  // Success.
	  break;
	}
    }

  if (file_empty)
    ACE_ERROR ((LM_WARNING, 
	       "warning: connection channel configuration file was empty\n"));

  return 0;
}

template <class IC, class OC> int
Gateway<IC, OC>::parse_rt_config_file (void)
{
  // File that contains the routing table configuration information.
  RT_Config_File_Parser rt_file;
  RT_Config_File_Entry  entry;  
  int file_empty = 1;
  int line_number = 0;

  if (rt_file.open (this->rt_config_file_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", this->rt_config_file_), -1);

  // Read config file line at a time.
  while (rt_file.read_entry (entry, line_number) != FP::EOFILE)
    {
      file_empty = 0;

      if (this->debug_)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%t) conn id = %d, logical id = %d, payload = %d, "
		     "number of destinations = %d\n",
		     entry.conn_id_, entry.logical_id_, entry.payload_type_,
		     entry.total_destinations_));
	  for (int i = 0; i < entry.total_destinations_; i++)
	    ACE_DEBUG ((LM_DEBUG, "(%t) destination[%d] = %d\n",
		       i, entry.destinations_[i]));
	}

      Routing_Entry *re;
      ACE_NEW_RETURN (re, Routing_Entry, -1);
      Routing_Entry::ENTRY_SET *channel_set = new Routing_Entry::ENTRY_SET;
      Peer_Addr peer_addr (entry.conn_id_, entry.logical_id_, 
			   entry.payload_type_);

      // Add the destinations to the Routing Entry.
      for (int i = 0; i < entry.total_destinations_; i++)
	{
	  Channel *channel = 0;

	  // Lookup destination and add to Routing_Entry set if found.
	  if (this->config_table_.find (entry.destinations_[i], 
					channel) != -1)
	    channel_set->insert (channel);
	  else
	    ACE_ERROR ((LM_ERROR, "(%t) not found: destination[%d] = %d\n", 
		       i, entry.destinations_[i]));
	}

      // Attach set of destination channels to routing entry.
      re->destinations (channel_set);

      // Bind with routing table, keyed by peer address.
      switch (this->routing_table_.bind (peer_addr, re))
	{
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR, "(%t) bind failed for connection %d\n", 
			    entry.conn_id_), -1);
	  /* NOTREACHED */
	case 1: // Oops, found a duplicate!
	  ACE_DEBUG ((LM_DEBUG, "(%t) duplicate routing table entry %d, "
		     "already bound\n", entry.conn_id_));
	  break;
	case 0:
	  // Success.
	  break;
	}
    }

  if (file_empty)
    ACE_ERROR ((LM_WARNING, 
	       "warning: routing table configuration file was empty\n"));

  return 0;
}

#if defined (ACE_HAS_THREADS) && (defined (USE_OUTPUT_MT) || defined (USE_INPUT_MT))
#if defined (USE_OUTPUT_MT)
typedef Thr_Output_Channel OUTPUT_CHANNEL;
#else
typedef Output_Channel OUTPUT_CHANNEL;
#endif /* USE_OUTPUT_MT */

#if defined (USE_INPUT_MT)
typedef Thr_Input_Channel INPUT_CHANNEL;
#else
typedef Input_Channel INPUT_CHANNEL;
#endif /* USE_INPUT_MT */
#else
// Instantiate a non-multi-threaded Gateway.
typedef Input_Channel INPUT_CHANNEL;
typedef Output_Channel OUTPUT_CHANNEL;
#endif /* ACE_HAS_THREADS */

typedef Gateway<INPUT_CHANNEL, OUTPUT_CHANNEL> ACE_Gateway;

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Gateway.

ACE_SVC_FACTORY_DEFINE (ACE_Gateway)
