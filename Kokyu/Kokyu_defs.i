// $Id$


namespace Kokyu
{

ACE_INLINE
Dispatcher_Attributes::Dispatcher_Attributes()
    :immediate_activation_ (0),
     base_thread_creation_flags_ (THR_NEW_LWP | THR_BOUND | THR_JOINABLE),
     thread_creation_flags_ (base_thread_creation_flags_)
{
   sched_policy (ACE_SCHED_FIFO);
   sched_scope (ACE_SCOPE_PROCESS);
}

ACE_INLINE
void Dispatcher_Attributes::sched_policy(int policy)
{       
   switch (policy)
   {
      case ACE_SCHED_FIFO: 
         thread_creation_flags_ = 
                base_thread_creation_flags_ | THR_SCHED_FIFO;
         break;

      case ACE_SCHED_OTHER:
         thread_creation_flags_ = 
                base_thread_creation_flags_ | THR_SCHED_DEFAULT;
         break;

      case ACE_SCHED_RR:
         thread_creation_flags_ = 
                base_thread_creation_flags_ | THR_SCHED_RR;
         break;
   }
}

ACE_INLINE
void Dispatcher_Attributes::sched_scope(int scope)
{       
   switch (scope)
   {
      case ACE_SCOPE_PROCESS:
      case ACE_SCOPE_LWP:
         thread_creation_flags_ = 
                base_thread_creation_flags_ | THR_SCOPE_PROCESS;
         break;

      case ACE_SCOPE_THREAD: 
      default:        
         thread_creation_flags_ = 
                base_thread_creation_flags_ | THR_SCOPE_SYSTEM;
         break;
   }
}

ACE_INLINE
int Dispatcher_Attributes::thread_creation_flags () const
{
  return thread_creation_flags_;
}

ACE_INLINE
Reordering_Queue_Attributes::Reordering_Queue_Attributes ()
  :static_bit_field_mask_ (0), // 2^(10) - 1
   static_bit_field_shift_ (0), // 10 low order bits
   dynamic_priority_max_ (0x7FFFFFFFUL), // 2^31-1
   dynamic_priority_offset_ (0x08000000UL) // 15/16th of dynamic prio range
{
  //bits for static priority = 0
  //max dynamic prio = 2^31 - 1
  //pending offset = 15/16th of the dynamic prio range
  //which means that the LATE population will be in the
  //1/16th part of the range.
  
  //For the Laxity and Deadline strategies these are the
  //defaults defined in Message_Block.h
  //static_bit_field_mask (0x3FFUL), // 2^(10) - 1
  //static_bit_field_shift (10), // 10 low order bits
  //dynamic_priority_max (0x3FFFFFUL), // 2^(22)-1
  //dynamic_priority_offset (0x200000UL) // 2^(22-1)
}

ACE_INLINE
Dispatch_Command::Dispatch_Command (int dont_delete, 
        ACE_Allocator *allocator)
        :dont_delete_ (dont_delete),
        allocator_ (allocator)
{
}

ACE_INLINE
int Dispatch_Command::can_be_deleted (void) const
{
  return !dont_delete_;
}

ACE_INLINE
void Dispatch_Command::destroy (void)
{
  if (allocator_)
  {
    allocator_->free (this);
  }
  else
  {
    delete this;
  } 
}

}

//to satisfy ACE_Array<ConfigInfo>
ACE_INLINE
int operator != (const Kokyu::ConfigInfo& lhs, 
                 const Kokyu::ConfigInfo& rhs)
{
  return (lhs.preemption_priority_ != rhs.preemption_priority_ ||
          lhs.thread_priority_ != rhs.thread_priority_ ||
          lhs.dispatching_type_ != rhs.dispatching_type_ );
}
