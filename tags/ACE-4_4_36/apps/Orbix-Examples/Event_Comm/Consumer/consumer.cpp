/* -*- C++ -*- */
// $Id$

// Consumer driver for the Orbix Notification example.


#include "Notification_Receiver_Handler.h"
#include "Input_Handler.h"

#if defined (ACE_HAS_ORBIX) && (ACE_HAS_ORBIX != 0)

class Consumer : public ACE_Event_Handler
{
public:
  Consumer (int argc, char *argv[]);
  ~Consumer (void);
  
  void run (void);
  // Execute the consumer;
  
private:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

  Input_Handler *ih_;
  // Handler for keyboard input.

  Notification_Receiver_Handler *nrh_;
  // Handler for CORBA Consumer.

  ACE_Service_Config daemon_;
  // ACE server event-loop mechanism.
};

int
Consumer::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "closing down Consumer\n"));
  return 0;
}

int 
Consumer::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "%S\n", signum));

  // Indicate that the consumer initiated the shutdown.
  this->ih_->consumer_initiated_shutdown (1);

  // Shut down the event loop.
  ACE_Reactor::end_event_loop();
  return 0;
}

// Run the event loop until someone calls
// calls ACE_Reactor::end_event_loop().

void
Consumer::run (void)
{
  if (ACE_Reactor::run_event_loop () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "run_reactor_event_loop"));
}

Consumer::Consumer (int argc, char *argv[])
  : ih_ (0), 
    nrh_ (0)
{
  // Initialize the server.
  if (this->daemon_.open (argc, argv) == -1)
    {
      if (errno == ENOENT) // There's no svc.conf file, so use static linking...
	{
	  ACE_DEBUG ((LM_DEBUG, "no config file, using static binding\n"));
	  // The constructor registers the handlers...
	  this->nrh_ = new Notification_Receiver_Handler (argc, argv);
	  ACE_ASSERT (this->nrh_ != 0);
	  this->ih_ = new Input_Handler (this->nrh_);
	  ACE_ASSERT (this->ih_ != 0);
	}
      else
	ACE_ERROR ((LM_ERROR, "%p\n%a", "open", 1));
    }

  if (ACE_Reactor::instance ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
}

Consumer::~Consumer (void)
{
  // Free up the handlers if they were statically bound.
  this->ih_->handle_close ();
  this->nrh_->handle_close ();
}

int
main (int argc, char *argv[]) 
{
  // Initialize the supplier and consumer object references.
  Consumer consumer (argc, argv);

  // Loop forever handling events.
  consumer.run ();

  return 0;
}
#else /* !defined ACE_HAS_ORBIX */
int
main (int argc, char *argv[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "you must have Orbix to run application %s\n", argv[0]), 1);
}
#endif /* ACE_HAS_ORBIX */
