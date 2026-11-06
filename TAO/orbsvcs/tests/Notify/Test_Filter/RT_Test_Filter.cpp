#include "RT_Test_Filter.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/PortableServer/Root_POA.h"

#include <cstdlib>
#include <cstring>

char*
TAO_Notify_Tests_RT_Test_Filter::constraint_grammar ()
{
  return CORBA::string_dup ("ETCL");
}

CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_Tests_RT_Test_Filter::add_constraints (
  CosNotifyFilter::ConstraintExpSeq const& constraint_list)
{
  char const* const pool_id_prefix = "PoolId = ";
  size_t const pool_id_index = std::strlen (pool_id_prefix);

  char const* const lane_id_prefix = "LaneId = ";
  size_t const lane_id_index = std::strlen (lane_id_prefix);

  for (CORBA::ULong i {}; i < constraint_list.length (); ++i)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "(%t) received  = %s\n", constraint_list[i].constraint_expr.in ()));

      // Check pool id.
      if (std::strncmp (constraint_list[i].constraint_expr.in (), pool_id_prefix, pool_id_index) == 0)
        {
          if (std::strlen (constraint_list[i].constraint_expr) > pool_id_index)
            {
              this->expected_pool_id_ =
                std::atoi (&constraint_list[i].constraint_expr[pool_id_index]);

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG, "(%t) Parsed Pool Id = %d\n", this->expected_pool_id_));
            }
        }
      // Check lane id.
      else if (std::strncmp (constraint_list[i].constraint_expr.in (), lane_id_prefix, lane_id_index) == 0)
        {
          if (std::strlen (constraint_list[i].constraint_expr) > lane_id_index)
            {
              this->expected_lane_id_ =
                std::atoi (&constraint_list[i].constraint_expr[lane_id_index]);

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG, "(%t) Parsed Lane Id = %d\n", this->expected_lane_id_));
            }
        }
    }

  // Create the list that goes out.
  CosNotifyFilter::ConstraintInfoSeq* infoseq_ptr {};
  ACE_NEW_THROW_EX (infoseq_ptr,
                    CosNotifyFilter::ConstraintInfoSeq (0),
                    CORBA::NO_MEMORY ());

  return infoseq_ptr;
}

void
TAO_Notify_Tests_RT_Test_Filter::modify_constraints (
  CosNotifyFilter::ConstraintIDSeq const& /*del_list*/,
  CosNotifyFilter::ConstraintInfoSeq const& /*modify_list*/)
{
}

CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_Tests_RT_Test_Filter::get_constraints (
  CosNotifyFilter::ConstraintIDSeq const& /*id_list*/)
{
  return nullptr;
}

CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_Tests_RT_Test_Filter::get_all_constraints ()
{
  return nullptr;
}

void
TAO_Notify_Tests_RT_Test_Filter::remove_all_constraints ()
{
}

void
TAO_Notify_Tests_RT_Test_Filter::destroy ()
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  PortableServer::POA_var const my_poa = _default_POA ();

  PortableServer::ObjectId_var const object_id = my_poa->servant_to_id (this);

  my_poa->deactivate_object (object_id.in ());
}

CORBA::Boolean
TAO_Notify_Tests_RT_Test_Filter::match (CORBA::Any const& /*filterable_data*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO_Notify_Tests_RT_Test_Filter::match_structured (
  CosNotification::StructuredEvent const& notification)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "TAO_Notify_Tests_RT_Test_Filter (%x) ::match_structured (%t)\n", this));

  TAO_Root_POA* const poa = dynamic_cast<TAO_Root_POA*> (this->_default_POA ());

  ACE_ASSERT (poa != nullptr);

  CORBA::ORB_ptr const orb = poa->orb_core ().orb ();

  CORBA::Object_var const object =
    orb->resolve_initial_references ("RTCurrent");

  RTCORBA::Current_var const current =
    RTCORBA::Current::_narrow (object.in ());

  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources* const tss =
    orb->orb_core ()->get_tss_resources ();

  // Get the lane attribute in TSS.
  TAO_Thread_Lane* const lane = static_cast<TAO_Thread_Lane*> (tss->lane_);

  // Check if the event carries a Priority.
  CosNotification::PropertySeq const& prop_seq = notification.header.variable_header;

  for (CORBA::ULong i {}; i < prop_seq.length (); ++i)
    {
      if (std::strcmp (prop_seq[i].name.in (), CosNotification::Priority) == 0)
        {
          CORBA::Short event_priority {};
          prop_seq[i].value >>= event_priority;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "(%P, %t) Event at %d priority, expected at %d\n",
                        current->the_priority (), event_priority));

          if (current->the_priority () != event_priority)
            {
              ACE_DEBUG ((LM_DEBUG, "(%t) Error: Event at %d priority, expected at %d\n",
                          current->the_priority (), event_priority));
            }

          break;
        }
    }

  if (lane)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    " (%t) Expected (pool = %d; lane = %d), Received (pool = %d; lane = %d)\n",
                    this->expected_pool_id_,
                    this->expected_lane_id_,
                    lane->pool ().id (),
                    lane->id ()));

      if (this->expected_lane_id_ != lane->id () ||
          this->expected_pool_id_ != lane->pool ().id ())
        ACE_DEBUG ((LM_DEBUG,
                    " (%t) Error: Expected (pool = %d; lane = %d), Received (pool = %d; lane = %d)\n",
                    this->expected_pool_id_,
                    this->expected_lane_id_,
                    lane->pool ().id (),
                    lane->id ()));
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    " (%t)(default thread pool) Expected (pool = %d; lane = %d)\n",
                    this->expected_pool_id_,
                    this->expected_lane_id_));

      if (this->expected_lane_id_ != 0 || this->expected_pool_id_ != 0)
        ACE_DEBUG ((LM_DEBUG,
                    " (%t) Error: (default thread pool) Expected (pool = %d; lane = %d)\n",
                    this->expected_pool_id_,
                    this->expected_lane_id_));
    }

  return true;
}

CORBA::Boolean
TAO_Notify_Tests_RT_Test_Filter::match_typed (
  CosNotification::PropertySeq const& /*filterable_data*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyFilter::CallbackID
TAO_Notify_Tests_RT_Test_Filter::attach_callback (
  CosNotifyComm::NotifySubscribe_ptr /*callback*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO_Notify_Tests_RT_Test_Filter::detach_callback (
  CosNotifyFilter::CallbackID /*callback*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyFilter::CallbackIDSeq *
TAO_Notify_Tests_RT_Test_Filter::get_callbacks ()
{
  throw CORBA::NO_IMPLEMENT ();
}
