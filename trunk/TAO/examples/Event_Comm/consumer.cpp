// $Id$

#include "Consumer_Handler.h"
#include "Consumer_Input_Handler.h"

ACE_RCSID(Consumer, consumer, "$Id$")

class Consumer : public ACE_Event_Handler
{
  // = TITLE
  //    Consumer driver for the Publish/Subscribe example.
  //
  // = DESCRIPTION
  //    The Consumer holds the <Consumer_Input_Handler> and
  //    <Cosumer_Handler> objects.
public:
  // = Initialization and termination methods.
  Consumer (void);
  // Constructor.

  ~Consumer (void);
  // Destructor.

  int initialize (int argc, char *argv[]);
  // Initialization method.

  int run (void);
  // Execute the consumer;

private:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Signal handler method.

  Consumer_Input_Handler ih_;
  // Handler for keyboard input.

  Consumer_Handler ch_;
  // Handler for CORBA Consumer.
};

Consumer::Consumer (void)
{
  // No-Op.
}

Consumer::~Consumer (void)
{
  // Allow the handlers to clean up.
  this->ih_.handle_close();
  this->ch_.handle_close();
}

int
Consumer::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "%S\n",
              signum));

  // Indicate that the consumer initiated the shutdown.
  this->ih_.consumer_initiated_shutdown (1);

  // Shut down the ORB
  ch_.close ();

  return 0;
}

int
Consumer::run (void)
{
  // Run the <Consumer_Handler>'s ORB.

  return ch_.run ();
}

int
Consumer::initialize (int argc, char *argv[])
{
  // Initialize the <Consumer_Handler>.
  if (this->ch_.init (argc, argv) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
			"%p\n",
			"Consumer_Handler failed to initialize\n"), -1);

   // Initialize the <Consumer_Input_Handler>.
  if (this->ih_.initialize (&this->ch_) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
			"%p\n",
			"Consumer_Input_Handler failed to initialize\n"), -1);

  if (this->ch_.reactor()->register_handler (SIGINT,
					     this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "register_handler"), -1);
  return 0;
}

int
main (int argc, char *argv[])
{
  // Initialize the supplier and consumer object references.
  Consumer consumer;

  if (consumer.initialize (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "Consumer init failed\n"),
		      1);

  // Loop forever handling events.
  if (consumer.run () == -1)
  ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "Consumer run failed\n"),
		      1);

  return 0;
}
