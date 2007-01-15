// -*- C++ -*-
// $Id$ 

// ============================================================================
//
// = LIBRARY
//    TAO/example/Event_Comm
//
// = FILENAME
//    supplier.h
//
// = DESCRIPTION
//    This class implements  driver for the Publish/Subscribe example
//
// = AUTHOR
//
// ============================================================================


class Supplier : public ACE_Event_Handler, public ShutdownCallback
{
  // = TITLE
  //   Supplier driver for the TAO Publish/Subscribe example.
  //
  // = DESCRIPTION
  //    This class starts up the <Supplier_Input_Handler> and
  //    <Notifier_Handler> objects.
public:
  // Initialization and Termination methods.
  Supplier (void);
  // Constructor.

  ~Supplier (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initialization method. returns 0 on success, -1 on error.

  void run (void);
  // Execute the supplier.

  virtual void close (void);
  // Shutdown the application.

private:
  virtual int handle_signal (int signum,
			     siginfo_t *,
			     ucontext_t *);
  // Handle shutdown signals.

  Supplier_Input_Handler ih_;
  // Handler for keyboard input.

  Notifier_Handler nh_;
  // The notifier handler.
};
