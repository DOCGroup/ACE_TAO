//
// $Id$
//
#include "orbsvcs/Event_Utilities.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Event_Utilities.i"
#endif /* __ACE_INLINE__ */

ACE_ConsumerQOS_Factory::ACE_ConsumerQOS_Factory (void) :
  qos_ (),
  designator_set_ (0)
{
}

ACE_ConsumerQOS_Factory::~ACE_ConsumerQOS_Factory (void)
{
}

int
ACE_ConsumerQOS_Factory::start_conjunction_group (void)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  qos_.dependencies[l].event_.type_ = ACE_ES_CONJUNCTION_DESIGNATOR;
  // TODO: qos_.dependencies[l].event_.data_.lval (0);
  designator_set_ = 1;
  return 0;
}

int
ACE_ConsumerQOS_Factory::start_disjunction_group (void)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  qos_.dependencies[l].event_.type_ = ACE_ES_DISJUNCTION_DESIGNATOR;
  // TODO: qos_.dependencies[l].event_.data_.lval (0);
  designator_set_ = 1;
  return 0;
}

int
ACE_ConsumerQOS_Factory::insert (const RtecEventChannelAdmin::Dependency &subscribe)
{
  RtecScheduler::RT_Info dummy;
  // Make sure that a designator is first.
  if (designator_set_ == 0)
    {
      int l = qos_.dependencies.length ();
      qos_.dependencies.length (l + 1);
      qos_.dependencies[l].event_.type_ = ACE_ES_GLOBAL_DESIGNATOR;
      // TODO: IDL union qos_.dependencies[l].event_.data_.lval (0);
      designator_set_ = 1;
    }

  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  qos_.dependencies[l] = subscribe;
  // TODO: IDL union qos_.dependencies[l].event_.data_.lval (0);
  return 0;
}

void event_debug (const char* p,
		  const RtecEventComm::Event& event)
{
  int l = ACE_OS::strlen (p);
  ACE_DEBUG ((LM_DEBUG,
	      "%*.*s - event.source: %d\n"
	      "%*.*s   event.type: %d\n"
	      "%*.*s   event.time: %f\n",
	      l, l, p, event.source_,
	      l, l, p, event.type_,
	      l, l, p, event.creation_time_));
}

void
ACE_ConsumerQOS_Factory::debug (const RtecEventChannelAdmin::ConsumerQOS& qos)
{
  ACE_DEBUG ((LM_DEBUG, "ConsumerQOS { \n"));

  for (u_int i = 0; i < qos.dependencies.length (); ++i)
    {
      char buf[128];
      ACE_OS::sprintf (buf, " dep[%d]", i);
      event_debug (buf, qos.dependencies[i].event_);
      ACE_DEBUG ((LM_DEBUG, "%s  rt_info: %d\n",
		  buf, qos.dependencies[i].rt_info));
    }
  ACE_DEBUG ((LM_DEBUG, "}\n"));
}

// ************************************************************

ACE_SupplierQOS_Factory::ACE_SupplierQOS_Factory (void) :
  qos_ ()
{
}

int
ACE_SupplierQOS_Factory::insert (RtecEventComm::EventSourceID sid,
				 RtecEventComm::EventType type,
				 RtecScheduler::handle_t rt_info,
				 u_int ncalls)
{
  int l = qos_.publications_.length ();
  qos_.publications_.length (l + 1);
  qos_.publications_[l].event_.source_ = sid;
  qos_.publications_[l].event_.type_ = type;
  // TODO: IDL union qos_.publications_[l].event_.data_.lval (0);
  qos_.publications_[l].dependency_info_.rt_info = rt_info;
  qos_.publications_[l].dependency_info_.number_of_calls = ncalls;
  return 0;
}

void ACE_SupplierQOS_Factory::debug (const RtecEventChannelAdmin::SupplierQOS& qos)
{
  ACE_DEBUG ((LM_DEBUG, "ConsumerQOS { \n"));
  for (u_int i = 0; i < qos.publications_.length (); ++i)
    {
      char buf[128];
      ACE_OS::sprintf (buf, " publications[%d]", i);
      event_debug (buf, qos.publications_[i].event_);
      ACE_DEBUG ((LM_DEBUG,
		  "%s   dependency_info.rt_info: %d\n"
		  "%s   dependency_info.number_of_calls: %d\n",
		  buf, qos.publications_[i].dependency_info_.rt_info,
		  buf, qos.publications_[i].dependency_info_.number_of_calls));
	}
  ACE_DEBUG ((LM_DEBUG, "}\n"));

}
