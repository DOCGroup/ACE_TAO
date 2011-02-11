// $Id$

#include "RT_Test_Filter.h"



#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/PortableServer/Root_POA.h"

TAO_Notify_Tests_RT_Test_Filter::TAO_Notify_Tests_RT_Test_Filter (void)
  : expected_pool_id_ (0)
    , expected_lane_id_ (0)
{
}

TAO_Notify_Tests_RT_Test_Filter::~TAO_Notify_Tests_RT_Test_Filter ()
{

}

char*
TAO_Notify_Tests_RT_Test_Filter::constraint_grammar (void)
{
  return CORBA::string_dup ("ETCL");
}


CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_Tests_RT_Test_Filter::add_constraints (const CosNotifyFilter::ConstraintExpSeq& constraint_list
                                )
{
  const char* PoolId_prefix = "PoolId = ";
  size_t poolId_index = ACE_OS::strlen (PoolId_prefix);

  const char* LaneId_prefix = "LaneId = ";
  size_t laneId_index = ACE_OS::strlen (LaneId_prefix);

  for (CORBA::ULong i = 0; i < constraint_list.length (); ++i)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "(%t) received  = %s\n", constraint_list[i].constraint_expr.in ()));

      // Check Poolid
      if (ACE_OS::strncmp (constraint_list[i].constraint_expr.in (), PoolId_prefix, poolId_index) == 0)
        {
          if (ACE_OS::strlen (constraint_list[i].constraint_expr) > poolId_index)
            {
              this->expected_pool_id_ = ACE_OS::atoi (&constraint_list[i].constraint_expr[poolId_index]);

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG, "(%t) Parsed Pool Id = %d\n", this->expected_pool_id_));
            }
        }
      // Check Poolid
      else if (ACE_OS::strncmp (constraint_list[i].constraint_expr.in (), LaneId_prefix, laneId_index) == 0)
        {
          if (ACE_OS::strlen (constraint_list[i].constraint_expr) > laneId_index)
            {
              this->expected_lane_id_ = ACE_OS::atoi (&constraint_list[i].constraint_expr[laneId_index]);

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG, "(%t) Parsed Lane Id = %d\n", this->expected_lane_id_));
            }
        }
    }

  // Create the list that goes out.
  CosNotifyFilter::ConstraintInfoSeq* infoseq_ptr;
  ACE_NEW_THROW_EX (infoseq_ptr,
                    CosNotifyFilter::ConstraintInfoSeq (0),
                    CORBA::NO_MEMORY ());

  return infoseq_ptr;
}

void
TAO_Notify_Tests_RT_Test_Filter::modify_constraints (const CosNotifyFilter::ConstraintIDSeq & /*del_list*/,
                                   const CosNotifyFilter::ConstraintInfoSeq & /*modify_list*/
                                   )
{

}

CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_Tests_RT_Test_Filter::get_constraints (const CosNotifyFilter::ConstraintIDSeq & /*id_list*/
                                )
{
  return 0;
}

CosNotifyFilter::ConstraintInfoSeq *
TAO_Notify_Tests_RT_Test_Filter::get_all_constraints (void)
{
        return 0;
}

void
TAO_Notify_Tests_RT_Test_Filter::remove_all_constraints (void)
{

}

void
TAO_Notify_Tests_RT_Test_Filter::destroy (void)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  PortableServer::POA_var my_POA = _default_POA ();

  PortableServer::ObjectId_var refTemp = my_POA->servant_to_id (this);

  my_POA->deactivate_object (refTemp.in ());
}

CORBA::Boolean
TAO_Notify_Tests_RT_Test_Filter::match (const CORBA::Any & /*filterable_data */
                           )
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO_Notify_Tests_RT_Test_Filter::match_structured (const CosNotification::StructuredEvent & notification
                                 )
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "TAO_Notify_Tests_RT_Test_Filter (%x) ::match_structured (%t)\n", this));

  TAO_Root_POA *poa = dynamic_cast<TAO_Root_POA*>(this->_default_POA ());

  ACE_ASSERT (poa != 0);

  CORBA::ORB_ptr orb = poa->orb_core ().orb ();

  CORBA::Object_var object =
    orb->resolve_initial_references ("RTCurrent");

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (object.in ());

  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources *tss =
    orb->orb_core ()->get_tss_resources ();

  // Get the lane attribute in TSS.
  TAO_Thread_Lane *lane =
    (TAO_Thread_Lane *) tss->lane_;

  // Check if the event carries a Priority.
  const CosNotification::PropertySeq& prop_seq = notification.header.variable_header;

  for (CORBA::ULong i = 0; i < prop_seq.length (); ++i)
    {
      if (ACE_OS::strcmp (prop_seq[i].name.in (), CosNotification::Priority) == 0)
        {
          CORBA::Short event_priority = 0;
          prop_seq[i].value >>= event_priority;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,"(%P, %t) Event at %d priority, expected at %d\n",
                        current->the_priority (), event_priority));

          if (current->the_priority () != event_priority)
            {
              ACE_DEBUG ((LM_DEBUG,"(%t) Error: Event at %d priority, expected at %d\n",
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

      if (this->expected_lane_id_ != lane->id ()
          ||
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

      if (this->expected_lane_id_ != 0
          ||
          this->expected_pool_id_ != 0)
        ACE_DEBUG ((LM_DEBUG,
                    " (%t) Error: (default thread pool) Expected (pool = %d; lane = %d)\n",
                    this->expected_pool_id_,
                    this->expected_lane_id_));
    }

  return 1;
}

CORBA::Boolean
TAO_Notify_Tests_RT_Test_Filter::match_typed (
                            const CosNotification::PropertySeq & /* filterable_data */
                            )
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyFilter::CallbackID
TAO_Notify_Tests_RT_Test_Filter::attach_callback (
                                CosNotifyComm::NotifySubscribe_ptr /* callback */
                                )
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO_Notify_Tests_RT_Test_Filter::detach_callback (
                                CosNotifyFilter::CallbackID /* callback */
                                )
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyFilter::CallbackIDSeq *
TAO_Notify_Tests_RT_Test_Filter::get_callbacks (void)
{
  throw CORBA::NO_IMPLEMENT ();
}
