
namespace Kokyu
{

ACE_INLINE
void Dispatcher_Attributes::sched_policy(int policy)
{
  sched_policy_ = policy;
}

ACE_INLINE
void Dispatcher_Attributes::sched_scope(int scope)
{
  sched_scope_ = scope;
}

ACE_INLINE
Reordering_Queue_Attributes::Reordering_Queue_Attributes () = default;

ACE_INLINE
Dispatch_Command::Dispatch_Command (bool dont_delete,
        ACE_Allocator *allocator)
        :dont_delete_ (dont_delete),
        allocator_ (allocator)
{
}

ACE_INLINE
bool Dispatch_Command::can_be_deleted () const
{
  return !dont_delete_;
}

ACE_INLINE
void Dispatch_Command::destroy ()
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

ACE_INLINE
ConfigInfo::ConfigInfo () = default;

}

//to satisfy ACE_Array<ConfigInfo>
ACE_INLINE
bool operator != (Kokyu::ConfigInfo const &lhs,
                  Kokyu::ConfigInfo const &rhs)
{
  return (lhs.preemption_priority_ != rhs.preemption_priority_ ||
          lhs.thread_priority_ != rhs.thread_priority_ ||
          lhs.dispatching_type_ != rhs.dispatching_type_);
}
