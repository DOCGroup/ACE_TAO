// $Id$

#include "RT_Test_Filter.h"

ACE_RCSID(Notify, TAO_RT_Test_Filter, "$id$")

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/PortableServer/POA.h"

TAO_NS_RT_Test_Filter::TAO_NS_RT_Test_Filter (void)
  : expected_pool_id_ (0)
    , expected_lane_id_ (0)
{
}

TAO_NS_RT_Test_Filter::~TAO_NS_RT_Test_Filter ()
{

}

char*
TAO_NS_RT_Test_Filter::constraint_grammar (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CORBA::string_dup ("ETCL");
}


CosNotifyFilter::ConstraintInfoSeq*
TAO_NS_RT_Test_Filter::add_constraints (const CosNotifyFilter::ConstraintExpSeq& constraint_list
                                ACE_ENV_ARG_DECL
                                )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::InvalidConstraint
                   ))
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
  ACE_CHECK_RETURN (0);

  return infoseq_ptr;
}

void
TAO_NS_RT_Test_Filter::modify_constraints (const CosNotifyFilter::ConstraintIDSeq & /*del_list*/,
                                   const CosNotifyFilter::ConstraintInfoSeq & /*modify_list*/
                                   ACE_ENV_ARG_DECL_NOT_USED
                                   )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::InvalidConstraint,
                   CosNotifyFilter::ConstraintNotFound
                   ))
{

}

CosNotifyFilter::ConstraintInfoSeq*
TAO_NS_RT_Test_Filter::get_constraints (const CosNotifyFilter::ConstraintIDSeq & /*id_list*/
                                ACE_ENV_ARG_DECL_NOT_USED
                                )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyFilter::ConstraintNotFound))
{
  return 0;
}

CosNotifyFilter::ConstraintInfoSeq *
TAO_NS_RT_Test_Filter::get_all_constraints (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
        return 0;
}

void
TAO_NS_RT_Test_Filter::remove_all_constraints (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
TAO_NS_RT_Test_Filter::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  PortableServer::POA_var my_POA = _default_POA ();

  PortableServer::ObjectId_var refTemp = my_POA->servant_to_id (this);

  my_POA->deactivate_object (refTemp.in ());
}

CORBA::Boolean
TAO_NS_RT_Test_Filter::match (const CORBA::Any & /*filterable_data */
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyFilter::UnsupportedFilterableData))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

CORBA::Boolean
TAO_NS_RT_Test_Filter::match_structured (const CosNotification::StructuredEvent & notification
                                 ACE_ENV_ARG_DECL
                                 )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyFilter::UnsupportedFilterableData))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "TAO_NS_RT_Test_Filter (%x) ::match_structured (%t)\n", this));

  TAO_POA *poa = this->_default_POA ()->_tao_poa_downcast();

  ACE_ASSERT (poa != 0);

  CORBA::ORB_ptr orb = poa->orb_core ().orb ();

  CORBA::Object_var object =
    orb->resolve_initial_references ("RTCurrent"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (object.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

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
TAO_NS_RT_Test_Filter::match_typed (
                            const CosNotification::PropertySeq & /* filterable_data */
                            ACE_ENV_ARG_DECL
                            )
  ACE_THROW_SPEC (( CORBA::SystemException,
                    CosNotifyFilter::UnsupportedFilterableData))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

CosNotifyFilter::CallbackID
TAO_NS_RT_Test_Filter::attach_callback (
                                CosNotifyComm::NotifySubscribe_ptr /* callback */
                                ACE_ENV_ARG_DECL
                                )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

void
TAO_NS_RT_Test_Filter::detach_callback (
                                CosNotifyFilter::CallbackID /* callback */
                                ACE_ENV_ARG_DECL
                                )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyFilter::CallbackNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosNotifyFilter::CallbackIDSeq *
TAO_NS_RT_Test_Filter::get_callbacks (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
