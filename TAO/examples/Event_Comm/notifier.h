//=============================================================================
/**
 *  @file    notifier.h
 *
 *  This class implements notifier  driver for the Publish/Subscribe example
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
  /// Constructor.
  Notifier (int argc, ACE_TCHAR *argv[]);

  /// Destructor.
  ~Notifier () override;

  Notifier (Notifier const &) = delete;
  Notifier &operator= (Notifier const &) = delete;
  Notifier (Notifier &&) = delete;
  Notifier &operator= (Notifier &&) = delete;

  /// Execute the notifier.
  void run ();

 private:
  /// Handle signals that shut us down.
  int handle_signal (int signum,
                     siginfo_t *,
                     ucontext_t *) override;

  /// The notifier server.
  Notifier_Server ns_;
};
