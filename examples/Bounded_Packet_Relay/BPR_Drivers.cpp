// $Id$

// ============================================================================
// = LIBRARY
//    examples
//
// = FILENAME
//    BPR_Driver.cpp
//
// = DESCRIPTION
//    This code builds an abstraction to factor out common code for
//    the different implementations of the Timer_Queue.
//
// = AUTHORS
//    Chris Gill           <cdgill@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu>
//
//    Based on the Timer Queue Test example written by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#if !defined (_BPR_DRIVER_CPP_)
#define _BPR_DRIVER_CPP_

#include "BPR_Drivers.h"

ACE_RCSID(Bounded_Packet_Relay, BPR_Drivers, "$Id$")

// Constructor.

Input_Device_Wrapper_Base::Input_Device_Wrapper_Base (ACE_Thread_Manager *input_task_mgr)
  : ACE_Task_Base (input_task_mgr),
    send_input_msg_cmd_ (0),
    input_period_ (ACE_ONE_SECOND_IN_USECS),
    is_active_ (0),
	send_count_ (0)
{
}

// Destructor.

Input_Device_Wrapper_Base::~Input_Device_Wrapper_Base (void)
{
}
 
// Sets send input message command in the input device driver object.

int
Input_Device_Wrapper_Base::set_send_input_msg_cmd (Command_Base *send_input_msg_cmd)
{
  // Set the new command.  Input device is not responsible 
  // for deleting the old command, if any.
  send_input_msg_cmd_ = send_input_msg_cmd;
  return 0;
}

// Sets period between when input messages are produced.

int 
Input_Device_Wrapper_Base::set_input_period (u_long input_period)
{
  input_period_ = input_period;
  return 0;
}

// Sets count of messages to send.

int 
Input_Device_Wrapper_Base::set_send_count (long count)
{
  send_count_ = count;
  return 0;
}

// Request that the input device stop sending messages
// and terminate its thread.  Should return 1 if it will do so, 0
// if it has already done so, or -1 if there is a problem doing so.

int 
Input_Device_Wrapper_Base::request_stop (void)
{
  if (is_active_)
    {
      is_active_ = 0;
      return 1;
    }

  return 0;
}

// This method runs the input device loop in the new thread.

int 
Input_Device_Wrapper_Base::svc (void)
{
  ACE_Time_Value timeout;
  ACE_Message_Block *message;

  // Set a flag to indicate we're active.
  is_active_ = 1;

  // Start with the total count of messages to send.
  for (current_count_ = send_count_;
       // While we're still marked active, and there are packets to send.
       (is_active_) && (current_count_ != 0);
       )
    {
      // Make sure there is a send command object.
      if (send_input_msg_cmd_ == 0)
        {
          is_active_ = 0;
          ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                             "send message command object not instantiated"), 
                            -1);
        }

      // Create an input message to send.
      message = create_input_message ();
      if (message == 0)
        {
          is_active_ = 0;
          ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                             "Failed to create input message object"), 
                             -1);
        }

      // Send the input message.
      if (send_input_msg_cmd_->execute ((void *) message) < 0)
        {
          is_active_ = 0;
          delete message;
          ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                             "Failed executing send message command object"), 
                            -1);
        }

      // If all went well, decrement count of messages to send, and
      // run the reactor event loop unti we get a timeout or something
      // happens in a registered upcall.
      if (current_count_ > 0) 
        --current_count_;

      timeout = ACE_Time_Value (0, input_period_);
      reactor_.run_event_loop (timeout);
    }

  is_active_ = 0;

  return 0;
}

// Sends a newly created message block, carrying data read from the
// underlying input device, by passing a pointer to the message block
// to its command execution.

int 
Input_Device_Wrapper_Base::send_input_message (ACE_Message_Block *amb)
{
  if (send_input_msg_cmd_)
    return send_input_msg_cmd_->execute ((void *) amb);
  else
    ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", 
                       "Input_Device_Wrapper_Base::send_input_message: "
                       "command object not instantiated"), 
                      -1);
}

#endif /* _BPR_DRIVER_CPP_ */
