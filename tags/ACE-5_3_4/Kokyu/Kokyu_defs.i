// $Id$


namespace Kokyu
{

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
