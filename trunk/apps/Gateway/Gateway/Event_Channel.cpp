/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "Config_Files.h"
#include "IO_Handler_Connector.h"
#include "Event_Channel.h"

#if !defined (ACE_EVENT_CHANNEL_C)
#define ACE_EVENT_CHANNEL_C

template <class SH, class CH> 
ACE_Event_Channel<SH, CH>::~ACE_Event_Channel (void)
{
}

template <class SH, class CH> 
ACE_Event_Channel<SH, CH>::ACE_Event_Channel (void)
  : connection_config_file_ ("connection_config"),
    consumer_config_file_ ("consumer_config"),
    active_connector_role_ (1),
    performance_window_ (0),
    blocking_semantics_ (ACE_NONBLOCK),
    debug_ (0),
    connector_ (0),
    socket_queue_size_ (0)
{
}

template <class SH, class CH> int
ACE_Event_Channel<SH, CH>::handle_timeout (const ACE_Time_Value &, 
					   const void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) doing the performance timeout here...\n"));
  CONNECTION_MAP_ITERATOR cti (this->connection_map_);

  // If we've got a ACE_Thread Manager then use it to suspend all the
  // threads.  This will enable us to get an accurate count.

#if defined (USE_OUTPUT_MT) || defined (USE_INPUT_MT)
  if (ACE_Service_Config::thr_mgr ()->suspend_all () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
  ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads..."));
#endif /* USE_INPUT_MT || USE_OUTPUT_MT */

  size_t total_bytes_in = 0;
  size_t total_bytes_out = 0;
  
  // Iterate through the consumer map connecting all the IO_Handlers.

  for (CONNECTION_MAP_ENTRY *me = 0;
       cti.next (me) != 0; 
       cti.advance ())
    {
      IO_Handler *io_handler = me->int_id_;

      if (io_handler->direction () == 'C')
	total_bytes_out += io_handler->total_bytes ();
      else // io_handler->direction () == 'S'
	total_bytes_in += io_handler->total_bytes ();
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

#if defined (USE_INPUT_MT) || defined (USE_OUTPUT_MT)
  ACE_DEBUG ((LM_DEBUG, "(%t) resuming all threads..."));

  // Resume all the threads again.

  if (ACE_Service_Config::thr_mgr ()->resume_all () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "resume_all"), -1);
#endif /* USE_INPUT_MT || USE_OUTPUT_MT */

  return 0;
}

// Initiate connections with the Consumer and Supplier Peers.

template <class SH, class CH> int
ACE_Event_Channel<SH, CH>::initiate_connections (void)
{
  CONNECTION_MAP_ITERATOR cti (this->connection_map_);
				
  ACE_Synch_Options synch_options;
  
  if (this->blocking_semantics_ == ACE_NONBLOCK)
    synch_options = ACE_Synch_Options::asynch;
  else 
    synch_options = ACE_Synch_Options::synch;

  // Iterate through the Consumer Map connecting all the IO_Handlers.

  for (CONNECTION_MAP_ENTRY *me = 0;
       cti.next (me) != 0;
       cti.advance ())
    {
      IO_Handler *io_handler = me->int_id_;

      if (this->connector_->initiate_connection 
	  (io_handler, synch_options) == -1)
	continue;
    }

  return 0;
}

// This method gracefully shuts down all the Handlers in the
// IO_Handler Connection Map.

template <class SH, class CH> int
ACE_Event_Channel<SH, CH>::close (void)
{
#if defined (USE_INPUT_MT) || defined (USE_OUTPUT_MT)
  ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads\n"));
  if (ACE_Service_Config::thr_mgr ()->suspend_all () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
#endif /* USE_INPUT_MT || USE_OUTPUT_MT */

  CONNECTION_MAP_ITERATOR cti (this->connection_map_);

  // Iterate over all the handlers and shut them down.

  for (CONNECTION_MAP_ENTRY *me;
       cti.next (me) != 0; 
       cti.advance ())
    {
      IO_Handler *io_handler = me->int_id_;

      ACE_DEBUG ((LM_DEBUG, "(%t) closing down route %d\n",
		 io_handler->id ()));

      if (io_handler->state () != IO_Handler::IDLE)
	// Mark IO_Handler as DISCONNECTING so we don't try to
	// reconnect...
	io_handler->state (IO_Handler::DISCONNECTING);

      // Deallocate IO_Handler resources.
      io_handler->destroy (); // Will trigger a delete.
    }

  // Free up the resources allocated dynamically by the ACE_Connector.
  delete this->connector_;
  return 0;
}

template <class SH, class CH> int
ACE_Event_Channel<SH, CH>::open (int argc, char *argv[])
{
  this->parse_args (argc, argv);

  ACE_NEW_RETURN (this->connector_, IO_Handler_Connector (), -1);

  if (this->active_connector_role_)
    {
      // Parse the connection configuration file.
      this->parse_connection_config_file ();

      // Parse the consumer map config file and build the consumer map.
      this->parse_consumer_config_file ();

      // Initiate connections with the peers.
      this->initiate_connections ();
    }

  // If this->performance_window_ > 0 start a timer.

  if (this->performance_window_ > 0)
    {
      if (ACE_Service_Config::reactor ()->schedule_timer 
	  (this, 0, this->performance_window_) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "schedule_timer"));
      else
	ACE_DEBUG ((LM_DEBUG, "starting timer for %d seconds...\n", 
		   this->performance_window_));
    }

  return 0;
}

// Parse and build the connection table.

template <class SH, class CH> int
ACE_Event_Channel<SH, CH>::parse_connection_config_file (void)
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
		    "direction = %c, max retry timeout = %d, local port = %d\n",
		    entry.conn_id_, 
		    entry.host_, 
		    entry.remote_poconsumer_, 
		    entry.direction_,
		    entry.max_retry_delay_, 
		    entry.local_poconsumer_));

      IO_Handler *io_handler = 0;

      // The next few lines of code are dependent on whether we are
      // making an Supplier_Handler or an Consumer_Handler.

      if (entry.direction_ == 'C') // Configure a Consumer_Handler.
	ACE_NEW_RETURN (io_handler,
			CONSUMER_HANDLER (&this->consumer_map_, 
					  this->connector_, 
					  ACE_Service_Config::thr_mgr (),
					  this->socket_queue_size_),
			-1);
      else /* direction == 'S' */ // Configure a Supplier_Handler.
	ACE_NEW_RETURN (io_handler,
			SUPPLIER_HANDLER (&this->consumer_map_, 
					  this->connector_, 
					  ACE_Service_Config::thr_mgr (),
					  this->socket_queue_size_),
			-1);

      // The following code is common to both Supplier_Handlers_ and
      // Consumer_Handlers.

      // Initialize the routing entry's peer addressing info.
      io_handler->bind (ACE_INET_Addr (entry.remote_poconsumer_, entry.host_), 
			ACE_INET_Addr (entry.local_poconsumer_), entry.conn_id_);

      // Initialize max timeout.
      io_handler->max_timeout (entry.max_retry_delay_);

      // Try to bind the new IO_Handler to the connection ID.
      switch (this->connection_map_.bind (entry.conn_id_, io_handler))
	{
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR, 
			     "(%t) bind failed for connection %d\n", 
			    entry.conn_id_), -1);
	  /* NOTREACHED */
	case 1: // Oops, found a duplicate!
	  ACE_DEBUG ((LM_DEBUG, 
		      "(%t) duplicate connection %d, already bound\n", 
		     entry.conn_id_));
	  break;
	case 0:
	  // Success.
	  break;
	}
    }

  if (file_empty)
    ACE_ERROR ((LM_WARNING, 
	       "warning: connection io_handler configuration file was empty\n"));
  return 0;
}

template <class SH, class CH> int
ACE_Event_Channel<SH, CH>::parse_consumer_config_file (void)
{
  // File that contains the consumer map configuration information.
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
		      "number of destinations = %d\n",
		      entry.conn_id_, 
		      entry.logical_id_, 
		      entry.payload_type_,
		      entry.total_destinations_));
	  for (int i = 0; i < entry.total_destinations_; i++)
	    ACE_DEBUG ((LM_DEBUG, "(%t) destination[%d] = %d\n",
		       i, entry.destinations_[i]));
	}

      Consumer_Entry *re;
      ACE_NEW_RETURN (re, Consumer_Entry, -1);

      Consumer_Entry::ENTRY_SET *io_handler_set;
      ACE_NEW_RETURN (io_handler_set, Consumer_Entry::ENTRY_SET, -1);

      Event_Addr event_addr (entry.conn_id_, 
			    entry.logical_id_, 
			    entry.payload_type_);

      // Add the destinations to the Routing Entry.
      for (int i = 0; i < entry.total_destinations_; i++)
	{
	  IO_Handler *io_handler = 0;

	  // Lookup destination and add to Consumer_Entry set if found.
	  if (this->connection_map_.find (entry.destinations_[i], 
					  io_handler) != -1)
	    io_handler_set->insert (io_handler);
	  else
	    ACE_ERROR ((LM_ERROR, "(%t) not found: destination[%d] = %d\n", 
		       i, entry.destinations_[i]));
	}

      // Attach set of destination io_handlers to routing entry.
      re->destinations (io_handler_set);

      // Bind with consumer map, keyed by peer address.
      switch (this->consumer_map_.bind (event_addr, re))
	{
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR, "(%t) bind failed for connection %d\n", 
			    entry.conn_id_), -1);
	  /* NOTREACHED */
	case 1: // Oops, found a duplicate!
	  ACE_DEBUG ((LM_DEBUG, "(%t) duplicate consumer map entry %d, "
		     "already bound\n", entry.conn_id_));
	  break;
	case 0:
	  // Success.
	  break;
	}
    }

  if (file_empty)
    ACE_ERROR ((LM_WARNING, 
	       "warning: consumer map configuration file was empty\n"));
  return 0;
}

// Parse the "command-line" arguments and set the corresponding flags.

template <class SH, class CH> int
ACE_Event_Channel<SH, CH>::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "bc:dpr:q:w:", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'b': // Use blocking connection establishment.
	  this->blocking_semantics_ = 0;
	  break;
	case 'c':
	  this->connection_config_file_ = get_opt.optarg;
	  break;
	case 'd':
	  this->debug_ = 1;
	  break;
	case 'p':
	  // We are not playing the active Connector role.
	  this->active_connector_role_ = 0;
	  break;
	case 'q':
	  this->socket_queue_size_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'r':
	  this->consumer_config_file_ = get_opt.optarg;
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

#endif /* ACE_EVENT_CHANNEL_C */
