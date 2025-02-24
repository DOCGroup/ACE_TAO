#include "Reactor_Impl.h"

/// ACE_Single_Input_Reactor is a reactor implementation with extremely limited functionality.
///
/// It is designed for use by TAO in cases where there is only a single input to de-multiplex
/// so select() is not needed.  The single event handler which is registered will be treated
/// as always ready for input.  Other event types (write, exception) are not supported.
/// Handling events with a timeout is not supported.  Notification is not supported.
///
/// When used by TAO, this reactor is suitable for running on a dedicated thread which will
/// only handle the acceptor's socket (see -ORBConcurrency thread-per-connection).
class ACE_Single_Input_Reactor : public ACE_Reactor_Impl {
public:
  int open (size_t size,
            bool restart = 0,
            ACE_Sig_Handler * = nullptr,
            ACE_Timer_Queue * = nullptr,
            int disable_notify_pipe = 0,
            ACE_Reactor_Notify * = nullptr) override;

  int current_info (ACE_HANDLE, size_t &) override;

  int set_sig_handler (ACE_Sig_Handler *signal_handler) override;

  int timer_queue (ACE_Timer_Queue *tq) override;

  ACE_Timer_Queue *timer_queue () const override;

  int close () override;

  int work_pending (const ACE_Time_Value &max_wait_time =  ACE_Time_Value::zero) override;

  int handle_events (ACE_Time_Value *max_wait_time = nullptr) override;
  int alertable_handle_events (ACE_Time_Value *max_wait_time = nullptr) override;
  int handle_events (ACE_Time_Value &max_wait_time) override;
  int alertable_handle_events (ACE_Time_Value &max_wait_time) override;

  int deactivated () override;
  void deactivate (int do_stop) override;

  int register_handler (ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask mask) override;
  int register_handler (ACE_HANDLE io_handle,
                        ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask mask) override;
#ifdef ACE_WIN32
  int register_handler (ACE_Event_Handler *event_handler,
                        ACE_HANDLE event_handle = ACE_INVALID_HANDLE) override;
#endif
  int register_handler (ACE_HANDLE event_handle,
                        ACE_HANDLE io_handle,
                        ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask mask) override;
  int register_handler (const ACE_Handle_Set &handles,
                        ACE_Event_Handler *event_handler,
                        ACE_Reactor_Mask mask) override;
  int register_handler (int signum,
                        ACE_Event_Handler *new_sh,
                        ACE_Sig_Action *new_disp = nullptr,
                        ACE_Event_Handler **old_sh = nullptr,
                        ACE_Sig_Action *old_disp = nullptr) override;
  int register_handler (const ACE_Sig_Set &sigset,
                        ACE_Event_Handler *new_sh,
                        ACE_Sig_Action *new_disp = nullptr) override;

  int remove_handler (ACE_Event_Handler *event_handler,
                      ACE_Reactor_Mask mask) override;
  int remove_handler (ACE_HANDLE handle,
                      ACE_Reactor_Mask mask) override;
  int remove_handler (const ACE_Handle_Set &handle_set,
                      ACE_Reactor_Mask mask) override;
  int remove_handler (int signum,
                      ACE_Sig_Action *new_disp,
                      ACE_Sig_Action *old_disp = nullptr,
                      int sigkey = -1) override;
  int remove_handler (const ACE_Sig_Set &sigset) override;

  int suspend_handler (ACE_Event_Handler *event_handler) override;
  int suspend_handler (ACE_HANDLE handle) override;
  int suspend_handler (const ACE_Handle_Set &handles) override;

  int suspend_handlers () override;

  int resume_handler (ACE_Event_Handler *event_handler) override;
  int resume_handler (ACE_HANDLE handle) override;
  int resume_handler (const ACE_Handle_Set &handles) override;

  int resume_handlers () override;
  int resumable_handler () override;
  bool uses_event_associations () override;

  long schedule_timer (ACE_Event_Handler *event_handler,
                       const void *arg,
                       const ACE_Time_Value &delay,
                       const ACE_Time_Value &interval = ACE_Time_Value::zero) override;

  int reset_timer_interval (long timer_id,
                            const ACE_Time_Value &interval) override;

  int cancel_timer (ACE_Event_Handler *event_handler,
                    int dont_call_handle_close = 1) override;
  int cancel_timer (long timer_id,
                    const void **arg = nullptr,
                    int dont_call_handle_close = 1) override;

  int schedule_wakeup (ACE_Event_Handler *event_handler,
                       ACE_Reactor_Mask masks_to_be_added) override;
  int schedule_wakeup (ACE_HANDLE handle,
                       ACE_Reactor_Mask masks_to_be_added) override;

  int cancel_wakeup (ACE_Event_Handler *event_handler,
                     ACE_Reactor_Mask masks_to_be_cleared) override;
  int cancel_wakeup (ACE_HANDLE handle,
                     ACE_Reactor_Mask masks_to_be_cleared) override;

  int notify (ACE_Event_Handler *event_handler = nullptr,
              ACE_Reactor_Mask mask = ACE_Event_Handler::EXCEPT_MASK,
              ACE_Time_Value * = nullptr) override;

  void max_notify_iterations (int) override;
  int max_notify_iterations () override;

  int purge_pending_notifications (ACE_Event_Handler * = nullptr,
                                   ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK) override;

  ACE_Event_Handler *find_handler (ACE_HANDLE handle) override;

  int handler (ACE_HANDLE handle,
               ACE_Reactor_Mask mask,
               ACE_Event_Handler **event_handler = nullptr) override;
  int handler (int signum,
               ACE_Event_Handler **event_handler = nullptr) override;

  bool initialized () override;
  size_t size () const override;
  ACE_Lock &lock () override;
  void wakeup_all_threads () override;

  int owner (ACE_thread_t new_owner, ACE_thread_t *old_owner = nullptr) override;
  int owner (ACE_thread_t *owner) override;

  bool restart () override;
  bool restart (bool) override;

  void requeue_position (int) override;
  int requeue_position () override;

  int mask_ops (ACE_Event_Handler *event_handler,
                ACE_Reactor_Mask mask,
                int ops) override;
  int mask_ops (ACE_HANDLE handle,
                ACE_Reactor_Mask mask,
                int ops) override;

  int ready_ops (ACE_Event_Handler *event_handler,
                 ACE_Reactor_Mask mask,
                 int ops) override;
  int ready_ops (ACE_HANDLE handle,
                 ACE_Reactor_Mask,
                 int ops) override;

  void dump () const override;

private:
  ACE_Event_Handler_var event_handler_;
};

