// $Id$

// Driver for the peer daemon (peerd).  Note that this is completely
// generic code due to the Service Configurator framework!

#include "ace/Service_Config.h"
#include "Peer.h"

class Service_Ptr
  // = TITLE
  //     Holds the <ACE_Service_Object> * until we're done. 
{
public:
  Service_Ptr (ACE_Service_Object *so)
    : service_object_ (so) {}

  ~Service_Ptr (void) { this->service_object_->fini (); }

  ACE_Service_Object *operator-> () { return this->service_object_; }

private:
  ACE_Service_Object *service_object_;
  // Holds the service object until we're done.
};

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;

  if (daemon.open (argc, argv) == -1)
    {
      if (errno != ENOENT)
	ACE_ERROR ((LM_ERROR, "%p\n%a", "open", 1));
      else // Use static linking.
	{
	  Service_Ptr sp = ACE_SVC_INVOKE (Peer_Acceptor);
	  
	  if (sp->init (argc - 1, argv + 1) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "init", 1));
	  
	  // Run forever, performing the configured services until we
	  // are shut down by a SIGINT/SIGQUIT signal.

	  daemon.run_reactor_event_loop ();

	  // Destructors of Service_Ptr's automagically call fini().

	}
    }
  else // Use dynamic linking.

    // Run forever, performing the configured services until we are shut
    // down by a SIGINT/SIGQUIT signal.
    
    daemon.run_reactor_event_loop ();

  return 0;
}
