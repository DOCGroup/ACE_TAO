//=============================================================================
/**
 *  @file    supplier.h
 *
 *  This class implements  driver for the Publish/Subscribe example
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
  Supplier () = default;

  /// Destructor.
  ~Supplier () override = default;

  Supplier (Supplier const &) = delete;
  Supplier &operator= (Supplier const &) = delete;
  Supplier (Supplier &&) = delete;
  Supplier &operator= (Supplier &&) = delete;

  /// Initialization method. returns 0 on success, -1 on error.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Execute the supplier.
  void run ();

  /// Shutdown the application.
  void close () override;

private:
  /// Handle shutdown signals.
  int handle_signal (int signum,
                     siginfo_t *,
                     ucontext_t *) override;

  /// Handler for keyboard input.
  Supplier_Input_Handler ih_;

  /// The notifier handler.
  Notifier_Handler nh_;
};
