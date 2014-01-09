// -*- C++ -*-

//=============================================================================
/**
 *  @file    supplier.h
 *
 *  $Id$
 *
 *  This class implements  driver for the Publish/Subscribe example
 *
 *
 */
//=============================================================================



/**
 * @class Supplier
 *
 * @brief Supplier driver for the TAO Publish/Subscribe example.
 *
 * This class starts up the <Supplier_Input_Handler> and
 * <Notifier_Handler> objects.
 */
class Supplier : public ACE_Event_Handler, public ShutdownCallback
{
public:
  // Initialization and Termination methods.
  /// Constructor.
  Supplier (void);

  /// Destructor.
  ~Supplier (void);

  /// Initialization method. returns 0 on success, -1 on error.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Execute the supplier.
  void run (void);

  /// Shutdown the application.
  virtual void close (void);

private:
  /// Handle shutdown signals.
  virtual int handle_signal (int signum,
                             siginfo_t *,
                             ucontext_t *);

  /// Handler for keyboard input.
  Supplier_Input_Handler ih_;

  /// The notifier handler.
  Notifier_Handler nh_;
};
