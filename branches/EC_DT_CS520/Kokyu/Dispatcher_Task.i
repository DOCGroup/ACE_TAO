// $Id$

namespace Kokyu
{
ACE_INLINE
Dispatcher_Task::Dispatcher_Task (const ConfigInfo& config_info,
                                  ACE_Thread_Manager* thr_manager)
  :  ACE_Task<ACE_SYNCH> (thr_manager),
     curr_config_info_ (config_info),
     allocator_ (config_info.allocator_),
     own_allocator_ (0),
     deadline_msg_strategy_ (config_info.reordering_flags_.static_bit_field_mask_,
                             config_info.reordering_flags_.static_bit_field_shift_,
                             config_info.reordering_flags_.dynamic_priority_max_,
                             config_info.reordering_flags_.dynamic_priority_offset_),
     laxity_msg_strategy_ (config_info.reordering_flags_.static_bit_field_mask_,
                           config_info.reordering_flags_.static_bit_field_shift_,
                           config_info.reordering_flags_.dynamic_priority_max_,
                           config_info.reordering_flags_.dynamic_priority_offset_)
#ifdef KOKYU_HAS_RELEASE_GUARD
  ,  releases_()
  ,  deferrer_attr_()
  ,  deferrer_()
#endif //KOKYU_HAS_RELEASE_GUARD
{
   this->initialize();
}

ACE_INLINE
Dispatcher_Task::~Dispatcher_Task ()
{
   if (own_allocator_)
   {
      delete allocator_;
   }
}

ACE_INLINE
Priority_t
Dispatcher_Task::preemption_priority() const
{
    return curr_config_info_.preemption_priority_;
}


ACE_INLINE
const ConfigInfo&
Dispatcher_Task::get_curr_config_info() const
{
    return curr_config_info_;
}

ACE_INLINE
int 
Dispatcher_Task::enqueue (Dispatch_Queue_Item *qitem)
{
#ifdef KOKYU_HAS_RELEASE_GUARD
  //update release time
  //TODO: want release time before or after enqueuing call?
  ACE_Time_Value release = ACE_OS::gettimeofday();
#endif //KOKYU_HAS_RELEASE_GUARD

  this->putq (qitem);

#ifdef KOKYU_HAS_RELEASE_GUARD
  //if qos_info is not in map, this should add it
  this->releases_.rebind(qitem->qos_info(),release.msec());
#endif //KOKYU_HAS_RELEASE_GUARD

  return 0;
}

ACE_INLINE
Dispatch_Queue_Item::Dispatch_Queue_Item (
        const Dispatch_Command* cmd,
        const QoSDescriptor& qos_info,
        ACE_Data_Block *data_block,
        int flags,
        ACE_Allocator* mb_allocator)
 : ACE_Message_Block (data_block, 
                      flags, 
                      mb_allocator),
   command_ (cmd), qos_info_ (qos_info)

{
   this->init_i (qos_info);
}

ACE_INLINE
Dispatch_Queue_Item::Dispatch_Queue_Item (
        const Dispatch_Command* cmd,
        const QoSDescriptor& qos_info,
        ACE_Allocator* mb_allocator)
 : ACE_Message_Block (mb_allocator),
   command_ (cmd), qos_info_ (qos_info)
{
   this->init_i (qos_info);
}

ACE_INLINE
Dispatch_Command*
Dispatch_Queue_Item::command()
{
  return const_cast<Dispatch_Command*> (command_);
}

ACE_INLINE
const QoSDescriptor&
Dispatch_Queue_Item::qos_info() const
{
  return this->qos_info_;
}

} //namespace Kokyu
