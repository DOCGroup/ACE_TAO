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

int
main (int, char *[])
{
  // Auto ptr ensures that the driver memory is released
  // automatically.
  THREAD_BOUNDED_PACKET_RELAY_DRIVER *tbprd;
  ACE_NEW_RETURN (tbprd,
                  Thread_Bounded_Packet_Relay_Driver,
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
