// $Id$

#include "Kokyu_defs.h"

#if ! defined (__ACE_INLINE__)
#include "Kokyu_defs.inl"
#endif /* __ACE_INLINE__ */

namespace Kokyu
{
   Dispatch_Command::~Dispatch_Command (void)
   {
   }

  DSRT_ConfigInfo::DSRT_ConfigInfo ()
    :sched_policy_ (ACE_SCHED_RR),
     sched_scope_ (ACE_SCOPE_THREAD)
  {
  }

Dispatcher_Attributes::Dispatcher_Attributes()
    :immediate_activation_ (0),
     sched_policy_ (ACE_SCHED_FIFO),
     sched_scope_ (ACE_SCOPE_THREAD),
     base_thread_creation_flags_ (THR_NEW_LWP | THR_BOUND | THR_JOINABLE)
{
}

int Dispatcher_Attributes::thread_creation_flags () const
{
   int thread_creation_flags = base_thread_creation_flags_;

   switch (sched_policy_)
   {
      case ACE_SCHED_FIFO:
         thread_creation_flags |= THR_SCHED_FIFO;
         break;

      case ACE_SCHED_OTHER:
         thread_creation_flags |= THR_SCHED_DEFAULT;
         break;

      case ACE_SCHED_RR:
         thread_creation_flags |= THR_SCHED_RR;
         break;
   }

   switch (sched_scope_)
   {
      case ACE_SCOPE_PROCESS:
      case ACE_SCOPE_LWP:
         thread_creation_flags |= THR_SCOPE_PROCESS;
         break;

      case ACE_SCOPE_THREAD:
      default:
         thread_creation_flags |= THR_SCOPE_SYSTEM;
         break;
   }
   return thread_creation_flags;
}

}

