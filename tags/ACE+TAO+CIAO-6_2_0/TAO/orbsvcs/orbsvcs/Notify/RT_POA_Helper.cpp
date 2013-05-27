// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/RT_POA_Helper.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/debug.h"
#include "orbsvcs/NotifyExtC.h"
#include "orbsvcs/Notify/RT_Properties.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_RT_POA_Helper::~TAO_Notify_RT_POA_Helper ()
{
}

void
TAO_Notify_RT_POA_Helper::init (PortableServer::POA_ptr parent_poa, const NotifyExt::ThreadPoolParams& tp_params)
{
  ACE_CString child_poa_name = this->get_unique_id ();

  this->init (parent_poa, child_poa_name.c_str (), tp_params);
}

void
TAO_Notify_RT_POA_Helper::init (PortableServer::POA_ptr parent_poa, const char* poa_name
                                 , const NotifyExt::ThreadPoolParams& tp_params)
{
  CORBA::PolicyList policy_list (4);

  this->set_policy (parent_poa, policy_list);

  RTCORBA::RTORB_var rt_orb = TAO_Notify_RT_PROPERTIES::instance ()->rt_orb ();

  RTCORBA::PriorityModel priority_model =
    tp_params.priority_model == NotifyExt::CLIENT_PROPAGATED ?
    RTCORBA::CLIENT_PROPAGATED : RTCORBA::SERVER_DECLARED;

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "Priority Model = %d, Server prio = %d\n"
                , tp_params.priority_model, tp_params.server_priority));

  policy_list.length (3);
  policy_list[2] =
    rt_orb->create_priority_model_policy (priority_model,
                                          tp_params.server_priority);

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "Creating threadpool: static threads = %d, def. prio = %d\n"
                , tp_params.static_threads, tp_params.default_priority));

  // Create the thread-pool.
  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool (tp_params.stacksize,
                               tp_params.static_threads,
                               tp_params.dynamic_threads,
                               tp_params.default_priority,
                               tp_params.allow_request_buffering,
                               tp_params.max_buffered_requests,
                               tp_params.max_request_buffer_size);

  policy_list.length (4);
  policy_list[3] =
    rt_orb->create_threadpool_policy (threadpool_id);

  this->create_i (parent_poa, poa_name, policy_list);
}

void
TAO_Notify_RT_POA_Helper::init (PortableServer::POA_ptr parent_poa, const NotifyExt::ThreadPoolLanesParams& tpl_params)
{
  ACE_CString child_poa_name = this->get_unique_id ();

  this->init (parent_poa, child_poa_name.c_str (), tpl_params);
}

void
TAO_Notify_RT_POA_Helper::init (PortableServer::POA_ptr parent_poa, const char* poa_name
                                 , const NotifyExt::ThreadPoolLanesParams& tpl_params)
{
  CORBA::PolicyList policy_list (4);

  this->set_policy (parent_poa, policy_list);

  RTCORBA::RTORB_var rt_orb = TAO_Notify_RT_PROPERTIES::instance ()->rt_orb ();

  RTCORBA::PriorityModel priority_model =
    tpl_params.priority_model == NotifyExt::CLIENT_PROPAGATED ?
    RTCORBA::CLIENT_PROPAGATED : RTCORBA::SERVER_DECLARED;

  policy_list.length (3);
  policy_list[2] =
    rt_orb->create_priority_model_policy (priority_model,
                                          tpl_params.server_priority);

  // Populate RTCORBA Lanes.
  RTCORBA::ThreadpoolLanes lanes (tpl_params.lanes.length ());
  lanes.length (tpl_params.lanes.length ());

  for (CORBA::ULong index = 0; index < tpl_params.lanes.length (); ++index)
    {
      if (TAO_debug_level > 0)
            {
              ORBSVCS_DEBUG ((LM_DEBUG, "Creating threadpool lane %d: priority = %d, static threads = %d\n",
                          index, tpl_params.lanes[index].lane_priority, tpl_params.lanes[index].static_threads));
            }

      lanes[index].lane_priority = tpl_params.lanes[index].lane_priority;
      lanes[index].static_threads = tpl_params.lanes[index].static_threads;
      lanes[index].dynamic_threads = tpl_params.lanes[index].dynamic_threads;
    }

  // Create the thread-pool.
  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool_with_lanes (tpl_params.stacksize,
                                          lanes,
                                          tpl_params.allow_borrowing,
                                          tpl_params.allow_request_buffering,
                                          tpl_params.max_buffered_requests,
                                          tpl_params.max_request_buffer_size);

  policy_list.length (4);
  policy_list[3] =
    rt_orb->create_threadpool_policy (threadpool_id);

  this->create_i (parent_poa, poa_name, policy_list);
}

void
TAO_Notify_RT_POA_Helper::init (PortableServer::POA_ptr parent_poa)
{
  CORBA::PolicyList policy_list (1);

  RTCORBA::RTORB_var rt_orb = TAO_Notify_RT_PROPERTIES::instance ()->rt_orb ();

  policy_list.length (1);

  policy_list[0] =
    rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                          0);

  ACE_CString child_poa_name = this->get_unique_id ();

  this->create_i (parent_poa, child_poa_name.c_str (), policy_list);
}

TAO_END_VERSIONED_NAMESPACE_DECL
