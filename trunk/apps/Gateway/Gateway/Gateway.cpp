/* -*- C++ -*- */
// $Id$

#define ACE_BUILD_SVC_DLL
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
  ACE_Reactor::end_event_loop();
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

int
Gateway::init (int argc, char *argv[])
{
  // Parse the "command-line" arguments.
  Options::instance ()->parse_args (argc, argv);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive signals so we can shut down
  // gracefully.

  if (ACE_Reactor::instance ()->register_handler (sig_set,
                                                  this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%t) %p\n",
                       "register_handler"),
                      -1);

  // Register this handler to receive events on stdin.  We use this to
  // shutdown the Gateway gracefully.
  if (ACE::register_stdin_handler (this,
				   ACE_Reactor::instance (),
				   ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%t) %p\n",
                       "register_stdin_handler"),
                      -1);

  // If this->performance_window_ > 0 start a timer.

  if (Options::instance ()->performance_window () > 0)
    {
      if (ACE_Reactor::instance ()->schedule_timer
	  (&this->event_channel_, 0,
	   Options::instance ()->performance_window ()) == -1)
	ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "schedule_timer"));
      else
	ACE_DEBUG ((LM_DEBUG,
                    "starting timer for %d seconds...\n",
		   Options::instance ()->performance_window ()));
    }

  if (Options::instance ()->enabled (Options::CONSUMER_CONNECTOR | Options::SUPPLIER_CONNECTOR))
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
  // Remove the handler that receive events on stdin.  Otherwise, we
  // will crash on shutdown.
  ACE::remove_stdin_handler (ACE_Reactor::instance (),
                             ACE_Thread_Manager::instance ());

  // Close down the event channel.
  this->event_channel_.close ();

  // Need to make sure we cleanup this Singleton.
  delete Options::instance ();
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

// Parse and build the proxy table.

int
Gateway::parse_proxy_config_file (void)
{
  // File that contains the proxy configuration information.
  Proxy_Config_File_Parser proxy_file;
  int file_empty = 1;
  int line_number = 0;

  if (proxy_file.open (Options::instance ()->proxy_config_file ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "(%t) %p\n",
		       Options::instance ()->proxy_config_file ()),
		      -1);

  // Read config file one line at a time.
  for (Proxy_Config_Info pci;
       proxy_file.read_entry (pci, line_number) != FP::EOFILE;
       )
    {
      file_empty = 0;

      if (Options::instance ()->enabled (Options::DEBUG))
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
	ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "make_proxy_handler"),
                          -1);

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

  if (consumer_file.open (Options::instance ()->consumer_config_file ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%t) %p\n",
                       Options::instance ()->consumer_config_file ()),
                      -1);

  // Read config file line at a time.
  for (Consumer_Config_Info cci;
       consumer_file.read_entry (cci, line_number) != FP::EOFILE;
       )
    {
      file_empty = 0;

      if (Options::instance ()->enabled (Options::DEBUG))
	{
	  ACE_DEBUG ((LM_DEBUG,
                      "(%t) conn id = %d, supplier id = %d, payload = %d, "
		      "number of consumers = %d\n",
		      cci.proxy_id_,
		      cci.supplier_id_,
		      cci.type_,
		      cci.total_consumers_));

	  for (int i = 0; i < cci.total_consumers_; i++)
	    ACE_DEBUG ((LM_DEBUG,
                        "(%t) destination[%d] = %d\n",
                        i,
                        cci.consumers_[i]));
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<Proxy_Handler *>;
template class ACE_Unbounded_Set<Proxy_Handler *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<Proxy_Handler *>
#pragma instantiate ACE_Unbounded_Set<Proxy_Handler *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

