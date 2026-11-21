
//=============================================================================
/**
 *  @file    bpr_thread.cpp
 *
 *  Exercises drivers for a bounded packet relay, based on threaded timer queues.
 *
 *  @author Chris Gill <cdgill@cs.wustl.edu>
 *  @author Douglas C. Schmidt   <d.schmidt@vanderbilt.edu>
 *
 * Based on the Timer Queue Test example written by Carlos O'Ryan <coryan@cs.wustl.edu>
 * and Douglas C. Schmidt <d.schmidt@vanderbilt.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================

#include "Thread_Bounded_Packet_Relay.h"
#include <memory>
#include <new>

using THREAD_BOUNDED_PACKET_RELAY_DRIVER = Bounded_Packet_Relay_Driver<Thread_Timer_Queue>;

using INPUT_CALLBACK = ACE_Command_Callback<Bounded_Packet_Relay, Bounded_Packet_Relay::ACTION>;

// A snippet from Andrew Marvell (Oliver Cromwell's poet laureate)
static constexpr char input_text [] =
"But ever at my back I hear\n"
" Time's winged chariot hurrying near.";

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // The input wrapper and relay borrow this manager.
  std::unique_ptr<ACE_Thread_Manager> mgr (
    new (std::nothrow) ACE_Thread_Manager);
  if (!mgr)
    {
      errno = ENOMEM;
      return -1;
    }

  std::unique_ptr<Text_Input_Device_Wrapper> input (
    new (std::nothrow) Text_Input_Device_Wrapper (mgr.get (), input_text));
  if (!input)
    {
      errno = ENOMEM;
      return -1;
    }

  std::unique_ptr<Text_Output_Device_Wrapper> output (
    new (std::nothrow) Text_Output_Device_Wrapper);
  if (!output)
    {
      errno = ENOMEM;
      return -1;
    }

  std::unique_ptr<Bounded_Packet_Relay> relay (
    new (std::nothrow) Bounded_Packet_Relay (mgr.get (), input.get (), output.get ()));
  if (!relay)
    {
      errno = ENOMEM;
      return -1;
    }

  // The input wrapper borrows the callback; keep it alive through the driver.
  std::unique_ptr<INPUT_CALLBACK> callback (
    new (std::nothrow) INPUT_CALLBACK (*relay,
                                       &Bounded_Packet_Relay::receive_input));
  if (!callback)
    {
      errno = ENOMEM;
      return -1;
    }
  if (input->set_send_input_msg_cmd (callback.get ()) < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "failed to register input callback"),
                        -1);
    }

  std::unique_ptr<THREAD_BOUNDED_PACKET_RELAY_DRIVER> driver (
    new (std::nothrow) Thread_Bounded_Packet_Relay_Driver (relay.get ()));
  if (!driver)
    {
      errno = ENOMEM;
      return -1;
    }

  return driver->run ();
  // All dynamically allocated memory is released when main() returns.
}

