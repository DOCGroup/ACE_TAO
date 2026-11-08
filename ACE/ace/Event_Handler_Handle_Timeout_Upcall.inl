#include "ace/Event_Handler.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
ACE_Event_Handler_Handle_Timeout_Upcall::
registration (ACE_Timer_Queue &,
              ACE_Event_Handler *event_handler,
              void const *)
{
  event_handler->add_reference ();
  return 0;
}

ACE_INLINE int
ACE_Event_Handler_Handle_Timeout_Upcall::
preinvoke (ACE_Timer_Queue &,
          ACE_Event_Handler *event_handler,
          void const *,
          int,
          ACE_Time_Value const &,
          void const *&upcall_act)
{
  bool const requires_reference_counting =
    event_handler->reference_counting_policy ().value () ==
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED;

  if (requires_reference_counting)
    {
      event_handler->add_reference ();

      upcall_act = &this->requires_reference_counting_;
    }

  return 0;
}

ACE_INLINE int
ACE_Event_Handler_Handle_Timeout_Upcall::
postinvoke (ACE_Timer_Queue & /* timer_queue */,
            ACE_Event_Handler *event_handler,
            void const * /* timer_act */,
            int /* recurring_timer */,
            ACE_Time_Value const & /* cur_time */,
            void const *upcall_act)
{
  if (upcall_act == &this->requires_reference_counting_)
    {
      event_handler->remove_reference ();
    }

  return 0;
}

ACE_INLINE int
ACE_Event_Handler_Handle_Timeout_Upcall::
cancel_timer (ACE_Timer_Queue &,
              ACE_Event_Handler *event_handler,
              int,
              int requires_reference_counting)
{
  if (requires_reference_counting)
    event_handler->remove_reference ();

  return 0;
}


ACE_END_VERSIONED_NAMESPACE_DECL
