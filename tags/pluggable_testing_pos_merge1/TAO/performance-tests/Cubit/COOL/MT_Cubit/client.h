// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    client.h
//
// = AUTHOR
//    Andy Gokhale, Brian Mendel, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#include <corba/cool.H>
#include "cubit.H"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "Task_Client.h"
#include "Util_Thread.h"

#if defined (CHORUS)
#include "pccTimer.h"
#endif /* CHORUS */

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* defined (VME_DRIVER) */

