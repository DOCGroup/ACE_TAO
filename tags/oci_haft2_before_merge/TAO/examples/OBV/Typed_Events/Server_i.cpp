// $Id$

#include "Server_i.h"
#include "Event_Types_impl.h"
#include "tao/ORB_Core.h"

Event_List_var alarms;
// The list of events which caused an alarm. ('Singleton')

Criterion_List_var criterions;
// Arriving events are tested against the list members.



// Set the ORB pointer, register OBV factories and init the
// lists declared above.

void
Checkpoint_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);

  // Make factories to unmarshal OBV. The macro TAO_OBV_REGISTER_FACTORY is defined
  // in tao/ValueFactory.h to prevent from getting wound fingertips.
  // Notice that the way we currently do register_factory is not OMG conform.

  // TAO_OBV_REGISTER_FACTORY (Event_factory);
  // This one not. Event behaves as an abstract base,
  // so we should't receive it (see header file)

  TAO_OBV_REGISTER_FACTORY (Temperature_factory, Temperature);
  TAO_OBV_REGISTER_FACTORY (Position_factory, Position);
  TAO_OBV_REGISTER_FACTORY (Log_Msg_factory, Log_Msg);
  TAO_OBV_REGISTER_FACTORY (Event_List_factory, Event_List);
  TAO_OBV_REGISTER_FACTORY (Event_List_Link_factory, Event_List_Link);

  // This list is returned by get_critical_events ().
  alarms = new Event_List_impl;

  // Now we build the list against which the events are tested.
  criterions = new Criterion_List_impl;

  // The conditions which will raise an alarm follow.

  // The first parameter is the origin_id and the second the temperature.
  Temperature_Criterion_var tc (ACE_static_cast(Temperature_Criterion*,
                                                new Temperature_Criterion_impl (KITCHEN, 32)));
  criterions->store_criterion (tc.in ());

  tc = ACE_static_cast(Temperature_Criterion*,
                       new Temperature_Criterion_impl (BATHROOM, 27));
  criterions->store_criterion (tc.in ());


  // Alarm if little Jonas is not in the child's room

  Point leftbottom = {1, 2, 3};
  Position_var leftbottom_v (ACE_static_cast(Position*,
                                             new Position_impl (leftbottom)));
  Point topright = {2, 3, 4};
  Position_var topright_v (ACE_static_cast(Position*,
                                           new Position_impl (topright)));
  Position_Criterion_var pc (ACE_static_cast(Position_Criterion*,
                                             new Position_Criterion_impl (JONAS,
                                                                          leftbottom_v,
                                                                          topright_v)));
  criterions->store_criterion (pc.in ());

  // Generic criterion matches every urgent massage
  Log_Msg_Criterion_var lc (ACE_static_cast(Log_Msg_Criterion*,
                                            new Log_Msg_Criterion_impl));
  // Null id flags to handle every Log_Msg
  lc->origin_id_ (0);
  criterions->store_criterion (lc.in ());

  ACE_DEBUG ((LM_DEBUG, "This criterions are stored:\n"));
  for (Criterion_List_Iterator i (criterions); i.next (); i.advance ())
    {
      Event *e = Event::_downcast (i.next ());
      e->do_print ();
    }
    ACE_DEBUG ((LM_DEBUG, "\nNow waiting for events...\n"));
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
Checkpoint_i::put_event (Event *event
                        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // In general this function would be a dispatcher which looks at the
  // origin id and finds an event handler object for the event. The
  // event handler then would (type-safe) downcast the event to the
  // concrete type it can handle. Here we only apply the list of
  // criterions to the event.


  // Let us see what event has arrived
  event->do_print ();

  // Does it match any criterion for alarm ?
  if (criterions->is_critical (event))
    {
      alarms->store_event (event);
    }



  // If we want to return the event (e.g. return type is Event *)
  // return (DUP_REF (event));
  // should be right.

  return;
}


Event_List *
Checkpoint_i::get_critical_events (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The skeleton should not steal our list by decrementing
  // the reference counter.

  CORBA::add_ref (alarms.ptr ());

  // alarms._retn () would reset and destroy our alarms.

  return alarms.in ();
}


// Shutdown the server application.

void
Checkpoint_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "\n%s\n",
              "The Checkpoint server is shutting down"));

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
