// $Id$

#include "Consumer_Handler.h"
#include "Consumer_Input_Handler.h"

ACE_RCSID(Consumer, consumer, "$Id$")

class Consumer : public ACE_Event_Handler
{
  // = TITLE
  // Consumer driver for the Publish/Subscribe example.
public:
  Consumer (int argc, char *argv[]);
  ~Consumer (void);

  void run (void);
  // Execute the consumer;

private:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

  Consumer_Input_Handler *ih_;
  // Handler for keyboard input.

  Consumer_Handler *ch_;
  // Handler for CORBA Consumer.
};

int
Consumer::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              "closing down Consumer\n"));
  return 0;
}

int
Consumer::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "%S\n",
              signum));

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
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "run_reactor_event_loop"));
}

Consumer::Consumer (int argc, char *argv[])
  : ih_ (0),
    ch_ (0)
{
  ACE_DEBUG ((LM_DEBUG,
	      "no config file, using static binding\n"));
  // The constructor registers the handlers...
  ACE_NEW (this->ch_,
	   Consumer_Handler (argc, argv));
  ACE_NEW (this->ih_,
	   Consumer_Input_Handler (this->ch_));

  if (ACE_Reactor::instance ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "register_handler"));
}

Consumer::~Consumer (void)
{
  // Free up the handlers if they were statically bound.
  this->ih_->handle_close ();
  this->ch_->handle_close ();
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
