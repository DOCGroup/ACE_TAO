// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/example/Event_Comm
//
// = FILENAME
//    notifier.h
//
// = DESCRIPTION
//    This class implements notifier  driver for the Publish/Subscribe example
//
// = AUTHOR
//
// ============================================================================



class Notifier : public ACE_Event_Handler
{
  // = TITLE
  //   Notifier driver for the TAO Publish/Subscribe example.
  //
  // = DESCRIPTION
  //    The driver class for the <Event_Comm::Notifier> object.
 public:
  // = Initialization and termination methods.
  Notifier (int argc, char *argv[]);
  // Constructor.
  
  ~Notifier (void);
  // Destructor.
  
  void run (void);
  // Execute the notifier.
  
 private:
  virtual int handle_signal (int signum,
			     siginfo_t *,
			     ucontext_t *);
  // Handle signals that shut us down.
  
  Notifier_Server ns_;
  // The notifier server.
};
