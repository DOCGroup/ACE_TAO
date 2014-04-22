// $Id$

#include "Server_i.h"
#include "OBV_impl.h"
#include "tao/ORB_Core.h"

// Set the ORB pointer, register OBV factories and init the
// lists declared above.

void
Checkpoint_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);

  TAO_OBV_REGISTER_FACTORY (Event_factory, Event);
}


// Sidebar on assignment from pointer to _var type.
//
//   Event_var e_var (event);
// This is fatal because the reference counter in *event is not increased
// when a pointer is assigned to a _var. (This happens only
// in assignments from another Event_var;
// But Assignment from new is fine, the referencecount is yet one.)
//
// ok is
//   CORBA::add_ref (event);
//   Event_var e_var (event);
//
// but possibly easier to read it a macro
#define DUP_REF(vt_ptr) (CORBA::add_ref (vt_ptr), vt_ptr)
//   then it reads: Event_var e_var (DUP_REF (event));
// (But it may cause desaster if vt_ptr is a function call.)
//
// Be careful with assignments from T1_var to T2_var, if T1 and T2 are
// related through inheritance. Because there are many implicit casts
// between pointer and _var types possible, the reference counter of
// the duplicated reference is not increased.
//
// Is there any general solution with faulty reference counting ?


void
Checkpoint_i::put_event (Event *event)
{
  // Let us see what event has arrived
  event->do_print ();
}

// Shutdown the server application.

void
Checkpoint_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nThe Checkpoint server is shutting down\n"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

// Constructor.
Checkpoint_i::Checkpoint_i (void)
{
}

// Destructor.
Checkpoint_i::~Checkpoint_i (void)
{
}
