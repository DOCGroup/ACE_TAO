// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    bpr_thread.cpp
//
// = DESCRIPTION
//    Exercises drivers for a bounded packet relay, based on threaded timer queues.  
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

#include "ace/Auto_Ptr.h"
#include "Thread_Bounded_Packet_Relay.h"

ACE_RCSID(Bounded_Packet_Relay, bpr_thread, "$Id$")

typedef Bounded_Packet_Relay_Driver<Thread_Timer_Queue>
	THREAD_BOUNDED_PACKET_RELAY_DRIVER;

typedef Bounded_Packet_Relay<ACE_MT_SYNCH>
	BOUNDED_PACKET_RELAY;

// A snippet from Andrew Marvell (Oliver Cromwell's poet laureate)
const char input_text [] = "But ever at my back I hear\n"
                           "Time's winged chariot hurrying near.\n";

int
main (int, char *[])
{
  // Construct a new thread manager for the input device task.
  // Auto ptr ensures memory is freed when we exit this scope.
  ACE_Thread_Manager *input_task_mgr;
  ACE_NEW_RETURN (input_task_mgr,
                  ACE_Thread_Manager,
                  -1);
  auto_ptr <ACE_Thread_Manager> mgr (input_task_mgr);

  // Construct a new input device wrapper.
  // Auto ptr ensures memory is freed when we exit this scope.
  Text_Input_Device_Wrapper *input_device;
  ACE_NEW_RETURN (input_device,
                  Text_Input_Device_Wrapper (input_task_mgr,
                                             sizeof (input_text), 
                                             input_text),
                  -1);
  auto_ptr <Text_Input_Device_Wrapper> input (input_device);

  // Construct a new output device wrapper.
  // Auto ptr ensures memory is freed when we exit this scope.
  Text_Output_Device_Wrapper *output_device;
  ACE_NEW_RETURN (output_device,
                  Text_Output_Device_Wrapper,
                  -1);
  auto_ptr <Text_Output_Device_Wrapper> output (output_device);

  // Construct a new bounded packet relay.
  // Auto ptr ensures memory is freed when we exit this scope.
  BOUNDED_PACKET_RELAY *packet_relay;
  ACE_NEW_RETURN (packet_relay,
                  BOUNDED_PACKET_RELAY (input_task_mgr,
                                        input_device,
                                        output_device),
                  -1);
  auto_ptr <BOUNDED_PACKET_RELAY> relay (packet_relay);


  // Construct a new bounded packet relay driver.
  // Auto ptr ensures memory is freed when we exit this scope.
  THREAD_BOUNDED_PACKET_RELAY_DRIVER *tbprd;
  ACE_NEW_RETURN (tbprd,
                  Thread_Bounded_Packet_Relay_Driver (packet_relay),
                  -1);
  auto_ptr <THREAD_BOUNDED_PACKET_RELAY_DRIVER> driver (tbprd);

  return driver->run ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr <THREAD_BOUNDED_PACKET_RELAY_DRIVER>;
template class ACE_Auto_Basic_Ptr <THREAD_BOUNDED_PACKET_RELAY_DRIVER>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr <THREAD_BOUNDED_PACKET_RELAY_DRIVER>
#pragma instantiate ACE_Auto_Basic_Ptr <THREAD_BOUNDED_PACKET_RELAY_DRIVER>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
