// $Id$

namespace Kokyu
{
ACE_INLINE
Dispatcher_Task::Dispatcher_Task (const ConfigInfo& config_info,
                                  ACE_Thread_Manager* thr_manager)
  :  ACE_Task<ACE_SYNCH> (thr_manager),
     allocator_ (0),
     curr_config_info_ (config_info),
        deadline_msg_strategy_ (0, 0, 0x7FFFFFFFUL, 0x08000000UL ),
        //bits for static priority = 0
        //max dynamic prio = 2^31 - 1
        //pending offset = 15/16th of the dynamic prio range
        //which means that the LATE population will be in the
        //1/16th part of the range.
        laxity_msg_strategy_ (0, 0, 0x7FFFFFFFUL, 0x08000000UL )
{
   this->init();
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
        const QoSDescriptor& qos_info)
 :command_ (cmd), qos_info_ (qos_info)
{
   this->init_i (qos_info);
}

ACE_INLINE
Dispatch_Queue_Item::Dispatch_Queue_Item (
        const Dispatch_Command* cmd,
        const QoSDescriptor& qos_info,
        ACE_Data_Block *data_block,
        ACE_Allocator* mb_allocator)
 : ACE_Message_Block (data_block, 0, mb_allocator),
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
