
//=============================================================================
/**
 *  @file    notifier.h
 *
 *  $Id$
 *
 *  This class implements notifier  driver for the Publish/Subscribe example
 *
 *
 */
//=============================================================================




 /**
  * @class Notifier
  *
  * @brief Notifier driver for the TAO Publish/Subscribe example.
  *
  * The driver class for the <Event_Comm::Notifier> object.
  */
class Notifier : public ACE_Event_Handler
{
 public:
  // = Initialization and termination methods.
  /// Constructor.
  Notifier (int argc, ACE_TCHAR *argv[]);

  /// Destructor.
  ~Notifier (void);

  /// Execute the notifier.
  void run (void);

 private:
  /// Handle signals that shut us down.
  virtual int handle_signal (int signum,
                             siginfo_t *,
                             ucontext_t *);

  /// The notifier server.
  Notifier_Server ns_;
};
