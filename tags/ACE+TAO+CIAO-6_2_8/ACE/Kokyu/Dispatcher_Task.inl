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
}
